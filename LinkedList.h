#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

#define START 0
#define DEFAULT_NODE nullptr
#define DEFAULT_TILE nullptr

class LinkedList {
   public:
      LinkedList();
      LinkedList(LinkedList& otherLinkedList);
      ~LinkedList();
      void addTile(std::shared_ptr<Tile> tile);
      void removeTile(int index);
      std::shared_ptr<Tile> getTile(int index) const;
      std::shared_ptr<Tile> search(std::string tileValue);
      int getSize();

   private:
      std::shared_ptr<Node> headNode;
      int size;
};

#endif // ASSIGN2_LINKEDLIST_H