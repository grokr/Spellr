#ifndef __IOFUNCTIONS_H__
#define __IOFUNCTIONS_H__

#include "Log.h"

#include <list>
#include <iterator>
#include <fstream>
#include <utility>

class TYPODLL_API Output
{
public:
	bool _bIsGood;
	Output(const string& filename){ 
		_bIsGood = true;
		ofs.open(filename.c_str(), ios::trunc);
		if (false == ofs.good()){
			_bIsGood = false;
		}
	}

	void update(const string& to_fix, const string& punctuation, const vector <ds_pair> &cand_list)
	{
		if (cand_list.empty()) 	
			ofs << to_fix << punctuation; 
		else if ((cand_list.front()).first > (cand_list.back()).first)
			ofs << (cand_list.front()).second << punctuation; 
		else
			ofs << (cand_list.back()).second << punctuation; 
	}

	void update(const string& s) { ofs << s;}
	void update(const string& s1, const string& s2) { ofs << s1 << s2;}


private:
	ofstream ofs;

};

/*
//input: ecspresif proces litereche aksplorertin filosofi aspasheli welyiu akspireans artikol ebaut
//output format: o="offset" l="word's length" s="suggestions"
<?xml version="1.0" encoding="UTF-8"?>							XML_HEADER
<spellresult error="0" clipped="0" charschecked="91">			XML_HEADER_START
		<c o="0" l="9" s="1">ekspresif expressive express expressing expires expressed</c>
</spellresult>													XML_HEADER_END
*/
class TYPODLL_API XmlOutput //: public Output
{
public:

	XmlOutput(){} 

	void init(const string& filename)
	{
		//const typo_string XML_HEADER_START=_T("<spellresult error=\"0\" clipped=\"0\" charschecked=\"91\">");
		const typo_string XML_HEADER_START=_T("<spellresult error=\"0\" clipped=\"0\">");
		_offset = 0;
		_bIsGood = true;

		ofs.open(filename.c_str(), ios::trunc);
		if (false == ofs.good()){
			_bIsGood = false;
		}
		else{
			ofs << XML_HEADER << XML_HEADER_START  << endl;
		}
	}


	~XmlOutput()
	{
		ofs << XML_HEADER_END  << endl;
	}
//	virtual void update(const string& s) { ofs << s;}
//	virtual void update(const string& s1, const string& s2) { ofs << s1 << s2;}
	virtual void update(const string& s1, const vector<ds_pair>& v);
	bool _bIsGood;
private:
	ofstream ofs;
	int _offset;
};
/*
class TYPODLL_API TxtOutput  : public Output
{
public:
	TxtOutput(const string& filename){ 
		_bIsGood = true;
		ofs.open(filename.c_str(), ios::trunc);
		if (false == ofs.good()){
			_bIsGood = false;
		}
	}
	~TxtOutput(){
		
	}
//	virtual void update(const string& s1, const string& s2) { ofs << s1 << s2; }
	
	bool _bIsGood;
private:

	ofstream ofs;
};

class TYPODLL_API AspellOutput : public Output
{
public:
	AspellOutput(const string& filename){ 
		_bIsGood = true;
		ofs.open(filename.c_str(), ios::trunc);
		if (false == ofs.good()){
			_bIsGood = false;
		}
	}
//	void addXmlSection(const typo_string &to_fix, 
//		const vector <ds_pair> &cand_list, iostream &output);
	virtual void update(const typo_string &to_fix, const vector <ds_pair> &cand_list);//updateAspellResults
//	void updateStdResults(const vector <string> &best_choices, const string& path);
//	int printFile(const string& path);
	virtual int fileCopy(const typo_string& from, const typo_string& to);
	bool _bIsGood;
private:
	ofstream ofs;//output;
		
};

class TYPODLL_API XmlOutput : public Output
{
public:
	XmlOutput(const string& filename): _bIsGood(true){
		ofs.open(filename.c_str(), ios::trunc);
		if (false == ofs.good()){
			_bIsGood = false;
		}
		else{
			ofs << XML_HEADER << endl <<  XML_HEAD_START  << endl;
		}
	}
	~XmlOutput()
	{
		ofs << XML_HEAD_END  << endl;
	}
//	virtual void update(const string& s) { ofs << s;}
//	virtual void update(const string& s1, const string& s2) { ofs << s1 << s2;}
	virtual void update(const string& s1, const vector<ds_pair>& v);
	bool _bIsGood;
private:
	ofstream ofs;
};
*/

class ReadFile{
public:
	ReadFile(){}
	bool get_next(typo_string& next);
	bool read_utf8_file(const typo_string& pathname, typo_string &content)//std::wstring 
	{
		//typo_string content; //wchar_t *
		ifstream::pos_type file_size;
		wifstream file_stream;

		file_stream.open( pathname.c_str(), ios_base::in | ios_base::binary | ios_base::ate );
		if ( file_stream.is_open() ) {
			file_size = file_stream.tellg();
			//file_content = new wchar_t[file_size];
			file_stream.seekg( 0, ios_base::beg );
			file_stream.read((wchar_t* ) content.c_str(), (streamsize)file_size );
			file_stream.close();
		}
	}
};



class IOManipulators{

public:

	IOManipulators();
	void trim(typo_string& str);

	int fileCopy(const typo_string& from, const typo_string& to)
	{
#ifdef _WIN32
		typo_string command = "copy "; command += from; command += C_SPACE; command += to;
		system(command.c_str());
#else
		typo_string command = "cp "; command += from; command += C_SPACE; command += to;
		system(unix_command.c_str());
#endif
		return RET_FSUCCESS;
	}	

