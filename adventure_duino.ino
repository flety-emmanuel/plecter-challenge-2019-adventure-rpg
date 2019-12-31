
#include <TimeLib.h>

#include "main.h"
#include "game.h"

/* This is one of the puzzles from the Plecter Labs xmas challenge 2019
 *  in the shape of a classic text aventure / RPG formerly compiled to 
 *  run on the Crystal Focus X (v10).
 *  It has been adapted to run on a more standard arduino. Based on the size
 *  of the maze and the crude programming, it make a quite extensive use
 *  of RAM which doesn't allow to compile it on a Arduino Nano/Uno/Mega but rather
 *  on a modern 32 bit platform such as the Arduino Zero, MKR or an Adafruit M0/M4
 *  
 *  I refactored the code to be as close as possible to the Arduino standard. It could probably
 *  run on a regular 8 bit duino by storing in RAM only the cells that have a special function, 
 *  character or item but for now, you'll need a beefy board to play :-)
 */



//////////////////////////////////////////////////////////////////////////
// Serial Port buffers (for Serial USB)
// Could be send to a class but fine for now
char SerialBuffer[MAX_SERIAL_LEN];
int SerialIndex = 0;

void setup() {
 
  pinMode(PIN_ACT, INPUT_PULLUP);
  pinMode(PIN_AUX, INPUT_PULLUP);
  
  // Serial port init
  Serial.begin(115200);
  
  delay(2000);

  // Init games cells and populate the game board
  for(int i = 0 ; i < GAME_HEIGHT ; i++) {
    for(int j = 0 ; j < GAME_WIDTH ; j++) {
      gameBoard[i][j].enable = gameRooms[i][j];
      gameBoard[i][j].x = j;
      gameBoard[i][j].y = i;
      gameBoard[i][j].descStr = descStandard;
      gameBoard[i][j].cb = standardCellCB;
    }
  }

  // Then populate a few items and characters for certain room
  gameBoard[14][0].cb = harryPotterCellCB;
  gameBoard[14][0].descStr = descWithHarryPotter;
  
  gameBoard[11][12].cb = knightCellCB;
  gameBoard[11][12].descStr = descWithKnight;
  
  gameBoard[2][4].cb = trollCellCB;       // the cells leading to him are disabled for now
  gameBoard[2][4].descStr = descWithTroll;
  
  gameBoard[2][13].cb = princessCellCB;   // the cells leading to her are disabled for now
  gameBoard[2][13].descStr = descWithPrincess;

  // Sword to pick
  gameBoard[8][2].item = SWORD; 

  // Finally define play start position. Maybe we randomize this to a random cell # that has a working room
  // defautl / normal start position
  ThePlayer.x = 7;
  ThePlayer.y = 15;

  // (just next to) Potter room
  //ThePlayer.x = 1;
  //ThePlayer.y = 14;

  // (just next to) Knight room
  //ThePlayer.x = 12;
  //ThePlayer.y = 10;

  // (just next to) sword
  //ThePlayer.x = 3;
  //ThePlayer.y = 8;
  
  ThePlayer.item = SWISS_KNIFE;
  //ThePlayer.item = SWORD;

  // Set time
  setTime(random(1000000)+ random(100000)); // Sync Arduino clock some random time
  

  // Original game checked / looked for resources on the SD card and would not start if you didn't find the puzzle leading
  // to the game contents (audio files)
  /*
  int resScan = FileLookup(GAME_PATH, INTRO_SND_FILE);
  resScan += FileLookup(GAME_PATH, TYPE_SND_FILE);
  resScan += FileLookup(GAME_PATH, GAMEOVER_SND_FILE);
  if(resScan != 3) {
    Serial.print("That was a good start, but you're missing something on your 360kB floppy disk... \n");
    Serial.print("Sorry, you can't play the game. Try again! \n");
    while(1);
  }*/

  Serial.print("\n\n\n\n\n");
  typeThis(descIntro);
  PlaySoundFx(INTRO_SND_FILE);
  displayHelp();
}

void loop() {
  
  if(actionNew) {
    actionNew = false;
    switch(actionCurrent) {
      case CMD_MAP:
        displayMap();
        break;

      case CMD_INSPECT:
        Serial.print(getCell().descStr);
        lookAround();
        break;

      case CMD_NORTH:
      case CMD_SOUTH:
      case CMD_EAST:
      case CMD_WEST:
        ThePlayer.go(actionCurrent);
        getCell().cb(actionCurrent);
        break;

      case CMD_INVENTORY:
        displayInventory();
        break;


      case CMD_REMOVE:
        handleDrop();
        break;

      case CMD_PICK:
        handlePick();
        break;

      case CMD_SUICIDE:
        Serial.print("You look around and there's nothing to help you with that. Flat walls and nothing to hang a rope too. Plus, you didn't even bring a rope!\n");
        Serial.print("But there are alternative solutions, if you're inventive with your moves\n");
        break;  

      case CMD_MOVE:
        typeThis("Sure, but where????\n", FAST);
        break;

      case CMD_FIGHT:
      case CMD_TALK:
      case CMD_FEED:
      case CMD_FUCK:
      case CMD_HELLO:
      case CMD_PRESS:
        getCell().cb(actionCurrent);
        break;
        
      case CMD_INSULT:
        Serial.print("I don't care, like Obiwan, I have the high ground\n");
        rudeCounter++;
        if(rudeCounter > 6) {
          Serial.print("Oh well, you looked for it...");
          delay(500);
          gameOver();
        }
        break;

      case CMD_HELP:
        helpCounter++;
        if(helpCounter > 7) {
          Serial.print("This is a Plecter Labs challenge, you're supposed to find out by yourself\n");
          delay(500);
          Serial.print("You want to go home and rethink your life. Bye for now\n");
          gameOver();
        }
        else {
          displayHelp();
        }

        
        
      case CMD_BLANK:
        break;  

      default:
        Serial.print("whut?? \n");
        break;
    }
    promptDate();
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Process serial ports and serial commands / interpreter 
  while(Serial.available() > 0) {
    SerialBuffer[SerialIndex] = Serial.read();
    if(SerialBuffer[SerialIndex] == '\n' || SerialBuffer[SerialIndex] == '\r') {
      if(SerialIndex > MAX_SERIAL_LEN) {
        SerialIndex = 0;
        memset(SerialBuffer, '\0', sizeof(SerialBuffer));
        break;
      }
      else {
        // Process command
        char StringBuffer[strlen(SerialBuffer) + 2];
        strcpy(StringBuffer, SerialBuffer);
        int len = strlen(StringBuffer);
        if(len==1)  // Empty command, just the terminator
          break;
        // purge CR and LF
        for(int i = 0 ; i < len ; i ++) {
          if((StringBuffer[i] == '\n') || (StringBuffer[i] == '\r'))
            StringBuffer[i] = '\0';
        }        
        // Reset Index
        SerialIndex = 0;
        memset(SerialBuffer, '\0', sizeof(SerialBuffer));
        actionCurrent = ProcessSerial(StringBuffer);
      }
      SerialIndex = 0;
    }
    else {
      SerialIndex++;
      if(SerialIndex > MAX_SERIAL_LEN)
        SerialIndex = 0;
    }
  }  // End Of Serial Processing
} // END OF  MAIN LOOP
