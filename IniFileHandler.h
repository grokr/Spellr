#ifndef __INIFILEHANDLER_H__
#define __INIFILEHANDLER_H__

#include "IniFileHandlerConstants.h"
#include "IniFileConstants.h"
#include "Constants.h"
#include <fstream>
//#include <exception>
#include <stdexcept>


#include "LogConstants.h"
//taken from: http://www.gamedev.net/community/forums/topic.asp?topic_id=405783
//one more sample: http://www.codeproject.com/KB/files/Sample_INI_File_in_C.aspx

namespace IniFileNamespace{

	template <typename Traits> class Collection;



	struct KeyTraits {
		typedef typo_string key_t;
		typedef typo_string value_t;
		typedef pair<key_t, value_t> item_t;
		static const char* begin_text() { return S_EMPTY.c_str(); }
		static const char* separator_text() { return S_SP_EQ_SP.c_str(); }
		static const char* end_text() { return S_NL.c_str(); }

	};

	typedef Collection<KeyTraits> INISection;

	struct SectionTraits {
		typedef typo_string key_t;
		typedef INISection value_t;
		typedef pair<key_t, value_t> item_t;
		static const char* begin_text() { return S_SQUARE_BRACKET.c_str(); }
		static const char* separator_text() { return S_BSQUARE_BRACKET_NL.c_str(); }
		static const char* end_text() { return S_NL.c_str(); }
	};

	template <typename Traits>
	class Collection {
	public:
		typedef typename Traits::key_t key_t;
		typedef typename Traits::value_t value_t;
		typedef map<key_t, value_t> storage_t;
		typedef typename storage_t::iterator iterator;
		typedef typename storage_t::const_iterator const_iterator;


	private:
		static value_t empty;
		storage_t data;

	public:

		const value_t& operator[](const key_t& key) const {
			const_iterator it = data.find(key);
			const value_t& result = (it == data.end()) ? empty : it->second;
			return result;
		}

		value_t& operator[](const key_t& key) {
			value_t& result = data[key];
			return data[key];
		}

	 void assign( const typo_string& current_section, const typo_string& key, 
			const typo_string& value) 
		{
			data[current_section][key] = value;
		}

		friend ostream& operator<<(ostream& os, const Collection<Traits>& d) {
			for (const_iterator it = d.data.begin(); it != d.data.end(); ++it) {
				os << Traits::begin_text() << it->first
					<< Traits::separator_text() << it->second
					<< Traits::end_text();
			}
			return os;
		}
	};


	template<typename Traits>
	typename Collection<Traits>::value_t Collection<Traits>::empty = typename Collection<Traits>::value_t();

	typedef Collection<SectionTraits> INIBase;

	class IniFile : public INIBase
	{
		typo_string filename;

	public:
		bool writeback;

		static void trim(typo_string& to_trim, _TCHAR ch) {
			int lstrip = to_trim.find_first_of(ch);
			if (lstrip < 1) return;
			int rstrip = to_trim.find_last_not_of(S_WHITESPACE);
			if (lstrip == typo_string::npos || rstrip == typo_string::npos) {
				to_trim = S_EMPTY;
			} else {
				to_trim = to_trim.substr(lstrip, rstrip + 1);
			}
		}

		static void trim(typo_string& to_trim) {
			int lstrip = to_trim.find_first_not_of(S_WHITESPACE);
			int rstrip = to_trim.find_last_not_of(S_WHITESPACE);
			if (lstrip == typo_string::npos || rstrip == typo_string::npos) {
				to_trim = S_EMPTY;
			} else {
				to_trim = to_trim.substr(lstrip, rstrip + 1);
			}
		}

		static INIBase createBase(const typo_string& filename) {

			// We construct with exists = false if we want to make a new INI file.
			ifstream input;
			try{
				INIBase tmp;
				if (filename == S_EMPTY) return tmp;

				typo_string current_line;
				typo_string current_section = S_EMPTY;

				input.open(filename.c_str());
				if (!input.is_open()) {
					//cout << filename << endl;
					throw MSG_00031;
				}

				while (getline(input, current_line)) {

					// Trim beginning of unicode file
					trim(current_line, C_SQUARE_BRACKET);

					// Trim whitespace from line
					trim(current_line);
					// Skip empty lines
					if (current_line.empty()) { continue; }
					int last_index = current_line.length() - 1;
					// Make sure this is clearly either a section title or key=value pair.
					if (current_line[0] == C_SQUARE_BRACKET && current_line[last_index] == C_BSQUARE_BRACKET) {
						if (current_line.find_first_of(S_BSQUARE_BRACKETS, 1) != last_index) {
							throw runtime_error(MSG_00032);
						}

						current_section = current_line.substr(1, last_index - 1);
						trim(current_section);
						if (current_section == S_EMPTY) {
							throw runtime_error(MSG_00033);
						}
					} else {
						// This should be a key=value pair. Make sure some section is "open".
						if (current_section == S_EMPTY) {
							throw runtime_error(MSG_00034);
						}
						// Find the '=' and make sure there's only one.
						int pos = current_line.find(C_SP_EQ_SP);
						if (pos == typo_string::npos) {
							throw runtime_error(MSG_00035);
						}
						if (pos != current_line.rfind(C_SP_EQ_SP)) {
							throw runtime_error(MSG_00036);
						}
						// Split up key and value and do the insertion
						typo_string key(current_line, 0, pos); trim(key);
						typo_string value(current_line, pos + 1); trim(value);
						tmp[current_section][key] = value;
					}
				}
				return tmp;
			}
			catch(const typo_string& msg)
			{
				if (msg != MSG_00031){
					input.close();
				}
				throw msg;
			}
		}

	

		IniFile(const IniFile& copy):filename(copy.filename), writeback(copy.writeback){}

		IniFile(const typo_string& filename=S_EMPTY, 
			bool writeback = true, bool exists = true):
		filename(filename), writeback(writeback), INIBase(exists ?	createBase(filename) : INIBase()){}

		~IniFile() {
			if (writeback) {
				write_to_file();
			}
		}
		//	static INIBase createBase(const typo_string& filename);

		//		static void trim(typo_string& to_trim);
		//		static void trim(typo_string& to_trim, _TCHAR ch);

		void write_to_file() {
			ofstream output;
			output.open(filename.c_str());
			output << *this;
			output.close();
		}

	};

};
#endif //__INIFILEHANDLER_H__