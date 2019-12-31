#include <stdarg.h>
#include <TimeLib.h>
#include "game.h"


// Use those to lead to game over if you are an asshole or a askhole
int helpCounter = 0;
int rudeCounter = 0;
int bangHeadCounter = 0;
bool actionNew = false;
uint8_t actionCurrent = CMD_UNKNOWN;
cell gameBoard[GAME_WIDTH][GAME_HEIGHT];
player ThePlayer;
character Princess;
character Knight;
character Troll;


cell& getCell() {
  return(gameBoard[ThePlayer.y][ThePlayer.x]);
}

bool player::go(uint8_t direction) {
  bool res = false;
  switch(direction) {
    case CMD_NORTH:
      if(((y-1) >= 0)) {
        if(gameBoard[y-1][x].enable) {
          y--;
          res = true;
          Serial.print("You moved NORTH\n");
        }
      }
      break;

    case CMD_SOUTH:
      if(((y+1) < GAME_HEIGHT)) {
        if(gameBoard[y+1][x].enable) {
          y++;
          res = true;
          Serial.print("You moved SOUTH\n");
        }
      }
      break; 


    case CMD_EAST:
      if(((x+1) < GAME_WIDTH)) {
        if(gameBoard[y][x+1].enable) {
          x++;
          res = true;
          Serial.print("You moved EAST\n");
          if((x==15) && (y == 0)) {     // you reached the exit
            gameWin();
          }
        }
      }
      break;
      
    case CMD_WEST:
      if(((x-1) >= 0)) {
        if(gameBoard[y][x-1].enable) {
          x--;
          res = true;
          Serial.print("You moved WEST\n");
        }
      }
      break;

    default:
      Serial.print("you decide to stay here\n");
      break;   
  }

  if(!res) {
    Serial.print("You can't go there\n");
    bangHeadCounter++;
    if(bangHeadCounter > 7) {
      Serial.print("It started as a trend, then a hobby, and now stubborness...\n");
      delay(300);
      Serial.print("After banging your head on the wall so much, you die of a stroke\n");
      gameOver();
    }
    else if(bangHeadCounter > 3) {
      Serial.print("Yeah, yeah, keep going....\n");
    }
  }
  else
    bangHeadCounter = 0;
    
  return(res);  
}

// Checks room around and move the player in the game board when there's nothing else to do
void standardCellCB(uint8_t action) {
  switch(action) {
    case CMD_PRESS:
      Serial.print("There's nothing to use in that room...\n");
      break;
    
    case CMD_FIGHT:
      Serial.print("Since there's no one around, you punch yourself in the face\n");
      Serial.print("As you looked ugly already and it's not the first time you do that, you don't even get XP points here...\n");
      break;
      
    case CMD_FEED:
      Serial.print("No one to feed, boooo\n");
      break;
    
    case CMD_FUCK:
      Serial.print("You'd need a partner for that. No one around, sorry\n");
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      Serial.print("Like Duke Nukem, you like talking to yourself. The corridor echoes your own voice in sign of approval\n");
      break;
    
  }
}

int hpFeedCounter = 0;
void harryPotterCellCB(uint8_t action) { 
   switch(action) {    
    case CMD_FIGHT:
      Serial.print("Fighting a sorcerer? really?\n");
      delay(800);
      Serial.print("Harry potter holds his wand and spells \"Ignorantus, Vanisharum, Energy, Mr Spock!!\" and turns you in TruCosmicJoules (tm)\n");
      delay(500);
      Serial.print("During a fraction of seconds, you realise that you've been turned into a lightsaber blade and your johnson feels like a small, unstable crystal\n");
      delay(800);
      typeThis("Karma is a bitch\n");
      delay(300);
      gameOver();
      break;
      
    case CMD_FEED:
      Serial.print("Harry looks at your half eaten sandwich and states he would prefer a butter beer\n");
      hpFeedCounter++;
      if(hpFeedCounter > 3) {
        Serial.print("As you're no giving him his butter beer, Harry is so frustrated that he turns you in a toad\n");
        typeThis("before smashing you with his foot\n");
        delay(300);
        gameOver();
      }
      break;
    
    case CMD_FUCK:
      Serial.print("Harry isn't really into this with... \"armor dudes\"\n");
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      Serial.print("Harry Potter is happy to see a visitor.\n");
      Serial.print("With a smile, he claims \"ego solemniter iuro quod ego sum, usque ut nihil boni\"\n");
      delay(300);
      Serial.print("Then he adds: \"When things fail, it's great to have a band-aid\"\n");
      typeThis("48 41 52 52 59 20 50 4F 54 54 45 52 20 57 41 53 20 48 45 52 45 20 54 4F 20 4D 45 53 53 20 55 50\n");
      typeThis("FD F5 60 3E 20 A7 C6 DF 22 B2 9F E7 34 8F 1C 4A EA B8 41 87 C8 95 E1 64 69 0B 6E 6A 45 7A 6E 20\n");
      break;
  }
}

