/*
 * Loads and handles an .ini file put into a map, using a singleton pattern.
 * @author Michael Jordan
 * @year 2018
 */

//Local includes
#include "IniParser.h"

//Library includes
#include <string>
#include <fstream>
#include <sstream>
#include <map>

//Static instances:
IniParser* IniParser::sm_pInstance = 0;
std::map<std::string, std::string>* m_Content = 0;

/* Calls the constructor of the Iniparser if it doesn't exist.
 *
 * @access	public
 * @returns	IniParser& 		Reference to the singleton Ini Parser.
 *
 */
IniParser&
IniParser::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new IniParser();
	}

	return (*sm_pInstance);
}

/* Calls the deconstructor of the instance.
 *
 * @access	public
 * @returns	N/A
 *
 */
void
IniParser::DestroyInstance()
{
	if(sm_pInstance != 0) 
		delete sm_pInstance;
}

/* Nullifies the map.
 *
 * @access	private
 * @returns	N/A
 *
 */
IniParser::IniParser()
	: m_Content(0)
	, filename("")
{
	
}

/* Deconstructs the map pointer.
 *
 * @access	public
 * @returns	N/A
 *
 */
IniParser::~IniParser()
{
	if (m_Content != 0)
	{
		m_Content->clear();
		delete m_Content;
	}
}

bool IniParser::CreateIniFile(const std::string& _fileLoc)
{
	if (_fileLoc.find(".ini") != -1) //File is of type .ini
	{
		filename = _fileLoc;

		if (m_Content == 0)
		{//Initialise map
			m_Content = new IniMap();
		}
		else
		{
			m_Content->clear();
		}

		std::ofstream file(_fileLoc); //creates file
		return true; //File now exists
	}
	return false; //File of wrong type
}

bool IniParser::SaveIniFile()
{
	std::ofstream file;
	file.open(filename);
	if (file.is_open())
	{
		ExtendedMap collect; 
		IniMap::iterator it;
		ExtendedMap::iterator collect_it;
		
		//Seperate and store values
		for (it = m_Content->begin(); it != m_Content->end(); ++it)
		{//for each
			it->first; //"section | key"
			it->second; //value
			int temp = it->first.find("|");
			std::string section = it->first.substr(0, temp-1); //before |
			std::string key = it->first.substr(temp + 2, it->first.size() - 1); //after |
			std::string val = key + "=" + it->second;
			collect_it = collect.find(section);
			if (collect_it == collect.end())
			{
				//create vector for section
				collect.insert(std::pair<std::string, std::vector<std::string>*>(section, new std::vector<std::string>()));
				collect_it = collect.find(section);
				collect_it->second->push_back(val);
			}
			else
			{	//add to section
				collect_it->second->push_back(val);
			}
		}
		
		for (collect_it = collect.begin(); collect_it != collect.end(); ++collect_it)
		{
			file << "[" +collect_it->first + "]\n";
			std::vector<std::string>::iterator v_it;
			for (v_it = collect_it->second->begin(); v_it != collect_it->second->end(); ++v_it)
			{
				file << *v_it + "\n";
			}
			file << "\n";
			delete (*collect_it).second; //Clear Data
		}
		collect.clear();
		file.close();
		return true;
	}
	return false; //File not found
}

/* Clears the map then loads it up with the data in the .ini file specified.
 *
 * @access	public
 * @param 	string 		(String) File location of the .ini file to open.
 * @returns	(boolean) 	the file was found.
 *
 */
