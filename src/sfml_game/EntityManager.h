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

#ifndef __ENTITYMANAGER
#define __ENTITYMANAGER

#include <list>
class GameEntity;
#include "GameEntity.h"

class EntityManager
{

public:
  static EntityManager& getInstance();

  void initIterator();
	void add(GameEntity* g);
	void animate (float delay);
	void render (sf::RenderTarget* app);
	void renderAfter (sf::RenderTarget* app, int n);
	void renderUnder (sf::RenderTarget* app, int n);
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