void knightCellCB(uint8_t action) { 
  switch(action) {
    case CMD_PRESS:
      Serial.print("There's nothing you see to use in that room...\n");
      break;
    
    case CMD_FIGHT:
      if(ThePlayer.item == SWISS_KNIFE) {
        Serial.print("You attempt to kill the knigth with your swiss-army knife but you grab the toothpick instead of a blade\n");
        delay(200);
        Serial.print("Chubby fingers... Next time, don't be on the cheap and get some Mitril instead of that can-steel armor!\n");
        delay(300);
        typeThis("Like in \"Smoke on the Water\", you die with an aweful sound\n", FAST);
        typeThis("Funky Claude couldn't help you since he's dead too\n", FAST);
        delay(200);
        typeThis("Well, he died in 2013, but this is Back to the Future LLC game grade here, so we can pretend anything\n");
        gameOver();
      }
      else {
        typeThis("Your elfic sword chops the knight in 2 like butter\n");
        delay(200);
        Serial.print("No time to make a toast and there's no CFX around\n");
        getCell().descStr = descWithDeadKnight;
        getCell().cb = deadKnightCellCB;
        typeThis("Now the Knight is dead, you notice a lever in the back of the room\n", FAST);
        delay(300);
        typeThis("There is a sign above stating \"DON'T TOUCH THIS\". Since you're so wise, what you gonna do?\n", FAST);
      }
      break;
      
    case CMD_FEED:
      Serial.print("The Knight doesn't seem hungry. That's what you understand from the grunts thru his helmet...\n");
      break;
    
    case CMD_FUCK:
      typeThis("That knight isn't easy and gets easily offended\n");
      delay(300);
      Serial.print("He chops you in half before you end your flirt\n");
      gameOver();
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      //Serial.print("Hard to know what emotion is behind the helmet...");
      typeThis("The Knight isn't that verbose\n");
      typeThis("You think you hear, indistinctly \"I AM GRUUUUNNTTTTSSS\"\n");
      break;
  }  
}

int leverCounter = 0;
void deadKnightCellCB(uint8_t action) { 
    switch(action) {
    case CMD_PRESS:
      leverCounter++;
      if(leverCounter < 2) {
        typeThis("Dudes in amor don't give a shit about warnings, you pull that freakin' lever\n");
        Serial.print("You seem to trigger something, and hear some distant sounds.\n");
        PlaySoundFx(SECRET_DOOR2_SND_FILE);
        //PlaySoundFx(SECRET_DOOR1_SND_FILE);   // That's for the troll->princess unlock
        typeThis("mmmmm.... interesting. Wish I knew where that came from!\n");
        
        delay(200);

        // Unlock game cell
        gameBoard[2][4].enable = true;    // Troll
        gameBoard[3][4].enable = true;
      }
      else {
        typeThis("That was a fair warning. You pull too hard and trigger a second mechanism\n");
        delay(200);
        typeThis("A hatch opens under your feet and you impale yourself on a very stingy trap\n");
        typeThis("You barely have the time to realise that you were the boss the whole time for that decision\n");
        gameOver();
      }
      break;
    
    case CMD_FIGHT:
      typeThis("The Ghost of BOROMIR appears and say:\n");
      delay(300);
      typeThis("One does not simply kill a dead body\n");
      delay(300);
      typeThis("Did you try that on your own, or asked a friend?\n");
      break;
      
    case CMD_FEED:
      typeThis("You try to feed a corpse. Your seems to mismatch necrophily and necromancy\n");
      break;
    
    case CMD_FUCK:
      typeThis("Though it's adult grade RPG text adventure, you'll just die for trying this\n");
      gameOver();
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      Serial.print("Like to talk to yourself, hu? Plays with yourself as well?\n");
      break;
  }  
}

