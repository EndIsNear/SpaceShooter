#ifndef __MFILE_H__
#define __MFILE_H__

#include "cocos2d.h"
#include "external\json\document.h"
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
		inputFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename(rFileName).c_str());

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
		return m_Document["Entries"].Size();
	}

	bool GetEntryByIndex(size_t idx, Entry& rEntry)
	{
		rEntry.Load(m_Document["Entries"][idx]);
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

typedef rapidjson::GenericValue<rapidjson::UTF8<> > FileEntryForRead;

#endif // __MFILE_H__