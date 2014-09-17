/**  This file is part of sfmlGame.
  *
  *  FreeTumble is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  FreeTumble is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with FreeTumble.  If not, see <http://www.gnu.org/licenses/>.
  */

#include "ImageManager.h"
#include <iostream>

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
    std::cout << "Releasing video memory... ";
    for (unsigned int i = 0; i < imageArray.size(); i++)
    {
        delete(imageArray[i]);
    }
    imageArray.clear();
    std::cout << "OK" << std::endl;
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager singleton;
    return singleton;
}

void ImageManager::addImage(const char* fileName)
{
    sf::Texture* newImage = new sf::Texture;
    newImage->loadFromFile(fileName);
    imageArray.push_back(newImage);
}

bool ImageManager::reloadImage(int n, const char* fileName)
{
    sf::Texture* newImage = new sf::Texture;
    bool result = newImage->loadFromFile(fileName);
    imageArray[n] = newImage;
    return result;
}

sf::Texture* ImageManager::getImage(int n)
{
    return imageArray[n];
}