int trollTalkCounter = 0;
void trollCellCB(uint8_t action) {
  switch(action) {
    case CMD_PRESS:
      Serial.print("The room is small and empty\n");
      break;
    
    case CMD_FIGHT:
      if(ThePlayer.item == SWORD) {
        typeThis("That's a very small cell. Your large sword locks in a corner\n");
        typeThis("The Troll manages to push you and you fall due to your heavy armor\n");
        typeThis("He catches your elfic sword and chops your head\n", FAST);
        gameOver();
      }
      else {  // This locks the game ! you can't finish it as you won't unlock the princess
        typeThis("You manage to unfold the corkscrew from your swiss-army knife\n");
        typeThis("You pierce the heart of the RED-MAD Troll and his Darkness escapes his body in the shape of black blood\n");
        typeThis("You feel some little victory here but wonders if that was the right thing to do...?\n");
        getCell().cb = deadTrollCellCB;
      }
      break;
      
    case CMD_FEED:
      typeThis("You ignored the warning on the wall... That was pretty explicit but you're the one on duty here!\n");
      typeThis("You feed the troll you're immediately overwhelmed by some Passive-Agressive Verbal Diarrhea\n", FAST);
      typeThis("As non-sense is bleeding thru your eyes, you're losing your sight and sanity\n", FAST);
      typeThis("Your final dab of common sense was already gone when you started this... see you next time!\n");
      gameOver();
      break;
    
    case CMD_FUCK:
      typeThis("You fuck the troll, who likes it\n");
      typeThis("However, you contract Trollism and instantly loose your common sense.\n");
      typeThis("He decide you'll stay here forever, as a slave, waiting for some other Knight to spoon feed you\n", FAST);
      typeThis("To keep you handy, he closes the door and locks it with a spell\n");
      PlaySoundFx(EXIT_DOOR_SND_FILE);   // locks the room
      //gameOver();
      gameBoard[3][4].enable = false; // Locks the player in troll cell - Locks the game as well
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      trollTalkCounter++;
      typeThis("[PLAYER]: Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vehicula, justo dapibus egestas ullamcorper, nibh metus accumsan leo, a vehicula dolor tellus eget mauris. Donec a erat ligula. Nullam viverra odio et tortor blandit lobortis\n");
      if(trollTalkCounter < 3) 
        typeThis("[TROLL]: meeeeeee, waaaaaaant... waaaaaaant.... foooooodd\n");
      else {
        typeThis("The troll is disappointed you didn't feed him but resigns and accepts your mental strength and verbal perseverance.\n");
        typeThis("[TROLL]: you should visit the Princess\n");
        Serial.print("The troll pulls a lever which seems to trigger something, and you hear some distant sounds.\n");
        PlaySoundFx(SECRET_DOOR1_SND_FILE);   // That's for the troll->princess unlock
        typeThis("mmmmm.... interesting. Wish I knew where that came from!\n", FAST);
        gameBoard[2][10].enable = true; // princess lounge
        gameBoard[2][11].enable = true;
        gameBoard[2][12].enable = true;
        gameBoard[2][13].enable = true;  
      }
      break;
  }  
}


void deadTrollCellCB(uint8_t action) {
  switch(action) {
    case CMD_PRESS:
      Serial.print("The room is small and empty\n");
      break;
    
    case CMD_FIGHT:
      Serial.print("He's dead already, in case you didn't notice\n");
      break;
      
    case CMD_FEED:
    case CMD_FUCK:
      typeThis("You should have considered this *before* killing him!\n"); 
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
        Serial.print("Do you expect that dead troll to answer you ???\n");
      break;
  }  
}


