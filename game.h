#ifndef _GAME_H
#define _GAME_H

#include <Arduino.h>
#include "main.h"


extern int helpCounter;
extern int rudeCounter;
extern int bangHeadCounter;
extern bool actionNew;
extern uint8_t actionCurrent;

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

enum s_typeSpeed {
  SLOW = 0,
  FAST
};


enum s_item {
  NO_ITEM = 0,
  SWISS_KNIFE,
  SWORD,
  FOOD,
  MEDIKIT,
};

enum s_character {
  KNIGHT = 0,
  PRINCESS,
  TROLL
};

#define GAME_WIDTH        16
#define GAME_HEIGHT       16

#define MAX_DESCRIPTION   80


class cell {
public:
  bool enable;
  int8_t x, y;               // Cartesian coordinates
  const char *descStr;          // Pointer to the description of the scene
  //bool details;           // has addition details
  
  void (*cb)(uint8_t action);

  uint8_t character;
  uint8_t item;

private:
  
};

class player {
public:
  bool go(uint8_t direction);
  bool hasNothing() { return (item == NO_ITEM);}

  uint8_t item;   // Can't hold too much
  int x, y;               // position
};

class character {
public:
  bool isAlive() {return alive;} 

private:  
  bool alive = true;
};
 


// Those are just the enable / disable states of the game board to populate the maze with 2 combined X-Y loops
  const uint8_t gameRooms[GAME_WIDTH][GAME_HEIGHT] = {
// 0 1 2 3 4 5 6 7 8 9 A B C D E F
  {0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0}, // 0
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, // 1
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, // 2
  {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, // 3
  {0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0}, // 4
  {0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1}, // 5
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1}, // 6
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1}, // 7
  {0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1}, // 8
  {0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0}, // 9
  {0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0}, // A
  {0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,0}, // B
  {0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0}, // C
  {0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0}, // D
  {1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0}, // E
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0}, // F
}; // Enter here ^

//////////////////////////////////////////////////////:
// Cell description

const char descEntrance[] = "Welcome to LaMoria Hotel\nMay your stay be enjoyable and like Travolta, try to stay alive...\n\
The Nazguls of Taberah, a local band, sing an Heavy Metal cover of \"Hotel California\" from speakers hidden in the corridor clock\n\
No-one there for your check-in, you're on your own! Good luck!\n";

const char descIntro[] = "RETRO-GAME OS v11.38 - Back to the Future LLC\n";
const char descStandard[] = "Standard Room, nothing to be seen though it's quite dark for a hotel...\n";
const char descWithDoor[] = "This room seems to have a door...\n";
const char descWithTroll[] = "Meet the RedMad Troll. A sign on the wall states to not feed it after midnight\n";
const char descWithKnight[] = "A Knight in armor guards this room\n";
const char descWithDeadKnight[] = "A Knight corpse is laying on the floor.\n";
const char descWithPrincess[] = "The Princess Lounge. A princess lies on a bed half naked. The Prince seems to have left for a loooong time...\n";
const char descWithHarryPotter[] = "Harry Potter is there!\n";


enum s_commands {
  CMD_HELP = 0,
  CMD_MOVE,
  CMD_NORTH,
  CMD_SOUTH,
  CMD_EAST,
  CMD_WEST,
  CMD_INSPECT,
  CMD_INVENTORY,
  CMD_PRESS,
  CMD_FIGHT,
  CMD_TALK,
  CMD_PICK,
  CMD_REMOVE,
  CMD_FEED,
  CMD_FUCK,
  CMD_MAP,
  CMD_INSULT,
  CMD_HELLO,
  CMD_SUICIDE,
  CMD_UNKNOWN,
  CMD_BLANK,
  LAST_COMMAND
};



/* game commands:
 *  - inspect: give details if available
 *  - feed: for the troll
 *  - kill, fight, murder, knock : fight the character
 *  - talk, speak
 *  - fuck (the princess, or else)
 *  - pick : pick an item
 *  - remove: discard an item (?)
 *  - help : display help / usage (commands) but doesn't show all of them
 *  - N, S, E, W : directions
 */

