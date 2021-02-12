//http://www.codeproject.com/KB/threads/ReentrantReaderWriterLock.aspx

#ifdef _MANAGED
#include <vcclr.h>
#endif

#include <set>
namespace nTrack {
	namespace Sync {		
class ReadWriteLockTLSRelease {
public:
	class TimeoutExpiredException : std::exception {};
	class ImplicitEscalationException : std::exception {};
private:
	volatile int numReaders;
	int numWriters;
	CRITICAL_SECTION atomizer;
	HANDLE spinEvent;
	static const int WaitSpinTimeout=1000; // If the timeout exipres it's not a big deal, as the ClaimXX() function rechecks if it can claim lock
	static const int MaxSpinIterations=4000;
	static const int MaxSpinIterationsWrite=4000;
	int writerThreadId;
	int tlsSlot;
	int anyThreadWaiting;
	
	class EscalatingPolicyAllow
	{
	public:
		static bool AllowImplicitEscalation()
		{
			return true;
		}
	};
	class EscalatingPolicyDeny
	{
	public:
		static bool AllowImplicitEscalation()
		{
			return false;
		}
	};
	__forceinline int GetTLSReaderCount()
	{
		return (int)(INT_PTR)TlsGetValue(tlsSlot);
	}
	__forceinline void SetTLSReaderCount(int count)
	{
		TlsSetValue(tlsSlot, (LPVOID)(INT_PTR)count);
	}
	__forceinline void SpinThreads()
	{
		if(anyThreadWaiting>0) PulseEvent(spinEvent); // Unreliable but in case of failure the timeout rescues us
	}
	template<class EscalatingPolicy> inline bool CheckUpgradingFromReaderToWriter()
	{
		if(numReaders==0) return false;
		int readerCount=GetTLSReaderCount();
		if(readerCount>0) {
			// exit read lock
			if(!EscalatingPolicy::AllowImplicitEscalation()) 
				throw ImplicitEscalationException();
			SetTLSReaderCount(-readerCount);
			while(true) {
				int old=numReaders;
				if(old==InterlockedCompareExchange((LONG*)&numReaders, numReaders-readerCount, old))
					break;
			}
			return true;
		}
		return false;
	}
	inline void CheckRestorePreviousReaderLock()
	{
		int previous=-GetTLSReaderCount();
		if(previous>0) {
			SetTLSReaderCount(previous);
			numReaders=previous;
		}
	}
	inline void IncrementReaderCount()
	{
		SetTLSReaderCount(GetTLSReaderCount()+1);
	}
	__forceinline void Spin()
	{
		Sleep(0);
	}
	__forceinline void AcquireReader()
	{
		_ASSERT(numReaders>=0);
		InterlockedIncrement((LONG*)&numReaders);
		IncrementReaderCount();
		_ASSERT(numWriters==0);//||(numWriters>0&&writerThreadId==myThreadId));
		LeaveCriticalSection(&atomizer);
	}
	__forceinline void AcquireWriter(int myThreadId)
	{
		numWriters++;
		_ASSERT(numReaders==0);
		writerThreadId=myThreadId;
		LeaveCriticalSection(&atomizer);
	}
	class TimeoutIgnore
	{
	public:
		__forceinline void CheckExpired(int timeout) const {}
	};
	class TimeoutChecker
	{
		unsigned long ticksAtStart;
	public:
		TimeoutChecker() 
		{
			ticksAtStart=timeGetTime();
		}
		void CheckExpired(int timeout)
		{
			if(int(timeGetTime()-ticksAtStart)>timeout) throw TimeoutExpiredException();
		}
	};
	template<class TimeoutPolicy> __forceinline void ClaimReaderInternal(int timeout)
	{
		_ASSERT(numReaders>=0);
		int myThreadId=GetCurrentThreadId();
		if(myThreadId==writerThreadId) return; // Grant read access if thread already has write access
		int old=numReaders;
		if(old>0) {
			if(old==InterlockedCompareExchange((LONG*)&numReaders, old+1, old)) {
#ifdef _DEBUG
				//printf("InterlockedCompareExchange() - numReaders: %d\n", numReaders);
				{
					char str[70];
					sprintf(str, "InterlockedCompareExchange() - numReaders: %d\n", numReaders);
					OutputDebugString(str);
				}
#endif
				_ASSERT(numReaders>=0);
				IncrementReaderCount();
				return;
			}
		}
		TimeoutPolicy t;
		for(int i=0; i<MaxSpinIterations; ++i)
		{
			if(numWriters==0) {
				EnterCriticalSection(&atomizer);
				if(numWriters==0) {
					AcquireReader();
					return;
				}
				LeaveCriticalSection(&atomizer);
			}
			t.CheckExpired(timeout);
			Spin();
		}
#ifdef _DEBUG
		printf("ClaimReader() Active spin failed\n");
#endif
		while(true) {
			EnterCriticalSection(&atomizer);
			if(numWriters==0) break;		
			InterlockedIncrement((LONG*)&anyThreadWaiting);
			LeaveCriticalSection(&atomizer);
			t.CheckExpired(timeout);
			WaitForSingleObject(spinEvent, WaitSpinTimeout);
			InterlockedDecrement((LONG*)&anyThreadWaiting);
		}
		AcquireReader();
	}
	template<class TimeoutPolicy, class EscalatingPolicy> __forceinline void ClaimWriterInternal(int timeout)
	{
		_ASSERT(numReaders>=0);
		int myThreadId=GetCurrentThreadId();
		TimeoutPolicy t;
		for(int i=0; i<MaxSpinIterationsWrite; ++i) {
			EnterCriticalSection(&atomizer);
			
			if(numWriters==1&&myThreadId==writerThreadId) {
				// Reentering write lock
				AcquireWriter(myThreadId);
				return;
			}
			
			CheckUpgradingFromReaderToWriter<EscalatingPolicy>();

			if(numReaders==0&&numWriters==0) {
				AcquireWriter(myThreadId);
				return;
			}
			LeaveCriticalSection(&atomizer);
			t.CheckExpired(timeout);
			Spin();
		}
#ifdef _DEBUG
		printf("ClaimWriter() Active spin failed\n");
#endif
		while(true) {
			EnterCriticalSection(&atomizer);
			
			CheckUpgradingFromReaderToWriter<EscalatingPolicy>();

			if(numReaders==0&&numWriters==0) {
				AcquireWriter(myThreadId);
				return;
			}
			t.CheckExpired(timeout);
			InterlockedIncrement((LONG*)&anyThreadWaiting);
			LeaveCriticalSection(&atomizer);
			WaitForSingleObject(spinEvent, WaitSpinTimeout);
			InterlockedDecrement((LONG*)&anyThreadWaiting);
		}
	}
public:
	~ReadWriteLockTLSRelease()
	{
		anyThreadWaiting=0;
		DeleteCriticalSection(&atomizer);
		CloseHandle(spinEvent);
		TlsFree(tlsSlot);
	}
	ReadWriteLockTLSRelease()
	{
		InitializeCriticalSection(&atomizer);
		numReaders=numWriters=0;
		spinEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
		tlsSlot=TlsAlloc(); // The slot default value is 0 (NULL) for each thread (not clearly documented)
		if(tlsSlot==TLS_OUT_OF_INDEXES) throw std::exception("Out of TLS slots");
	}
	void ClaimWriterAllowEscalating()
	{
		return ClaimWriterInternal<TimeoutIgnore, EscalatingPolicyAllow>(INFINITE);
	}
	void ClaimWriterAllowEscalating(int timeout)
	{
		return ClaimWriterInternal<TimeoutChecker, EscalatingPolicyAllow>(timeout);
	}
	void ClaimWriterNoEscalating()
	{
		return ClaimWriterInternal<TimeoutIgnore, EscalatingPolicyDeny>(INFINITE);
	}
	void ClaimWriterNoEscalating(int timeout)
	{
		return ClaimWriterInternal<TimeoutChecker, EscalatingPolicyDeny>(timeout);
	}
	void ClaimReader()
	{
		return ClaimReaderInternal<TimeoutIgnore>(INFINITE);
	}
	void ClaimReader(int timeout)
	{
		return ClaimReaderInternal<TimeoutChecker>(timeout);
	}
	void ReleaseWriter()
	{
		_ASSERT(numReaders>=0);
		EnterCriticalSection(&atomizer);
		_ASSERT(numWriters>0);
		numWriters--;
		if(0==numWriters) writerThreadId=0;
		CheckRestorePreviousReaderLock();
		SpinThreads();
		LeaveCriticalSection(&atomizer);
	}
	