int fuckPrincessCounter = 0; 
void princessCellCB(uint8_t action) {
  switch(action) {
    case CMD_PRESS:
      Serial.print("She's not an object. At least, not yours yet!\n");
      typeThis("Based on the room description, she's looking for action\n");
      break;
    
    case CMD_FIGHT:
      if(ThePlayer.item != NO_ITEM) {
        typeThis("You didn't like her Mme Claude brothel look, so you kill the Princess\n");
        typeThis("She squeeks a little but dies quickly, making your testosterone disappointed\n");
        typeThis("Too much of Game of Thrones on saturday evenings, hu ?\n", FAST);
        getCell().cb = deadPrincessCellCB;
      }
      else { 
        typeThis("You don't have any weapon! The princess kicks you in the balls, Xena-Da-Warriar grade!\n");
        typeThis("You squeek a little before she vapes you with her lipstick-pepper-spray combo\n", FAST);
        typeThis("Then she wrecks your skull with a ashtray, but it takes a few hours to complete\n", FAST);
        gameOver();
      }
      break;
      
    case CMD_FEED:
      typeThis("She doesn't seem to be be hungry for food...\n");
      break;
    
    case CMD_FUCK:
      fuckPrincessCounter++;
      if(fuckPrincessCounter < 2) {
        typeThis("You make love to the Princess, who likes it\n");
        typeThis("As she's in good mood, she's not keeping you as a slave and let you go\n");
        typeThis("[PRINCESS]: You've been good to me, though next time, remove your armor first...\n");
        typeThis("[PRINCESS]: To express my gratitude (and relief) here's something for your Quest\n");
        typeThis("[PRINCESS]: 687474703a2f2f7777772e706c65637465726c6162732e636f6d2f4d656469612f46756e2f323031392d4368616c6c656e67652f73616d75656c2d736f746f6d61796f722e7a6970 \n");  // translates as http://www.plecterlabs.com/Media/Fun/2019-Challenge/samuel-sotomayor.zip
        typeThis("She then flicks a switch on her makeup table\n");
        Serial.print("You hear some distant noises...\n");        
        PlaySoundFx(SECRET_DOOR1_SND_FILE);   // opens the exit
        typeThis("You think it's your chance to leave this crazy place\n");
        gameBoard[0][10].enable = true;
        gameBoard[0][11].enable = true;
        gameBoard[0][12].enable = true;
        gameBoard[0][13].enable = true;
        gameBoard[0][14].enable = true;
        gameBoard[0][15].enable = true;   // That's the exit
      }
      else {
        typeThis("Again? You over estimate yourself but give it a try because your a dude in armor\n");
        typeThis("Tired by this quest and that recent effort to satisfy the Princess, your heart doesn't hold and breaks\n");
        delay(300);
        typeThis("As a side effect, you also break the Princess's heart but like Gloria Gaynor, she will survive (unlike you)\n");
        gameOver();
      }
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
      typeThis("Princess doesn't seem to expect a talk...\n");
      break;
  }
}


void deadPrincessCellCB(uint8_t action) {  
  switch(action) {
    case CMD_PRESS:
      Serial.print("The room is small and empty\n");
      break;
    
    case CMD_FIGHT:
      Serial.print("She's dead already, in case you didn't notice\n");
      break;
      
    case CMD_FEED:
    case CMD_FUCK:
      typeThis("You should have considered this *before* killing her!\n"); 
      break;
      
    case CMD_TALK:
    case CMD_HELLO:
        Serial.print("Do you expect that dead princess to answer you ???\n");
      break;
  }  
}


void promptDate() {
  String str;
  str = "[gimli@la-moria]";
  str += (String)(hour()) + ":" + (String)(minute()) + ":" + (String)(second()) + ":" + (String)(day()) + ":" + (String)(month()) + ":" + (String)(year());
  //Serial.print("[gimli@la-moria]%d:%d:%d %d/%d/%d > ");
  Serial.print(str);
}

// Add an indication on the wall of a room that you can type a special command to get the map
// From harry potter : use that text to create the special statement
// The map is normally disguised as a blank piece of parchment. 
// To view the map, one must tap it with one's wand and recite, "I solemnly swear that I am up to no good." The content of the map will then reveal itself.[10]
// To hide the contents of the map so the parchment appears blank again, one must again tap it and recite, "Mischief managed." 
void displayMap() {
  Serial.print("\nMarauder's Map:\n\n");
  for(int i = 0 ; i < GAME_HEIGHT ; i++) {
    for(int j = 0 ; j < GAME_WIDTH ; j++) {
      if(!gameBoard[i][j].enable)
        Serial.print("O");
      else {
        if((ThePlayer.y == i) && (ThePlayer.x == j))
          Serial.print("X");
        else
          Serial.print(" ");
      }
    }
    Serial.print("\n");
  }
  Serial.print("\nMischief managed!\n");
}

void displayHelp() {
  Serial.print("\n\nThis is a text adventure game like in the 80's\n");
  Serial.print("Type your command in the serial terminal and ensure you end the command with a carriage return\n");
  Serial.print(descEntrance);
  promptDate();  
}

void displayInventory() { 
  Serial.print("You carry:\n");
  switch(ThePlayer.item) {
    case SWISS_KNIFE:
      Serial.print("a Swiss Army Knife\n");
      break;

    case SWORD:
      Serial.print("a Beautiful Elfic Sword\n");
      break;

    case NO_ITEM:
      Serial.print("nothing, except your heavy armor...\n");
      break;
  }
}

