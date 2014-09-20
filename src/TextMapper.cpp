#include "TextMapper.h"

#include <fstream>
#include <iostream>

namespace tools
{
	void setLanguage(const std::string & language)
	{
		TextMapper::instance().setLanguage(language);
	}

	const std::string getLabel(const std::string & key)
	{
		return TextMapper::instance().getText(key);
	}

	TextMapper::TextMapper()
		: separator('#')
		, textFileName("labels.txt")
	{
		LoadTextFile("english");
	}

	TextMapper& TextMapper::instance()
	{
		static TextMapper instance;
		return instance;
	}

	void TextMapper::setLanguage(const std::string & language)
	{
		textMap.clear(); // clear old map
		LoadTextFile(language); // load new text map
	}

	void TextMapper::LoadTextFile(const std::string & language)
	{
		// 1. open file
		std::ifstream textFile("data/" + textFileName);
		if (!textFile)
			std::cout << "[ERROR] No text file !";

		// 2. find language section
		std::string line;
		bool found = false;
		std::string token = "[" + language + "]";
		while (!found && std::getline(textFile, line))
			found = (trim(line) == token);

		if (!found)
		{
			std::string errMsg = "[ERROR] Language '" + language + "' NOT FOUND !";
			std::cout << errMsg.c_str();
		}

		// 3. load map
		bool mustStop = false;
		while (std::getline(textFile, line) && !mustStop)
		{
			std::string trimmed = trim(line);
			if (trimmed[0] == '[')
				mustStop = true;
			else if (!trimmed.empty())
				textMap.insert(split(trimmed));
		}
	}

} // end namespace tools
