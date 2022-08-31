#ifndef _INIPARSER_H_
#define _INIPARSER_H_

/*	
 * Loads and handles an .ini file put into a map, using a singleton pattern.
 * @author Michael Jordan
 * @year 2018
 */
 
//Library includes:
#include <string>
#include <map>
#include <vector>

#define IniMap std::map<std::string, std::string>
#define ExtendedMap std::map<std::string, std::vector<std::string>*>

class IniParser
{
	//Member methods:
public:
	static IniParser& GetInstance();
	static void DestroyInstance();
	~IniParser();
	
	bool CreateIniFile(const std::string& _fileLoc);
	bool SaveIniFile();
	bool LoadIniFile(const std::string& _fileLoc);

	void AddNewValue(const std::string& iniSection, const std::string& key, const std::string& value);
	void SetValueAsString(const std::string& iniSection, const std::string& key, const std::string& value);
	void SetValueAsBoolean(const std::string& iniSection, const std::string& key, const bool& value);

	std::string GetValueAsString(const std::string& iniSection, const std::string& key);
	int GetValueAsInt(const std::string& iniSection, const std::string& key);
	float GetValueAsFloat(const std::string& iniSection, const std::string& key);
	bool GetValueAsBoolean(const std::string& iniSection, const std::string& key);
	bool ContainsSection(const std::string& iniSection);

protected:

private:
	//Only one instance at all times.
	IniParser();
	IniParser(const IniParser& iniParser);
	IniParser& operator=(const IniParser& iniParser);

	//Member data:
public:

protected:
	static IniParser* sm_pInstance;
	
	IniMap* m_Content;
	std::string filename;
private:

};
#endif // !_INIPARSER_H_