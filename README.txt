==================================================================================================================================

HexMatch - README

Overview:

HexMatch is a memory and reaction time testing game implemented on a Raspberry Pi 4. 
The game challenges players to match randomly generated hexadecimal symbols/characters displayed on the 
TM1638 module with a target symbol on the 1-bit 7-segment LED display. 
The objective is to successfully complete 10 matches in a row.

==================================================================================================================================

Hardware Setup:

TM1638 Module:
VCC: Physical/Board pin 1 (3v3)
GND: Physical/Board pin 9 (GND)
STB: Physical/Board pin 15 (GPIO/BCM pin 22)
CLK: Physical/Board pin 40 (GPIO/BCM pin 21)
DIO: Physical/Board pin 11 (GPIO/BCM pin 17)

----------------------------------------------------------------------------------------------------------------------------------

TM1637 Module:
CLK: Physical/Board pin 5 (Wiring Pi pin 9)
DIO: Physical/Board pin 29 (Wiring Pi pin 21)
VCC: Physical/Board pin 17 (3v3)
GND: Physical/Board pin 39

----------------------------------------------------------------------------------------------------------------------------------

1-bit LED Display:
Segments A-G: Physical/Board pins 8, 10, 12, 16, 18, 22, 24 (Wiring Pi pins 15, 16, 1, 4, 5, 6, 10)
+: Physical/Board pin 20 (GND)

----------------------------------------------------------------------------------------------------------------------------------

Green, Red, Yellow LED:
GND: Physical/Board pin 34
Green LED: Physical/Board pin 19 (Wiring Pi pin 12)
Red LED: Physical/Board pin 21 (Wiring Pi pin 13)
Yellow LED: Physical/Board pin 23 (Wiring Pi pin 14)

----------------------------------------------------------------------------------------------------------------------------------

Momentary Switch:
VCC: Physical/Board pin 17 (3v3)
GND: Physical/Board pin 14
Signal: Physical/Board pin 31 (Wiring Pi pin 22)

==================================================================================================================================

Required Software Libraries:

wiringPi library
Installation: 
Pre-installed with standard Raspbian systems.

----------------------------------------------------------------------------------------------------------------------------------

bcm2835 library & header file
Installation:
tar zxvf bcm2835-1.73.tar.gz
cd bcm2835-1.73
./configure
make
sudo make check
sudo make install

----------------------------------------------------------------------------------------------------------------------------------

tm1638 header file
Installation:
git clone https://github.com/mjoldfield/pi-tm1638.git
autoreconf -vfi
./configure
make
sudo make install

----------------------------------------------------------------------------------------------------------------------------------

tm1637 header file
Installation:
sudo su
mv tm1637.h /usr/include/
ls /usr/include
exit

==================================================================================================================================

Building and Running the Game:

Open a terminal and navigate to the directory containing the game source code.
Compile the code using the following commands:
gcc ./game.c -o game -lwiringPi -ltm1638 -lbcm2835

Run the game:
./game

Press any button on the TM1638 module to start the game.

==================================================================================================================================

Gameplay:

The game displays 8 randomly chosen unique hexadecimal symbols/characters on the TM1638 module.
Match the target symbol/character displayed on the 1-bit LED display by pressing the corresponding button on the TM1638 module.
Repeat the matching process 10 times in a row without failure to complete the game successfully.

==================================================================================================================================

Game Controls:
Press any button on the TM1638 module to initiate the game.
During the game, press the corresponding buttons to match the displayed hexadecimal symbols/characters.
In case of failure or successful completion, the game can be restarted by pressing the momentary switch.

==================================================================================================================================

Known Issues and Troubleshooting:
Ensure proper hardware connections according to the provided hardware setup.
Check that the required software libraries are installed correctly.
If facing issues, restart the Raspberry Pi and run the game again.

==================================================================================================================================

Author:
HexMatch game was developed by Garrett Engelder.
Contacts: garrett.engelder@hotmail.com

==================================================================================================================================