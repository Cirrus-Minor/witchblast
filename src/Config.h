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

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <map>
#include "Constants.h"

class Config
{
public:
  Config();
  void loadFromFile(std::string file);
  void saveToFile(std::string file, std::map<std::string, std::string> newMap);
  void displayMap();

  int findInt(std::string key);
  std::string findString(std::string key);
  bool configFileExists();

private:
  std::map<std::string, std::string> configMap;
  bool configFileExistsFlag;
};

#endif // CONFIG_H_INCLUDED
