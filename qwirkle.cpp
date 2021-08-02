#include "GameEngine.cpp"
#include "TileCodes.h"
#include <regex>
#include <memory>
#include <random>

#define BOARD_ROW_COUNT 13
#define BOARD_COL_COUNT 13

#define DEFAULT_INPUT '0'
#define NEW_GAME '1'
#define LOAD_GAME '2'
#define CREDITS '3'
#define QUIT '4'

void displayMenu();
void displayCredits();



std::shared_ptr<LinkedList> generateTileBag();
std::shared_ptr<Player> createNewPlayer(std::string playerId);
void initialisePlayer(std::shared_ptr<LinkedList> tileBagPtr, 
      std::shared_ptr<GameEngine> gameEnginePtr, 
            std::shared_ptr<Player> playerPtr);

// Main method/ entry point for Programme.
int main(void) {
   bool running = true;

   char input = DEFAULT_INPUT;

   while (running) {
      displayMenu();

      std::cin >> input;

      if (input == NEW_GAME) {
         // New Game
         // Initialise empty board.
         std::vector<std::vector<std::string> > board;

         for (int i = START; i < BOARD_ROW_COUNT; i++) {
            std::vector<std::string> newBoardRow;

            for (int j = START; j < BOARD_COL_COUNT; j++) {
               newBoardRow.push_back("  ");
            }

            board.push_back(newBoardRow);
         }

         // Create Game Engine with empty board.
         std::shared_ptr<GameEngine> gameEnginePtr = 
               std::make_shared<GameEngine>(board);
         std::shared_ptr<LinkedList> tileBagPtr = 
               std::make_shared<LinkedList>(*generateTileBag());

         // Initialise Player One and Player Two.
         std::shared_ptr<Player> playerOnePtr = 
               std::make_shared<Player>(*createNewPlayer("1"));
         initialisePlayer(tileBagPtr, gameEnginePtr, playerOnePtr);

         std::shared_ptr<Player> playerTwoPtr = 
               std::make_shared<Player>(*createNewPlayer("2"));
         initialisePlayer(tileBagPtr, gameEnginePtr, playerTwoPtr);

         // Set Tile Bag LinkedList for Game Engine.
         gameEnginePtr->setTileBag(tileBagPtr);

         // Play Game starting with Player One.
         gameEnginePtr->playGame(playerOnePtr);
      } else if (input == LOAD_GAME) {
         // Load Game
         std::cout << std::endl;
         std::cout << "Enter the filename from which to load a game" << std::endl;
         std::cout << "> ";

         std::string fileName = "";
         std::cin >> fileName;

         // Read file from given fileName.
         std::ifstream file(fileName);

         // Check if file exists.
         if (file.is_open()) {
            // Read in each line from save file into string array.
            std::string gameInfo[10];
            std::string fileLine = "";

            int lineCounter = START;
   
            while (std::getline(file, fileLine)) {
               gameInfo[lineCounter] = fileLine;
               ++lineCounter;
            }

            const std::string PLAYER_ONE_NAME = gameInfo[0];
            const std::string PLAYER_ONE_SCORE = gameInfo[1];
            const std::string PLAYER_ONE_HAND = gameInfo[2];
            const std::string PLAYER_TWO_NAME = gameInfo[3];
            const std::string PLAYER_TWO_SCORE = gameInfo[4];
            const std::string PLAYER_TWO_HAND = gameInfo[5];
            const std::string CURRENT_BOARD_SHAPE = gameInfo[6];
            const std::string CURRENT_BOARD_STATUS = gameInfo[7];
            const std::string TILE_BAG_CONTENTS = gameInfo[8];
            const std::string CURRENT_PLAYER_NAME = gameInfo[9];

            // Load Player One
            std::shared_ptr<Player> loadedPlayerOne = 
                  std::make_shared<Player>(PLAYER_ONE_NAME, "1");
            loadedPlayerOne->setScore(stoi(PLAYER_ONE_SCORE));

            int begin = START;

            // Load in Tiles from save file into loadedPlayerOne's playerHand.
            for (unsigned int i = START; i < PLAYER_ONE_HAND.size(); ++i) {
               if (PLAYER_ONE_HAND[i] == ',' 
                     || i == PLAYER_ONE_HAND.size() - 2) {
                  char tileColour = PLAYER_ONE_HAND[begin];
                  int tileShape = PLAYER_ONE_HAND[begin + 1] - DEFAULT_INPUT;

                  std::shared_ptr<Tile> loadedTile = 
                        std::make_shared<Tile>(tileColour, tileShape);

                  loadedPlayerOne->getPlayerHand()->addTile(loadedTile);
                  begin = i + 2;
               }
            }

            // Load Player Two
            std::shared_ptr<Player> loadedPlayerTwo = 
                  std::make_shared<Player>(PLAYER_TWO_NAME, "2");
            loadedPlayerTwo->setScore(stoi(PLAYER_TWO_SCORE));

            begin = START;

            // Load in Tiles from save file into loadedPlayerTwo's playerHand.
            for (unsigned int i = START; i < PLAYER_TWO_HAND.size(); ++i) {
               if (PLAYER_TWO_HAND[i] == ',' 
                     || i == PLAYER_TWO_HAND.size() - 2) {
                  char tileColour = PLAYER_TWO_HAND[begin];
                  int tileShape = PLAYER_TWO_HAND[begin + 1] 
                                - DEFAULT_INPUT;

                  std::shared_ptr<Tile> loadedTile = 
                        std::make_shared<Tile>(tileColour, tileShape);

                  loadedPlayerTwo->getPlayerHand()->addTile(loadedTile);
                  begin = i + 2;
               }
            }

            // Load Board
            std::vector<std::vector<std::string> > loadedBoard;
            int loadedBoardXDims = START;
            int loadedBoardYDims = START;

            // Get Loaded Board's Shape from save file.
            for (unsigned int i = START; i < CURRENT_BOARD_SHAPE.size(); ++i) {
               if (CURRENT_BOARD_SHAPE[i] == ',') {
                  loadedBoardYDims = stoi(CURRENT_BOARD_SHAPE.substr(START, i));
                  loadedBoardXDims = stoi(CURRENT_BOARD_SHAPE.substr(i + 2));
               }
            }

            // Initialise loadedBoard as empty.
            for (int i = START; i < loadedBoardYDims; ++i) {
               std::vector<std::string> newBoardRow;

               for (int j = START; j < loadedBoardXDims; ++j) {
                  newBoardRow.push_back("  ");
               }

               loadedBoard.push_back(newBoardRow);
            }

            begin = START;

            // Load placed Tiles from save file into loadedBoard.
            for (unsigned int i = START; i < CURRENT_BOARD_STATUS.size(); 
                  ++i) {
               if (CURRENT_BOARD_STATUS[i] == ',') {
                  const int dividerPos = CURRENT_BOARD_STATUS[i - 3] == '@'
                        ? i - 3
                        : i - 4;

                  std::string tileValue = 
                     CURRENT_BOARD_STATUS.substr(begin, 2);

                  char c = 'A';
                  int rowCount = START;

                  while (c != CURRENT_BOARD_STATUS[dividerPos + 1]) {
                     ++c;
                     ++rowCount;
                  }

                  std::cout << dividerPos << std::endl;
                  // Place Tiles onto loadedBoard.
                  loadedBoard[rowCount][stoi(CURRENT_BOARD_STATUS
                                             .substr(dividerPos + 2, 
                                                     i - dividerPos - 2))]
                        = tileValue;
                  begin = i + 2;
               }
            }

            // Load Game Engine
            std::shared_ptr<GameEngine> loadedGameEngine = 
                  std::make_shared<GameEngine>(loadedBoard);
            loadedGameEngine->addPlayer(loadedPlayerOne);
            loadedGameEngine->addPlayer(loadedPlayerTwo);

            // Load Tile Bag
            std::shared_ptr<LinkedList> loadedTileBag = 
                  std::make_shared<LinkedList>();

            std::vector<std::string> tiles;
            begin = START;


            // Add all Tiles from save file into loadedTileBag.
            for (unsigned int i = START; i < TILE_BAG_CONTENTS.size(); ++i) {
               if (TILE_BAG_CONTENTS[i] == ',' 
                     || i == TILE_BAG_CONTENTS.size() - 2) {
                  char tileColour = TILE_BAG_CONTENTS[begin];
                  int tileShape = TILE_BAG_CONTENTS[begin + 1] 
                                - DEFAULT_INPUT;

                  std::shared_ptr<Tile> loadedTile = 
                        std::make_shared<Tile>(tileColour, tileShape);

                  loadedTileBag->addTile(loadedTile);
                  begin = i + 2;
               }
            }

            loadedGameEngine->setTileBag(loadedTileBag);

            // Load Current Player
            std::shared_ptr<Player> loadedCurrPlayer = 
               loadedPlayerOne->getName() == gameInfo[9] 
                  ? loadedPlayerOne 
                  : loadedPlayerTwo;

            // Play Game
            loadedGameEngine->playGame(loadedCurrPlayer);

            file.close();
         }
      } else if (input == CREDITS) {
         // Credits
         displayCredits();
      } else if (input != QUIT) {
         // Invalid input
         std::cout << "Invalid input. Please try again." 
            << std::endl;
      }

      if (input == QUIT) {
         running = false;
      }
   }

   std::cout << std::endl;
   std::cout << "Goodbye" << std::endl;

   return EXIT_SUCCESS;
}