	void ReleaseReader()
	{
		_ASSERT(numReaders>=0);
		EnterCriticalSection(&atomizer);
		int myThreadId=GetCurrentThreadId();
		if(numWriters==0) // if numWriters>0 I am also have the writer lock
		{
			InterlockedDecrement((LONG*)&numReaders);
			SetTLSReaderCount(GetTLSReaderCount()-1);
			_ASSERT(numReaders>=0);
			_ASSERT(numWriters==0);

			SpinThreads();
		}
		LeaveCriticalSection(&atomizer);
	}
};
		class ReadWriteLockTLS {
			CRITICAL_SECTION atomizer;
			int numReaders;
			int numWriters;
			HANDLE spinEvent;
			static const int WaitSpinTimeout=1000; // If the timeout exipres it's not a big deal, as the ClaimXX() function rechecks if it can claim lock
			int writerThreadId;
			__forceinline int GetTLSReaderCount()
			{
				return (int)(INT_PTR)TlsGetValue(tlsSlot);
			}
			__forceinline void SetTLSReaderCount(int count)
			{
				TlsSetValue(tlsSlot, (LPVOID)(INT_PTR)count);
			}
			int tlsSlot;
		public:
			~ReadWriteLockTLS()
			{
				DeleteCriticalSection(&atomizer);
				CloseHandle(spinEvent);
				TlsFree(tlsSlot);
			}
			ReadWriteLockTLS()
			{
				InitializeCriticalSection(&atomizer);
				numReaders=numWriters=0;
				spinEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
				tlsSlot=TlsAlloc(); // The slot default value is 0 (NULL) for each thread (not clearly documented)
			}
			void ClaimReader()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(numWriters>0) {
						// Grant read access if thread already has write access
						if(myThreadId==writerThreadId) {
							break;
						}
					} else if(numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);						
				}	
				numReaders++;
				SetTLSReaderCount(GetTLSReaderCount()+1);
				_ASSERT(numWriters==0||(numWriters>0&&writerThreadId==myThreadId));
				LeaveCriticalSection(&atomizer);
			}
			void ReleaseWriter()
			{
				EnterCriticalSection(&atomizer);
				_ASSERT(numWriters>0);
				numWriters--;
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		private:
			__forceinline void SpinThreads()
			{
				PulseEvent(spinEvent); // Unreliable but in case of failure there's always the wait timeout 
			}
			inline bool ThereIsASingleReaderAndItIsMe()
			{
				if(numReaders==0) return false;
				if(GetTLSReaderCount()==numReaders) return true;
				return false;
			}
		public:
			void ClaimWriter()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(ThereIsASingleReaderAndItIsMe()) // Escalate lock from read to write
						break;
					if(numWriters==1&&myThreadId==writerThreadId) 
						break; // Reentering write lock
					if(numReaders==0&&numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);
				}
				numWriters++;
				_ASSERT(numReaders==0||ThereIsASingleReaderAndItIsMe());
				writerThreadId=myThreadId;
				LeaveCriticalSection(&atomizer);
				return;
			}
			void ReleaseReader()
			{
				EnterCriticalSection(&atomizer);
				numReaders--;
				SetTLSReaderCount(GetTLSReaderCount()-1);
				int myThreadId=GetCurrentThreadId();
				_ASSERT(numReaders>=0);
				_ASSERT((ThereIsASingleReaderAndItIsMe()||numWriters==0)||numReaders==0);
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		};
		class ReadWriteLockReentrant {
			CRITICAL_SECTION atomizer;
			int numReaders;
			int numWriters;
			HANDLE spinEvent;
			static const int WaitSpinTimeout=1000; // If the timeout exipres it's not a big deal, as the ClaimXX() function rechecks if it can claim lock
			typedef multiset<int> ReadersIdsContainer;
			ReadersIdsContainer readerThreadIds;
			int writerThreadId;
		public:
			ReadWriteLockReentrant()
			{
				InitializeCriticalSection(&atomizer);
				numReaders=numWriters=0;
				spinEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
			}
			void ClaimReader()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(numWriters>0) {
						// Grant read access if thread already has write access
						if(myThreadId==writerThreadId) {
							break;
						}
					} else if(numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);						
				}	
				numReaders++;
				readerThreadIds.insert(GetCurrentThreadId());
				_ASSERT(numWriters==0||(numWriters>0&&writerThreadId==myThreadId));
				LeaveCriticalSection(&atomizer);
			}
			void ReleaseWriter()
			{
				EnterCriticalSection(&atomizer);
				_ASSERT(numWriters>0);
				numWriters--;
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		private:
			__forceinline void SpinThreads()
			{
				PulseEvent(spinEvent); // Unreliable but in case of failure the timeout rescues us
			}
			inline bool ThereIsASingleReaderAndItIsMe(int myThreadId)
			{
				if(numReaders==0) return false;
				for(ReadersIdsContainer::iterator it=readerThreadIds.begin(); it!=readerThreadIds.end(); ++it)
					if(*it!=myThreadId) 
						return false;
				return true;
			}
		public:
			void ClaimWriter()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(ThereIsASingleReaderAndItIsMe(myThreadId)) // Escalate lock from read to write
						break;
					if(numWriters==1&&myThreadId==writerThreadId) 
						break; // Reentering write lock
					if(numReaders==0&&numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);
				}
				numWriters++;
				_ASSERT(numReaders==0||ThereIsASingleReaderAndItIsMe(myThreadId));
				writerThreadId=myThreadId;
				LeaveCriticalSection(&atomizer);
				return;
			}
			void ReleaseReader()
			{
				EnterCriticalSection(&atomizer);
				numReaders--;
				int myThreadId=GetCurrentThreadId();
				readerThreadIds.erase(readerThreadIds.find(GetCurrentThreadId()));
				_ASSERT(numReaders>=0);
				_ASSERT(readerThreadIds.size()==numReaders);
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		};
		class ReadWriteLockReentrantNoSpin {
			CRITICAL_SECTION atomizer;
			int numReaders;
			int numWriters;
			
			typedef multiset<int> ReadersIdsContainer;
			ReadersIdsContainer readerThreadIds;
			int writerThreadId;
		public:
			ReadWriteLockReentrantNoSpin()
			{
				InitializeCriticalSection(&atomizer);
				numReaders=numWriters=0;
			}
			void ClaimReader()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(numWriters>0) {
						// Grant read access if thread already has write access
						if(myThreadId==writerThreadId) {
							break;
						}
					} else if(numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
				}	
				numReaders++;
				readerThreadIds.insert(GetCurrentThreadId());
				_ASSERT(numWriters==0||(numWriters>0&&writerThreadId==myThreadId));
				LeaveCriticalSection(&atomizer);
			}
			void ReleaseWriter()
			{
				EnterCriticalSection(&atomizer);
				_ASSERT(numWriters>0);
				numWriters--;
				LeaveCriticalSection(&atomizer);
			}
		private:
			inline bool ThereIsASingleReaderAndItIsMe(int myThreadId)
			{
				if(numReaders==0) return false;
				for(ReadersIdsContainer::iterator it=readerThreadIds.begin(); it!=readerThreadIds.end(); ++it)
					if(*it!=myThreadId) 
						return false;
				return true;
			}
		public:
			void ClaimWriter()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(ThereIsASingleReaderAndItIsMe(myThreadId)) // Escalate lock from read to write
						break;
					if(numWriters==1&&myThreadId==writerThreadId) 
						break; // Reentering write lock
					if(numReaders==0&&numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					continue;
				}
				numWriters++;
				_ASSERT(numReaders==0||ThereIsASingleReaderAndItIsMe(myThreadId));
				writerThreadId=myThreadId;
				LeaveCriticalSection(&atomizer);
				return;
			}
			void ReleaseReader()
			{
				EnterCriticalSection(&atomizer);
				numReaders--;
				int myThreadId=GetCurrentThreadId();
				readerThreadIds.erase(readerThreadIds.find(GetCurrentThreadId()));
				_ASSERT(numReaders>=0);
				_ASSERT(readerThreadIds.size()==numReaders);
				LeaveCriticalSection(&atomizer);
			}
		};
		class ReadWriteLockNoReentrance {
			CRITICAL_SECTION atomizer;
			int numReaders;
			int numWriters;
			HANDLE spinEvent;
			
			typedef multiset<int> ReadersIdsContainer;
			int writerThreadId;
			__forceinline void SpinThreads()
			{
				PulseEvent(spinEvent); // Unreliable but in case of failure the timeout rescues us
			}
			static const int WaitSpinTimeout=10000; // If the timeout exipres it's not a big deal, as the ClaimXX() function rechecks if it can claim lock
		public:
			ReadWriteLockNoReentrance()
			{
				InitializeCriticalSection(&atomizer);
				numReaders=numWriters=0;
				spinEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
			}
			void ClaimReader()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);						
				}	
				numReaders++;
				_ASSERT(numWriters==0||(numWriters>0&&writerThreadId==myThreadId));
				LeaveCriticalSection(&atomizer);
			}
			void ReleaseWriter()
			{
				EnterCriticalSection(&atomizer);
				_ASSERT(numWriters>0);
				numWriters--;
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		public:
			void ClaimWriter()
			{
				int myThreadId=GetCurrentThreadId();
				while(true) {
					EnterCriticalSection(&atomizer);
					if(numReaders==0&&numWriters==0)
						break;
					LeaveCriticalSection(&atomizer);
					WaitForSingleObject(spinEvent, WaitSpinTimeout);
					continue;
				}
				numWriters++;
				_ASSERT(numReaders==0);
				writerThreadId=myThreadId;
				LeaveCriticalSection(&atomizer);
				return;
			}
			void ReleaseReader()
			{
				EnterCriticalSection(&atomizer);
				numReaders--;
				int myThreadId=GetCurrentThreadId();
				_ASSERT(numReaders>=0);
				SpinThreads();
				LeaveCriticalSection(&atomizer);
			}
		};
		class ReadWriteLockCritSec {
			CRITICAL_SECTION atomizer;
		public:
			ReadWriteLockCritSec()
			{
				InitializeCriticalSection(&atomizer);
			}
			void ClaimReader()
			{
				EnterCriticalSection(&atomizer);
			}
			void ClaimWriter()
			{
				EnterCriticalSection(&atomizer);
			}
			void ReleaseWriter()
			{
				LeaveCriticalSection(&atomizer);
			}
			void ReleaseReader()
			{
				LeaveCriticalSection(&atomizer);
			}
		};
		class ReadWriteLockNoLock {
		public:
			void ClaimReader()
			{
			}
			void ClaimWriter()
			{
			}
			void ReleaseWriter()
			{
			}
			void ReleaseReader()
			{
			}
		};
		class ReadWriteLockNoRwMutex {
			HANDLE atomizer;
		public:
			ReadWriteLockNoRwMutex()
			{
				atomizer=CreateMutex(NULL, FALSE, NULL);
			}
			void ClaimReader()
			{
				WaitForSingleObject(atomizer, INFINITE);
			}
			void ClaimWriter()
			{
				WaitForSingleObject(atomizer, INFINITE);
			}
			void ReleaseWriter()
			{
				ReleaseMutex(atomizer);
			}
			void ReleaseReader()
			{
				ReleaseMutex(atomizer);
			}
		};

		#ifdef _MANAGED
		class ReadWriteLockDotNet {
			gcroot<System::Threading::ReaderWriterLock ^> rwLock;
			static const int timeout=10000;
		public:
			ReadWriteLockDotNet()
			{
				rwLock=gcnew System::Threading::ReaderWriterLock();
			}
			void ClaimReader()
			{
				rwLock->AcquireReaderLock(timeout);
			}
			void ClaimWriter()
			{
				rwLock->AcquireWriterLock(timeout);
			}
			void ReleaseWriter()
			{
				rwLock->ReleaseWriterLock();
			}
			void ReleaseReader()
			{
				rwLock->ReleaseReaderLock();
			}
		};
		#endif
		//typedef ReadWriteLockCritSec ReadWriteLock;
		typedef ReadWriteLockTLSRelease ReadWriteLock;

		class AutoLockReader
		{
			ReadWriteLock *underlying;
		public:
			AutoLockReader(ReadWriteLock *_underlying) : underlying(_underlying)
			{
				underlying->ClaimReader();
			}
			AutoLockReader(ReadWriteLock *_underlying, int timeout) : underlying(_underlying)
			{
				underlying->ClaimReader(timeout);
			}
			~AutoLockReader()
			{
				underlying->ReleaseReader();
			}
		};
		class AutoLockWriter
		{
			ReadWriteLock *underlying;
		public:
			AutoLockWriter(ReadWriteLock *_underlying) : underlying(_underlying)
			{
				underlying->ClaimWriterAllowEscalating();
			}
			AutoLockWriter(ReadWriteLock *_underlying, int timeout) : underlying(_underlying)
			{
				underlying->ClaimWriterAllowEscalating(timeout);
			}
			~AutoLockWriter()
			{
				underlying->ReleaseWriter();
			}
		};
		class AutoLockWriterNoEscalating
		{
			ReadWriteLock *underlying;
		public:
			AutoLockWriterNoEscalating(ReadWriteLock *_underlying) : underlying(_underlying)
			{
				underlying->ClaimWriterNoEscalating();
			}
			AutoLockWriterNoEscalating(ReadWriteLock *_underlying, int timeout) : underlying(_underlying)
			{
				underlying->ClaimWriterAllowEscalating(timeout);
			}
			~AutoLockWriterNoEscalating()
			{
				underlying->ReleaseWriter();
			}
		};
	}
}