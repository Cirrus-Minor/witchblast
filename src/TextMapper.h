/**  This file is part of Witch Blast.
*
*  Witch Blast is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Witch Blast is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
-> TextMapper class :
brief: the job of TextWrapper is to load a 'text description file' and provide a correponding string for each key, related to a given language.
implementation: it's an old lazy evaluated singleton /!\ not thread safe /!\, with two helper functions (setLanguage and getLabel) to ease the use of this singleton.
initialization: the unique TextWrapper instance is initialized, by default, with the 'english' state. It cant be re-initialized at any moment by
calling the member or free function setLanguage()

-> 'text description file': it's basically a map of pairs "key"/"value", separated by a given separator, where "value" is the text that will be displayed in the game.
this file is separated in x parts: each part correspond to a language. Each part is defined by the markup [language], where "language" is one of the supported language.
*/

#ifndef __TEXTMAPPER
#define __TEXTMAPPER

#include <map>
#include <string>

namespace tools
{

	void setLanguage(const std::string & language);
	const std::string getLabel(const std::string & key);

	class TextMapper
	{
		typedef std::map<std::string, std::string> TextMap;

	public:
		// deal with unique instance
		static TextMapper& instance();

		void setLanguage(const std::string & language);
		inline const std::string getText(const std::string & key) const
		{ return textMap.find(key) != textMap.end() ? textMap.find(key)->second : "[" + key + "]" ; }

	private:
		TextMapper();								// hidden ctor
		TextMapper(TextMapper const&) = delete;		// don't implement
		void operator=(TextMapper const&) = delete;	// don't implement

		void LoadTextFile(const std::string & language);

		/* tools funcs */
		// std::isspace function fail with some chars (à,é,è, etc.)
		inline static bool my_isspace(char c) { return (c == ' ' || c == '\t'); }

		// trim from start
		inline static std::string ltrim(const std::string &s) {
			size_t i = 0;
			while (i != s.size() && my_isspace(s[i++]));
			return (i == s.size()) ? "" : s.substr(i - 1, s.size());
		}

		// trim from end
		inline static std::string rtrim(const std::string &s) {
			size_t i = s.size();
			while (i != 0 && my_isspace(s[--i]));
			return (i == 0) ? "" : s.substr(0, i + 1);
		}

		// trim both sides
		inline static std::string trim(const std::string &s){ return (ltrim(rtrim(s))); }

		// split using separator
		inline std::pair<std::string, std::string> split(const std::string & s) const {
			auto pos = s.find_first_of(separator);
			return std::make_pair(s.substr(0, pos), s.substr(pos + 1, s.size() - pos));
		}

	private:
		const char separator;
		const std::string textFileName;
		TextMap textMap;
	};


} // end namespace tools


#endif