	int io_read(const typo_string &filename, map<typo_string, typo_long_num> &);
	int io_read(const typo_string &filename, map<typo_string, vector<typo_string>> &vs);
	int io_read(const typo_string &filename, map<sss_pair, typo_long_num> &m);
	int io_read_synonymsMap(const typo_string &filename, synonymsDict &m); //map<typo_string,  vector<ds_pair>>
//	int io_read(const typo_string &_lmpath, map<typo_string, spsi_pair> &mssd);
	int io_read(const typo_string &_lmpath,  map<typo_string, vector<si_pair>> &vs);



	template<typename T>
	int io_read(const typo_string &filename, vector <T>& v){
		ifstream is;
		is.open(filename.c_str());
		typo_string word=S_EMPTY;
		//int MaxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS]["MaxDictSize"].c_str());
		while (!is.eof()){ ////TODO add MaxDictSize parameter: && ((int)v.size() < MaxDictSize)){
			is >> word;
			v.push_back(T(word, D_ZERO));
		}
		is.close();
	}

	int io_read(const typo_string &filename, vector<typo_string> &dict);
	int io_read(const typo_string &filename, typo_string &content);
	//	int io_read(const istreambuf &is);

//////////////////////////////	io_write	/////////////////////////////////////////////


/*	template<typename T>
	int io_write(const typo_string &filename, const vector<T> &v, const typo_string &delim)
	{
		ofstream os;
		os.open(filename.c_str());
		if (!os.good()){		
		//	errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		vector<T>::const_iterator it = v.begin();  
		for (it = v.begin(); it != v.end(); it++){
			os << (*it).getData(); 
			os << delim; 
			os << (*it).getFreq();
			os << S_NL;
		}
		os.close();
		return RET_FSUCCESS;
	}
*/
	template<typename T>
	int io_write(const typo_string &filename, const vector<T> &_dict) //ds_pair
	{
		//	typo_string S_FORMAT_SIM = (*textStringsIniFile)[TEXT_CONSTANTS][TXT_ID_00001];//IOManipulators.h  "\tsimilarity:\t"
		//	typo_string S_FORMAT_FIX = (*textStringsIniFile)[TEXT_CONSTANTS][TXT_ID_00002];//IOManipulators.h  "\tfix:\t"

		typo_string S_FORMAT_SIM = "\tsimilarity:\t";
		typo_string S_FORMAT_FIX = "\tfix:\t";

		ofstream os;
		os.open(filename.c_str());
		if (!os.good()){		
		//	errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		vector<T>::const_reverse_iterator it=_dict.rbegin();
		while (it != _dict.rend()){
			outfile << S_FORMAT_SIM << (*it).first << S_FORMAT_FIX << (*it).second << endl;
			it++;
		}
		os.close();
	}

	template<typename T>
	int io_write(ofstream &os, const vector<T> &v) //
	{
		typo_string word=S_EMPTY;
		vector<T>::const_iterator it = v.begin();  
		while (it != v.end()){
			os << *it << endl;
			it++;
		}
		return RET_FSUCCESS;
	}
	//	int io_write(const typo_string &filename,  map<typo_string, int> &m);

	template<typename T>
	int io_write(const typo_string &filename,  map<typo_string, T> &m, const typo_string &delim)
	{
		ofstream os;
		os.open(filename.c_str(), ios::app);
		if (!os.good()){			
			//errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		map<typo_string, T>::iterator it = m.begin();  
		for (it = m.begin(); it != m.end(); it++){
			os << (*it).first;  
			os << S_TAB;
			os << (*it).second;
			os << delim; 
		}
		os.close();
		return RET_FSUCCESS;
	}
//	template<typename T>


	template<typename T1, typename T2> //svs_pair
	int io_write(const typo_string &filename,  map<T1,T2> &m, const typo_string &delim)
	{
		ofstream os;
		os.open(filename.c_str(), ios::app);
		if (!os.good()){		
		//	errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		map<T1, T2>::iterator it = m.begin();  
		for (it = m.begin(); it != m.end(); it++){
			os << (*it).first; 
			os << delim; 
			io_write<typo_string>(os, (*it).second);
		}
		os.close();
		return RET_FSUCCESS;
	}

	template<typename T1, typename T2> //svs_pair
	int io_write(const typo_string &filename,  map<T1,T2> &m, const typo_string &delim, bool erase)
	{
		ofstream os;
		if (erase)
			os.open(filename.c_str(), ios::out);
		else 
			os.open(filename.c_str(), ios::app);

		if (!os.good()){		
		//	errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		map<T1, T2>::iterator it = m.begin();  
		for (it = m.begin(); it != m.end(); it++){
			os << (*it).first; 
			os << delim; 
			io_write<typo_string>(os, (*it).second);
		}
		os.close();
		return RET_FSUCCESS;
	}

	template<typename T>
	int io_write(const typo_string &filename,  T &m, const typo_string &delim)
	{
		ofstream os(filename.c_str());
		if (!os.good()){		
		//	errLogFile.report("00009", filename);		
			return RET_FFAILURE;	
		}
		T::iterator it = m.begin();  
		for (it = m.begin(); it != m.end(); it++){
			//os << (*it).first; 
			//os << delim; 
			//TBD !!!!!!!!!!!!
			//io_write<T2>(os, )
			//os << (*it).second;
		}
		os.close();
	}

	template<class T>
	int IOManipulators::io_write(const list<T> &L, const typo_string &delim){
		copy(L.begin(), L.end(), ostream_iterator<T>(cout, delim));
	}

	int IOManipulators::saveLettersMappings(const typo_string &filename,  
		map<typo_string, vector<si_pair>> &m, const typo_string &delim, bool erase);
	
	//	template<class T> 
	//		int io_write(const list<T> &L, const typo_string &delim);
};
#endif //__IOFUNCTIONS_H__