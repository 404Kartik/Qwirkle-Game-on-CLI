
#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "LinkedList.h"

#define DEFAULT_NAME "None"
#define DEFAULT_ID "0"
#define DEFAULT_SCORE 0
#define DEFAULT_HAND nullptr

class Player {
public:
    Player();
    Player(std::string name, std::string id);
    Player(const Player& otherPlayer);
    ~Player();
    void setScore(int score);
    int getScore();
    std::string getName();
    std::string getId();
    std::shared_ptr<LinkedList> getPlayerHand();
    bool equals(std::shared_ptr<Player> otherPlayer);
    void addTileToHand(std::shared_ptr<Tile> drawnTile);
    void setInitialHand(std::shared_ptr<LinkedList> dealtHand);

private:
    std::string name;
    std::string id;
    int score;
    std::shared_ptr<LinkedList> playerHand;

};

#endif // ASSIGN2_PLAYER_H
