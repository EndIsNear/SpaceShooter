#ifndef __MFILE_H__
#define __MFILE_H__

#include "cocos2d.h"
#include "..\cocos2d\\external\json\\document.h"
#include <sstream>
#include "fstream"

typedef rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> DocumentEntry;

template <typename Entry>
class FileReader
{
public:
	FileReader(std::string rFileName)
	{
		std::ifstream inputFile;

		// Open file
		inputFile.open(cocos2d::FileUtils::getInstance()->sharedFileUtils()->fullPathForFilename(rFileName).c_str());

		//Read whole file
		inputFile.seekg(0, std::ios::end);
		m_Json.reserve(inputFile.tellg());
		inputFile.seekg(0, std::ios::beg);

		m_Json.assign((std::istreambuf_iterator<char>(inputFile)),
			std::istreambuf_iterator<char>());

		inputFile.close();
		m_Document.Parse<0>(m_Json.c_str());
	};


	size_t GetEntryCount()
	{
		if (m_Document.HasMember("EntryCount"))
			return m_Document["EntryCount"].GetInt();
	}

	bool GetEntryByIndex(size_t idx, Entry& rEntry)
	{
		std::stringstream stream;
		stream << "Entry" <<  idx;
		std::string str;
		stream >> str;
		rEntry.Load(m_Document[str.c_str()]);
		return true;
	}

	~FileReader() 
	{
	}
protected:
	FileReader(const FileReader&);
	FileReader& operator=(const FileReader&);
	
protected:
	std::string m_Json;
	rapidjson::Document m_Document;
};


#endif // __MFILE_H__