typedef struct {  
  uint8_t cmd;
  char cmdStr[50];
} Command;


// Add more commands for fun : hello, suck, stupid, asshole, moron etc. Increment the rude counter then game over 
// put funny answers for each of the things that don't work
const Command CommandDictionnary[] {
  { CMD_MOVE, "move"},
  { CMD_MOVE, "run"},
  { CMD_MOVE, "escape"},
  { CMD_NORTH, "north"},
  { CMD_SOUTH, "south"},
  { CMD_EAST, "east"},
  { CMD_WEST, "west"},
  { CMD_INSPECT, "inspect"},
  { CMD_INSPECT, "look"},
  { CMD_INSPECT, "peek"},
  { CMD_INSPECT, "observ"},
  { CMD_PRESS, "pull" },
  { CMD_PRESS, "push" },
  { CMD_PRESS, "press" },
  { CMD_PRESS, "action" },
  { CMD_PRESS, "use" },
  { CMD_INVENTORY, "inventory"},
  { CMD_INVENTORY, "list"},
  { CMD_INVENTORY, "backpack"},
  { CMD_INVENTORY, "carry"},
  { CMD_FIGHT, "fight"},
  { CMD_FIGHT, "kill"},
  { CMD_FIGHT, "murder"},
  { CMD_FIGHT, "knock"},
  { CMD_FIGHT, "spear"},
  { CMD_TALK, "talk"},
  { CMD_TALK, "speak"},
  { CMD_PICK, "pick"},
  { CMD_REMOVE, "remove"},
  { CMD_REMOVE, "leave"},
  { CMD_REMOVE, "dispose"},
  { CMD_REMOVE, "discard"},
  { CMD_REMOVE, "drop"},
  { CMD_FEED, "feed"},
  { CMD_FEED, "food"},
  { CMD_FEED, "give food"},
  { CMD_FUCK, "fuck"},
  { CMD_FUCK, "fuck princess"},
  { CMD_FUCK, "fuck the princess"},
  { CMD_FUCK, "make love to princess"},
  { CMD_FUCK, "make love to the princess"},
  { CMD_FUCK, "sodomize princess"},
  { CMD_MAP, "i solemnly swear that i am up to no good"},
  { CMD_MAP, "i solemnly swear that i'm up to no good"},
  //{ CMD_MAP, "map"},    // Just for us to debug
  { CMD_HELP, "help"},
  { CMD_INSULT, "stupid"},
  { CMD_INSULT, "asshole"},
  { CMD_INSULT, "moron"},
  { CMD_INSULT, "you suck"},
  { CMD_INSULT, "shit"},
  { CMD_INSULT, "shithead"},
  { CMD_INSULT, "dick"},
  { CMD_INSULT, "dickhead"},
  { CMD_HELLO, "hello"},
  { CMD_HELLO, "hi"},
  { CMD_SUICIDE, "suicide"},
  { CMD_SUICIDE, "commit suicide"},
};

void displayMap();      // Show the whole gameboard and where the rooms are and it it has something inside
void displayInventory();    // Objects you have
void lookAround();          // Display what's in the room, situation, where to go
uint8_t ProcessSerial(char *line);
void promptDate();
void gameOver();
void gameWin();
void displayHelp();
void typeThis(const char *str, bool fast = false);
bool PlaySoundFx(char *filename);
bool SwitchAction(void);
cell& getCell();
void handleDrop();
void dropItem();
void pickItem();
void handlePick();
void printItem(uint8_t item);

void standardCellCB(uint8_t);
void harryPotterCellCB(uint8_t action);
void knightCellCB(uint8_t action);
void deadKnightCellCB(uint8_t action);
void trollCellCB(uint8_t action);
void deadTrollCellCB(uint8_t action);
void princessCellCB(uint8_t action);
void deadPrincessCellCB(uint8_t action);


extern cell gameBoard[GAME_WIDTH][GAME_HEIGHT];
extern player ThePlayer;
extern character Princess;
extern character Knight;
extern character Troll;

#endif
