#include "IOManipulators.h"
#include "StringManipulators.h"
#include "IniFileHandler.h"
#include "LogConstants.h"
#include "Application.h"
#include "Log.h"
#include "ContainersManipulators.h"

#include <iterator>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace IniFileNamespace;

IOManipulators::IOManipulators()
{}

void IOManipulators::trim(typo_string& str)
{
	typo_string str_copy = str;
	typo_string letters_set = UC_ALPHABET_EN_LOWER; 
	int lstrip = str_copy.find_first_of(letters_set);
	int rstrip = str_copy.find_last_of(letters_set);
	if ((lstrip > 0) && (rstrip > 0) && (rstrip != str_copy.size()+1))
		str = str.substr(lstrip, rstrip-lstrip+1);
}


int IOManipulators::io_read(const typo_string &filename, map<typo_string, typo_long_num> &m)
{
	ifstream is;
	is.open(filename.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	while (!is.eof()){ //add as parameter && ((int)m.size() < MaxNGramSize)){
		typo_string what; typo_long_num attr;
		is >> what; is >> attr;
		m.insert(m.begin(), pair<typo_string, typo_long_num>(what, attr));
	}
	is.close();
	return RET_FSUCCESS;
}
int IOManipulators::io_read(const typo_string &filename, map<sss_pair, typo_long_num> &m)
{
	ifstream is;
	is.open(filename.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	while (!is.eof()){ //add as parameter && ((int)m.size() < MaxNGramSize)){
		sss_pair next; typo_long_num freq;
		is >> next.first.first; is >> next.first.second; is >> next.second; is >> freq;
		m.insert(m.begin(), pair<sss_pair, typo_long_num>(next, freq));
	}
	is.close();
	return RET_FSUCCESS;
}

//synonymsMap expected format:  
//a1; a2; a3
//b1; b2; b3
int IOManipulators::io_read_synonymsMap(const typo_string &filename, synonymsDict &m) //map<typo_string,  int(index)>
{
	ifstream is;
	is.open(filename.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}
	
	int index=0;
	typo_string synonym;  
	si_pair synonym_pair; 
	is >> synonym; 
	trim(synonym); //remove BOM
	synonym_pair.first = synonym;
	synonym_pair.second = index;
	m.insert(synonym_pair);
	
	while (!is.eof())
	{ 
		char line[1024];
		is.getline(line, 1024);
		typo_string ending=S_SEMICOLON, new_word= S_EMPTY;
		stringstream ll(line);
		synonym = S_EMPTY;
		while (1){
			ll >> new_word;
			if (new_word == S_EMPTY)
				break;
			ending = new_word.substr(new_word.size()-1, 1);
			if (ending == S_SEMICOLON){
				if (synonym == S_EMPTY)
					m.insert(si_pair(new_word.substr(0, new_word.size()-1), index));
				else {
					synonym += C_SPACE;
					m.insert(si_pair(synonym + new_word.substr(0, new_word.size()-1), index));
					synonym = S_EMPTY;
				}
			}
			else {
				if (synonym.size()>0)
					synonym += C_SPACE;
				synonym+= new_word;
			}
			new_word= S_EMPTY;
		}
		m.insert(si_pair(synonym, index));
		index++;
	}

	is.close();
	return RET_FSUCCESS;
}

int IOManipulators::io_read(const typo_string &filename,  vector<typo_string> &vs)
{
	ifstream is;
	is.open(filename.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	while (!is.eof()){ //TODO add MaxDictSize parameter:  && ((int)(vs.size()) < MaxDictSize)){
		typo_string word; is >> word;
		vs.insert(vs.begin(), word);
	}
	is.close();
	return RET_FSUCCESS;
}

int IOManipulators::io_read(const typo_string &filename, typo_string &content )
{
	ifstream is;
	is.open(filename.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	typo_string s((istreambuf_iterator<typo_char>(is)), istreambuf_iterator<typo_char>());
	content = s;
	is.close();
	return RET_FSUCCESS;
}

int IOManipulators::io_read(const typo_string &_lmpath, map<typo_string, vector<typo_string>> &vs)
{
	ifstream is;
	is.open(_lmpath.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	typo_char buffer[LINE_SIZE];
	typo_string to_fix, sfixes, line;
	typo_long_num freq = D_ZERO;
	StringManipulators sm;

	while (!is.eof()){////TODO add MaxDictSize parameter: && ((int)vs.size() < MaxDictSize)){
		to_fix = S_EMPTY; sfixes=S_EMPTY;
		vector<typo_string> fixes;
		is.getline (buffer, LINE_SIZE);
		line = buffer;
		sm.getLeft(to_fix, line);
		sm.getRight(sfixes, line);
		if ((to_fix == S_EMPTY) || (sfixes == S_EMPTY))
			continue;
		sm.parseStringAppendToVector(sfixes, C_COMA, fixes);
		vs.insert(svs_pair(to_fix, fixes));	
	}
	is.close();
	return RET_FSUCCESS;
}

int IOManipulators::io_read(const typo_string &_lmpath,  map<typo_string, vector<si_pair>> &vs)
{
	ifstream is;
	is.open(_lmpath.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}

	typo_char buffer[LINE_SIZE];
	typo_string to_fix, sfixes, line;
	typo_long_num freq = D_ZERO;
	StringManipulators sm;

	while (!is.eof()){////TODO add MaxDictSize parameter: && ((int)vs.size() < MaxDictSize)){
		to_fix = S_EMPTY; sfixes=S_EMPTY;
		vector<si_pair> fixes;
		is.getline (buffer, LINE_SIZE);
		line = buffer;
		sm.getLeft(to_fix, line);
		sm.getRight(sfixes, line);
		if ((to_fix == S_EMPTY) || (sfixes == S_EMPTY))
			continue;
		sm.parseStringAppendToVector(sfixes, C_COMA, fixes);
		vs.insert(svpsi_pair(to_fix, fixes));	
	}
	is.close();
	return RET_FSUCCESS;
}

int IOManipulators::saveLettersMappings(const typo_string &filename,  
		map<typo_string, vector<si_pair>> &m, const typo_string &delim, bool erase)
	{
		ofstream os;

		if (erase)
			os.open(filename.c_str(), ios::out);
		else 
			os.open(filename.c_str(), ios::app);

		if (!os.good()){	
		
			return RET_FFAILURE;	
		}
		map<typo_string, vector<si_pair>>::iterator it = m.begin();  
		for (it = m.begin(); it != m.end(); it++){
			os << (*it).first;  
			os << S_SPACE;
			vector<si_pair> vv = (*it).second;
			vector<si_pair>::iterator it = vv.begin();
			for (; it != vv.end(); it++)
				os << (*it).first << C_COMA << (*it).second << C_SEMICOLON;
			os << delim; 
		}
		os.close();
		return RET_FSUCCESS;
	}

bool ReadFile::get_next(typo_string& next)
{
	return true;
}

/*
input: ecspresif proces litereche aksplorertin filosofi aspasheli welyiu akspireans artikol ebaut
output format: o="offset" l="word's length" s="suggestions"
<c o="0" l="9" s="1">ekspresif expressive express expressing expires expressed</c>

<?xml version="1.0" encoding="UTF-8"?>
<spellresult error="0" clipped="0" charschecked="91">
<c o="0" l="9" s="1">ekspresif expressive express expressing expires expressed</c>
<c o="10" l="6" s="0">prices process proses Price's price's</c>
<c o="17" l="9" s="0">literacy literate litterer loiterer litre</c>
<c o="27" l="12" s="0">exploration exploring accelerating exploiting explored</c>
<c o="40" l="8" s="0">Flossi falsify falsifier Folsom Flossie</c>
<c o="49" l="9" s="0">Aspell apishly waspishly Ispell spacial</c>
<c o="59" l="6" s="0">Weylin Vilyui wellie Welbie welly</c>
<c o="66" l="10" s="0">aspirins aspirin's Shakespeareans expires Esperanza</c>
<c o="77" l="7" s="1">article atrial articular atoll Utrillo</c>
<c o="85" l="5" s="1">about abut beaut Ebert beauty</c>
</spellresult>
*/
void XmlOutput::update(const typo_string &to_fix, const vector <ds_pair> &cand_list)
{
	ofs << "<c o=\"" << _offset << "\" l=\"" << to_fix.size() << "\" s=\"0\">";
	_offset += to_fix.size() + 1;

	//ofs << to_fix << " " ;

	vector<ds_pair>::const_iterator it=cand_list.begin(); 	
	for(it=cand_list.begin(); it != cand_list.end(); ++it){
		ofs << (*it).second << " " ;		
	}

	ofs << "</c>";
	//ofs << endl;
//	long pos=ofs.tellp();
//	ofs.seekp (pos-2);
//	ofs << S_NL;

//	ofs << XML_CORRECTIONS_END << endl;
}
		

/*
void AspellOutput::update(const typo_string &to_fix, const vector <ds_pair> &cand_list)
{
//& absorbbtion 7 0: absorption, absolution, absorbing, abortion, absorptions, observation, adsorption
	ofs << C_AT << C_SPACE << to_fix << C_SPACE << cand_list.size() << C_SPACE << C_ZERO << C_COLON << C_SPACE;
	//copy(cand_list.begin(), cand_list.end(), ostream_iterator<ds_pair>(outfile, ", "));
	vector<ds_pair>::const_iterator it=cand_list.begin(); 	//	emission_probability["appendicularia"]["abundancies"] = 0.92; 
	for(it=cand_list.begin(); it != cand_list.end(); ++it){
		ofs << (*it).second << C_COMA << C_SPACE;
	}
	long pos=outfile.tellp();
	ofs.seekp (pos-2);
	ofs << S_NL;
}

int AspellOutput::fileCopy(const typo_string& from, const typo_string& to)
{
	typo_string command = "copy ";
	command += from; command += C_SPACE; command += to;
	#ifdef _WIN32
		system(command.c_str());
		return RET_FSUCCESS;
	#endif  //end of make a copy of algorithms.ini 
	
	return RET_FFAILURE;
}
*/
/*
int WriteFile::printFile(const string& path)
{
	ostream file(path.c_str());	
	if (false == file.good())
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00014, _tsb.outPath); 	
		return RET_FFAILURE;
	}
	ostream_iterator< std::typo_string > out_iter( std::cout, "\n" );
	copy( file.begin(), file.end(), out_iter);
	file.close();
	return RET_FSUCCESS;
	
}*/


/*
mind closing file before return : fin.cl ose();
bool ReadFile::read_utf8_file(const typo_string& pathname, std::wstring &contents, 
									bool b_rn2n) // convert "\r\n"->"\n"
{
    std::ifstream fin;
	fin.op en(pathname.c_str(), std::ios::binary);
    if (!fin)
        return false;

    BYTE bom[4];
    bool bUTF8BOM = false;
    if (!fin.read((typo_char*)bom, 4))
    {
        // if we hit eof then file is less than 4 bytes
		if (!fin.eof()){
			fin.cl ose();
            return false;
		}
        fin.clear();
    }//if
    else
    {
        bUTF8BOM = (bom[0] == 0xEF) && (bom[1] == 0xBB) && (bom[2] == 0xBF);
        // TODO - check for non-UTF8 BOM's
        // UTF8 BOM is 3 bytes, go back 1 byte if needed
        if (bUTF8BOM)
            fin.seekg(-1, std::ios::cur);
    }//else

    if (!bUTF8BOM)
        fin.seekg(0, std::ios::beg); // setup to re-read first 3 bytes

    std::stringstream ss;
	if (!(ss << fin.rdbuf())){ // note: will fail if file is empty
		fin.cl ose();
        return false;
	}
    fin.clo se(); // release resources

    const std::string &utf8 = ss.str();
    ss.str(""); // release resources

    // NOTE: we are assuming that the number of bytes in a UTF8 string is 
    //       always >= to the number of wchar_t's required to represent that 
    //       string in UTF16LE - which should hold true
    size_t len = utf8.length();
    std::vector<wchar_t> wbuff(len);
    int wlen = MultiByteToWideChar(CP_UTF8, 0,
                                   utf8.c_str(), len, 
                                   &wbuff[0], len);
    if (!wlen)
        return false;
    wbuff.resize(wlen); // wlen may be < len

    if (b_rn2n)
        contents.assign(wbuff.begin(), 
                        std::remove(wbuff.begin(), wbuff.end(), '\r'));
    else
        contents.assign(wbuff.begin(), wbuff.end());
    return true;
}//read_utf8_file
*/
