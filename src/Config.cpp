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

#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <iostream>

#include "Config.h"

Config::Config()
{
  loadFromFile(CONFIG_FILE);
}

bool Config::configFileExists()
{
  return configFileExistsFlag;
}

void Config::loadFromFile(std::string file)
{
  std::ifstream f(file.c_str());
  if (!f.is_open())
  {
    configFileExistsFlag = false;
  }
  else
  {
    configFileExistsFlag = true;
    std::string key;

    while (f >> key)
    {
      std::string data;
      if (f >> data)
      {
        configMap[key] = data;
      }
    }

    f.close();
  }
}

void Config::saveToFile(std::string fileName, std::map<std::string, std::string> newMap)
{
  std::ofstream file(fileName.c_str(), std::ios::out | std::ios::trunc);
  if (file)
  {
    configFileExistsFlag = true;
    std::map<std::string, std::string>::iterator it;

    for(it = newMap.begin(); it != newMap.end(); it++)
    {
      file << it->first << " " << it->second << std::endl;
    }
    file.close();
  }
}

void Config::displayMap()
{
  std::map<std::string, std::string>::const_iterator
  mit (configMap.begin()),
      mend(configMap.end());
  for(; mit!=mend; ++mit)
  {
    std::cout << "\"" << mit->first << "\"" << '\t' << mit->second << std::endl;
  }
}

int Config::findInt(std::string key)
{
  std::map<std::string, std::string>::const_iterator
  mit(configMap.find(key)),
      mend(configMap.end());
  if(mit!=mend)
  {
    int value = atoi(mit->second.c_str());
    return value;
  }
  else
  {
    return -99999;
  }

  return atoi(mit->second.c_str());
}

std::string Config::findString(std::string key)
{
  std::map<std::string, std::string>::const_iterator
  mit(configMap.find(key)),
      mend(configMap.end());
  std::string result = "";
  if(mit!=mend)
    result = mit->second.c_str();

  return result;
}