void handlePick() {
  if(getCell().item == NO_ITEM) {
    Serial.print("Nothing to pick, look better next time\n");
    return;
  }
  
  if(!ThePlayer.hasNothing()) {
    Serial.print("You already carry something. With that heavy armor, you don't want more!\n");
    return;
  }

  pickItem();
}

void pickItem() {
  ThePlayer.item = getCell().item;
  getCell().item = NO_ITEM;
  Serial.print("You picked:\n");
  printItem(ThePlayer.item);
}

void handleDrop() {
  if(ThePlayer.hasNothing()) {
    Serial.print("Nothing to drop, you don't carry anything\n");
    return;
  }

  if(getCell().item == NO_ITEM) {
    dropItem();
  }
  else {
    Serial.print("There's already something here. We don't want mess up the hotel with junk!\n");
  }
}



void printItem(uint8_t item) {
  if(item == SWISS_KNIFE)
    Serial.print("Swiss army knife\n");
  else
    Serial.print("Elfic Sword\n"); 
}


void dropItem() {
  Serial.print("You drop your ");
  printItem(ThePlayer.item);
  getCell().item = ThePlayer.item;
  ThePlayer.item = NO_ITEM;
}     

void lookAround() {
  if(getCell().item != NO_ITEM) {
    Serial.print("You look around and see:\n");
    printItem(getCell().item);
  }
  else
    Serial.print("You look around and see nothing on the floor\n");
}



// types text on the console like a typewriter plus carriage return ding sound at the end
// (sounds on the SD card)
void typeThis(const char *str, bool fast) {
  for(int i = 0 ; i < strlen(str) ; i++) {
    Serial.print(str[i]);
    if(!SwitchAction()) {
      // play typewriter sound
      PlaySoundFx(TYPE_SND_FILE);
      if(!fast)
        delay(20 + random(100));   // adjust
      else
        delay(random(20));   // adjust
    }
  }
  PlaySoundFx(END_SND_FILE);
}

void gameOver() {
  // play sound from the SD card to indicate game over
  Serial.print("\n\n\n\n\n\n\n");
  char end[] = {"#####  GAME OVER   #####"};
  typeThis(end);
  delay(500);
  PlaySoundFx(GAMEOVER_SND_FILE);
  Serial.print("\n\nand you fall in limbos, screaming, the ballrogue hanging from your masculine attributes...\n");
  delay(500);
  int i = 0;
  while(1) {
    Serial.print("A");
    delay(50);
    if(i > random(100)) {
      Serial.print(" BONK AOUCH FUCK THAT!!!!\n");
      PlaySoundFx(TYPE_SND_FILE);
      i = 0;
    }
    i++;
  }
}


void gameWin() {
  Serial.print("\n\n\n\n\n");
  PlaySoundFx(SECRET_DOOR2_SND_FILE);
  typeThis("You found the exit of LaMoria Hotel, you're finally Free - CONGRATULATIONS\n", FAST);
  char end[] = {"#####  YOU WIN !!!   #####"};
  typeThis(end);
  delay(500);
  typeThis("\n\nBefore you leave that place behind you, you pass a wide sign on which you read:\n", FAST);
  typeThis(" http://www.plecterlabs.com/Media/Fun/2019-Challenge/blink182.zip \n");
  while(1);
}



/////////////////////////////////////////////////////////////////////////////////
// Shared Routine to parse a command / parameter string
uint8_t ProcessSerial(char *line) {
  char dictStr[80];
  uint8_t action = CMD_UNKNOWN;

  if(!strlen(line))
    return CMD_BLANK;  
    
  strlwr(line);
  // Just a straight browsing of the commands, there aren't many
  for(int i = 0 ; i < NELEMS(CommandDictionnary) ; i++) {
    strcpy(dictStr, CommandDictionnary[i].cmdStr);
    strlwr(dictStr);
    if(!strcmp(line, dictStr)) {
      action = CommandDictionnary[i].cmd;
      break;
    }
  }
  
  if(action != CMD_UNKNOWN) {
    Serial.println(dictStr);
  }
  else
    Serial.print(line);
    Serial.print("\nThis is not the dro.. errr.... command you are looking for. Move along!\n");

  actionNew = true;
  return(action);
}



// Populate with your own sound playback routine if your board has this capability
bool PlaySoundFx(char *filename) {
  return true;
}

bool SwitchAction() {
  return(!digitalRead(PIN_ACT) || !digitalRead(PIN_AUX));
}
