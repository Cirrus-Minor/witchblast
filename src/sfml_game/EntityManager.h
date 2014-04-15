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

#ifndef __ENTITYMANAGER
#define __ENTITYMANAGER

#include <list>
class GameEntity;
#include "GameEntity.h"

class EntityManager
{

public:
  static EntityManager* getEntityManager();

  void initIterator();
	void add(GameEntity* g);
	void animate (float delay);
	void render (sf::RenderWindow* app);
	void onEvent(sf::Event event);
	void displayToConsole();

	typedef std::list<GameEntity*> EntityList;

	EntityList* getList();
	void clean();
	void partialClean(int n);
  void sortByZ();

protected:
	EntityManager();
	~EntityManager() { clean(); };      // TODO !

	GameEntity* nextItem();


	bool isEmpty() { return mItems == 0; };
	int count() { return mItems; }

	void saveIterator() { mSavedIterator = mIterator; }
	void restoreIterator() { mIterator = mSavedIterator; }


private:

	EntityList entityList;
	EntityList::iterator mIterator, mSavedIterator;
	int mItems;

};

#endif
