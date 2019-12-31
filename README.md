# plecter-challenge-2019-adventure-rpg

This is one of the puzzles from the Plecter Labs xmas challenge 2019 in the shape of a classic text aventure / RPG formerly compiled to run on the Crystal Focus X (v10). It has been adapted to run on a more standard arduino.
Based on the size of the maze and the crude programming, it make a quite extensive use of RAM which doesn't allow to compile it on a Arduino Nano/Uno/Mega but rather on a modern 32 bit platform such as the Arduino Zero, MKR or an Adafruit M0/M4.
I refactored the code to be as close as possible to the Arduino standard. It could probably run on a regular 8 bit duino by storing in RAM only the cells that have a special function, character or item but for now, you'll need a beefy board to play :-)