bool 
IniParser::LoadIniFile(const std::string& _fileLoc)
{
	if (filename == _fileLoc)
	{
		return true;
	}
	else
	{
		std::ifstream file(_fileLoc);
		std::string line;
		std::string section;
		this->filename = _fileLoc;
		if (file.is_open()) //Check if the file exists
		{
			if (m_Content == 0)
			{
				m_Content = new std::map<std::string, std::string>();
			}
			else
			{
				m_Content->clear();
			}

			while (std::getline(file, line))
			{
				if (line.empty())
				{
					//Ignore because it is empty.
				}
				else if (line.at(0) == '[')
				{
					section = line.substr(1, line.size() - 2);
				}
				else if (line.at(0) == ';')
				{
					//Ignore because it is a comment.
				}
				else
				{
					int temp = static_cast<int>(line.find("="));
					std::string key = line.substr(0, temp);
					std::string value = line.substr(temp + 1, line.size() - 1);
					m_Content->insert(std::pair<std::string, std::string>(section + " | " + key, value));
				}
			}
			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}
}

void IniParser::AddNewValue(const std::string& iniSection, const std::string& key, const std::string& value)
{
	if (m_Content != 0)
	{
		std::string insert = iniSection + " | " + key;
		m_Content->insert(std::pair<std::string, std::string>(insert, value));
	}
}

void IniParser::SetValueAsString(const std::string& iniSection, const std::string& key, const std::string& value)
{
	if (m_Content != 0)
	{
		std::string insert = iniSection + " | " + key;
		IniMap::iterator it = m_Content->find(insert);
		it->second = value;
	}
}

void IniParser::SetValueAsBoolean(const std::string& iniSection, const std::string& key, const bool& value)
{
	if (value)
	{
		SetValueAsString(iniSection, key, "true");
	}
	else
	{
		SetValueAsString(iniSection, key, "false");
	}
}

/* Gets the value from the map using the specified section/key combination.
 *
 * @access	public
 * @param 	iniSection 	(String) Section name reference, in the .ini file.
 * @param 	key 		(String) Key name reference, in the .ini file.
 * @returns	(string) Value as an string.
 *
 * @throws 	OutOfRange	Thrown if the combination of section and key don't exist in the map.
 */
std::string
IniParser::GetValueAsString(const std::string& iniSection, const std::string& key)
{
	return m_Content->at(iniSection + " | " + key);
}

/* Gets the value, and converts it into an integer, from the map using the specified section/key combination.
 *
 * @access	public
 * @param 	iniSection 	(String) Section name reference, in the .ini file.
 * @param 	key 		(String) Key name reference, in the .ini file.
 * @returns	(int) Value as an integer.
 *
 * @throws 	OutOfRange	Thrown if the combination of section and key don't exist in the map.
 */
int
IniParser::GetValueAsInt(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	std::stringstream geek(temp); //Convert String -> Integer
	int val = 0;
	geek >> val;
	return val;
}

/* Gets the value, and converts it into a float, from the map using the specified section/key combination.
 *
 * @access	public
 * @param 	iniSection 	(String) Section name reference, in the .ini file.
 * @param 	key 		(String) Key name reference, in the .ini file.
 * @returns	(float) Value as an float.
 *
 * @throws 	OutOfRange	Thrown if the combination of section and key don't exist in the map.
 */
float
IniParser::GetValueAsFloat(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	return strtof(temp.data(), NULL); //Convert String -> Float
}

/* Gets the value, and converts it into a boolean, from the map using the specified section/key combination.
 *
 * @access	public
 * @param 	iniSection 	(String) Section name reference, in the .ini file.
 * @param 	key 		(String) Key name reference, in the .ini file.
 * @returns	(boolean) Value as an boolean.
 *
 * @throws 	OutOfRange	Thrown if the combination of section and key don't exist in the map.
 */
bool
IniParser::GetValueAsBoolean(const std::string& iniSection, const std::string& key)
{
	std::string temp = m_Content->at(iniSection + " | " + key);
	//Manual Convert String -> Boolean
	if (temp == "true") 
	{
		return true;
	}
	else if(temp == "false")
	{
		return false;
	}
	return false;
}

bool IniParser::ContainsSection(const std::string& iniSection)
{
	IniMap::iterator it;

	//Seperate and store values
	for (it = m_Content->begin(); it != m_Content->end(); ++it)
	{
		std::string section = (*it).first.substr(0, (*it).first.find_first_of("|") - 1);
		if (iniSection == section)
		{
			return true;
		}
	}
	return false;
}