// Display menu options.
void displayMenu() {
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
   std::cout << std::endl;
   std::cout << "> ";
}

// Return Pointer of newly generated Tile Bag LinkedList.
std::shared_ptr<LinkedList> generateTileBag() {
   const int COLOUR_COUNT = 6;
   const int SHAPE_COUNT = 6;
   const int AMOUNT_OF_DUPLICATES = 2;

   char colours[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
   int shapes[] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

   std::shared_ptr<LinkedList> newTileBag = 
      std::make_shared<LinkedList>();


   // Generate tiles and add them to the bag
   for (int n = START; n < AMOUNT_OF_DUPLICATES; ++n) {
      for (int i = START; i < COLOUR_COUNT; ++i) {
         for (int j = START; j < SHAPE_COUNT; ++j) {
               std::shared_ptr<Tile> newTile = 
                     std::make_shared<Tile>(colours[i], shapes[j]);
               newTileBag->addTile(newTile);
         }
      }
   }

    return newTileBag;
}

// Return Pointer to newly generated Player.
std::shared_ptr<Player> createNewPlayer(std::string playerId) {
   std::string playerName = "";

   // Get new Player name.
   std::cout << "Enter a name for player " << playerId 
      << " (uppercase characters only)" << std::endl;
   std::cout << "> ";

   // Check if new Player name is valid. 
   // Continue to ask for Player name until valid name is given.
   bool validName = false;

   while (!validName) {
      std::cin >> playerName;

      validName = true;

      // Check if any char in new Player name is not capital or symbol.
      for (unsigned int i = START; i < playerName.size(); ++i) {
         if (playerName[i] < 'A' || playerName[i] > 'Z') {
            validName = false;
         }
      }

      if (!validName) {
         std::cout << "Invalid name. Please try again." << std::endl;
         std::cout << std::endl;

         std::cout << "> ";
      }
   }

   std::cout << std::endl;

   std::shared_ptr<Player> newPlayer = 
      std::make_shared<Player>(playerName, playerId);

   return newPlayer;
}

// Generate randomly drawn playerHand LinkedList for playerPtr.
void initialisePlayer(std::shared_ptr<LinkedList> tileBag, 
      std::shared_ptr<GameEngine> engine, 
            std::shared_ptr<Player> playerPtr) {
   const int MAX_HAND_SIZE = 6;
   const int MAX_TILE_BAG_SIZE = 72;

   engine->addPlayer(playerPtr);

   std::shared_ptr<LinkedList> tempPlayersHand = 
      std::make_shared<LinkedList>();

   // Generate random distribution.
   std::random_device randomSeed;
   std::uniform_int_distribution<int> 
      uniform_dist(START, MAX_TILE_BAG_SIZE-1);

   int i = START;

   // Loop and add randomly drawn Tiles into temPlayersHand for playerPtr.
   while (i < MAX_HAND_SIZE) {
      int randIndex = uniform_dist(randomSeed);

      if (tileBag->getTile(randIndex) != nullptr) {

         std::shared_ptr<Tile> newTile = 
            std::make_shared<Tile>(*tileBag->getTile(randIndex));

         tempPlayersHand->addTile(newTile);
         tileBag->removeTile(randIndex);

         ++i;
      }
   }

   playerPtr->setInitialHand(tempPlayersHand);
}


// Display the information of each student involved in the group project.
void displayCredits() {
   const int NUM_OF_STUDENTS = 4;
   const int NUM_OF_INFO = 3;

   const int STUDENT_NAME_COL = 0;
   const int STUDENT_ID_COL = 1;
   const int STUDENT_EMAIL_COL = 2;

   std::string students[NUM_OF_STUDENTS][NUM_OF_INFO] = {
      {"Vincent Tso", "S3855073", "S3855073@student.rmit.edu.au"},
      {"Justin John", "S3846390", "S3846390@student.rmit.edu.au"},
      {"Kartik Kumar", "S3788894", "S3788894@student.rmit.edu.au"},
      {"Anne Maria Sunil", "S3820057", "S3820057@student.rmit.edu.au"}
   };

   std::cout << std::endl;

   std::cout << "----------------------------------" << std::endl;

   for (int i = START; i < NUM_OF_STUDENTS; ++i) {
      std::cout << "Name: " << 
         students[i][STUDENT_NAME_COL] << std::endl;
      std::cout 
         << "Student ID: " << students[i][STUDENT_ID_COL] << std::endl;
      std::cout 
         << "Email: " << students[i][STUDENT_EMAIL_COL] << std::endl;

      if (i < NUM_OF_STUDENTS - 1)
         std::cout << std::endl;
   }

   std::cout << "----------------------------------" << std::endl;
   std::cout << std::endl;
}
