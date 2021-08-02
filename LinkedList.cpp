#include "LinkedList.h"

// Default Constructor of LinkedList.
LinkedList::LinkedList() {
   this->headNode = DEFAULT_NODE;
   this->size = START;
}

// Deep Copy Constructor of otherLinkedList.
LinkedList::LinkedList(LinkedList& otherLinkedList) {
   for (int i = START; i < otherLinkedList.getSize(); ++i) {
      std::shared_ptr<Tile> newTile =
            std::make_shared<Tile>(*otherLinkedList.getTile(i));
            
      this->addTile(newTile);
   }
}

// Return size of LinkedList.
int LinkedList::getSize() {
   return this->size;
}

// Deconstructor of LinkedList.
LinkedList::~LinkedList() {
   // TODO
}

// Add new Tile to back of LinkedList.
void LinkedList::addTile(std::shared_ptr<Tile> tile) {
   std::shared_ptr<Node> newNode = 
      std::make_shared<Node>(tile, DEFAULT_NODE);

   // Check if LinkedList is empty or not.
   if (this->headNode == DEFAULT_NODE) {
      this->headNode = newNode;
   } else {
      std::shared_ptr<Node> currNode = this->headNode;

      // Get last Node in LinkedList.
      while(currNode->getNext() != DEFAULT_NODE) {
         currNode = currNode->getNext();
      }

      // Add new Node to end of LinkedList.
      currNode->setNext(newNode);
   }

   ++this->size;
}

// Remove Tile in LinkedList by index while maintaining order.
void LinkedList::removeTile(int index) {
   if (index >= START && index < this->size) {
      if (this->headNode != DEFAULT_NODE) {
         int counter = START;
         std::shared_ptr<Node> currNode = this->headNode;
         std::shared_ptr<Node> prevNode = DEFAULT_NODE;

         // Get Node we want to remove, and previous Node.
         while (counter < index) {
            ++counter;
            prevNode = currNode;
            currNode = currNode->getNext();
         }

         /* 
          * Update next Node of previous Node to point 
          * to next Node of removed Node.
          */
         if (prevNode == DEFAULT_NODE) {
            this->headNode = currNode->getNext();
         } else {
            prevNode->setNext(currNode->getNext());
         }

         --this->size;
      }
   }
}

// Return Pointer to Tile of Node in LinkedList at given index.
std::shared_ptr<Tile> LinkedList::getTile(int index) const {
   std::shared_ptr<Tile> returnTile = DEFAULT_TILE;

   // Check if index is within bounds of LinkedList.
   if (index >= START && index < this->size) {
      int counter = START;
      std::shared_ptr<Node> currNode = this->headNode;

      // Get the Node at given index.
      while (counter < index) {
         ++counter;
         currNode = currNode->getNext();
      }

      returnTile = currNode->getTile();
   }

   return returnTile;
}

// Return Tile if present in LinkedList.
std::shared_ptr<Tile> LinkedList::search(std::string tileValue) {
   std::shared_ptr<Tile> returnTile = DEFAULT_TILE;

   bool searching = true;
   int counter = START;
   std::shared_ptr<Node> currNode = this->headNode;
   
   /* Loop through LinkedList until element 
    * is found or end of LinkedList is reached. 
    */
   while (searching) {
      if (currNode->getTile()->toString() == tileValue) {
         searching = false;
         returnTile = currNode->getTile();
      }

      ++counter;
      currNode = currNode->getNext();

      if (counter == size) {
         searching = false;
      }
   }

   return returnTile;
}
