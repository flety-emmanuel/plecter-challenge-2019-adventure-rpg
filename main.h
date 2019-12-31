#ifndef _MAIN_H
#define _MAIN_H
/////////////////////////////////////////////////////////////
//
//    main.h
//
/////////////////////////////////////////////////////////////

#include <Arduino.h>

// Define your 2 switches on the arduino GPIO of your choice
#define PIN_ACT     2
#define PIN_AUX     3

// Usual things
#define VERSION   "Plecter Labs Challenge\0"
#define DATE      "HELLO XMAS - 12/2019\0"

#define MAX_PATH_LEN      80
#define MAX_SERIAL_LEN    80

#define GAME_PATH                     "lamoriahotel"
#define INTRO_SND_FILE                "intro.wav"
#define TYPE_SND_FILE                 "sntext.wav"
#define END_SND_FILE                  "snend.wav"
#define GAMEOVER_SND_FILE             "gameover.wav"
#define SECRET_DOOR1_SND_FILE         "open.wav"
#define SECRET_DOOR2_SND_FILE         "gate.wav"
#define EXIT_DOOR_SND_FILE            "close.wav"


//////////////////////////////////////////
// Extension of files
#define SND_EXT     ".wav"
#define CFG_EXT     ".txt"
#define BMP_EXT     ".bmp"

typedef union uWord {
  int Value;
  unsigned char Val[sizeof(int)];
} Word;


#endif
