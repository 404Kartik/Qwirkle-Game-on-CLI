#include "Player.h"

// Default Constructor of Player.
Player::Player() {
    this->name = DEFAULT_NAME;
    this->id = DEFAULT_ID;
    this->score = DEFAULT_SCORE;
    this->playerHand = DEFAULT_HAND;
}

// Overloaded constructor for player
Player::Player(std::string name, std::string id) {
    this->name = name;
    this->id = id;
    this->score = DEFAULT_SCORE;
    this->playerHand = std::make_shared<LinkedList>();
}

// Copy Constructor of otherPlayer.
Player::Player(const Player& otherPlayer) :
    name(otherPlayer.name),
    id(otherPlayer.id),
    score(otherPlayer.score),
    playerHand(otherPlayer.playerHand)
{}

// Deconstructor for player
Player::~Player() {

}

// Set Score for Player.
void Player::setScore(int score) {
    this->score = score;
}

// Return Score of Player.
int Player::getScore() {
    return this->score;
}

// Return Name of Player.
std::string Player::getName() {
    return this->name;
}

// Return Id of Player.
std::string Player::getId() {
    return this->id;
}


// Set playerHand of Player.
void Player::setInitialHand(std::shared_ptr<LinkedList> dealtHand) {
    this->playerHand = std::make_shared<LinkedList>(*dealtHand);
}

// Return playerHand of Player.
std::shared_ptr<LinkedList> Player::getPlayerHand() {
    return this->playerHand;
}

// Add drawn Tile to playerHand of Player.
void Player::addTileToHand(std::shared_ptr<Tile> drawnTile) {
    this->playerHand->addTile(drawnTile);
}

// Return boolean checking if otherPlayer matches Player.
bool Player::equals(std::shared_ptr<Player> otherPlayer) {
    bool equalsOther = false;
    
    // Compare Name and Id of both Players.
    if (this->name == otherPlayer->getName()
            && this->id == otherPlayer->getId()) {
        equalsOther = true;
    }

    return equalsOther;
}