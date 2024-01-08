# tm1637.h
tm1637.h is a Raspberry Pi C library for TM1637 seven segments LED displays.

## Compilation
Include tm1636 in the beginning of your program, e.g. :

```
#include "tm1637.h"
```

Compile with wiringpi library, e.g. (makefile) :

```
PROGRAM=clock
$(PROGRAM) : $(PROGRAM).c
	gcc -s -Wall -O2 $(PROGRAM).c -o $(PROGRAM) -lwiringPi
```

## Functions

#### TMsetup(uint8_t pinClk, uint8_t pinDIO)

Initialize a TM1637Display object, setting the clock and data pins
(uses [wiringpi numbering scheme](https://pinout.xyz/pinout/wiringpi#)).

- **pinClk** : digital pin connected to the clock pin of the module
- **pinDIO** : digital pin connected to the DIO pin of the module.

### TMsetBrightness(uint8_t brightness)
Sets the brightness of the display. Takes effect when a command is given to change the data being displayed.

- **brightness** : a number from 0 (lower brightness) to 7 (highest brightness).

### TMsetSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
Display arbitrary data on the module.

This function receives raw segment values as input and displays them. The segment data is given as a byte array, each byte corresponding to a single digit. Within each byte, bit 0 is segment A, bit 1 is segment B etc. The function may either set the entire display or any desirable part on its own. The first digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length argument is the number of digits to be set. Other digits are not affected.

- **segments** : an array of size **length** containing the raw segment values
- **length** : the number of digits to be modified
- **pos** : the position from which to start the modification (0 - leftmost, 3 - rightmost).

#### TMclear()

Clear the display.

#### TMshowNumber(int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos)
Displays a decimal number, with dot control.

Displays the given argument as a decimal number. The dots between the digits (or colon) can be individually controlled.

- **num** : the number to be shown
- **dots** : dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot between the digits (or colon mark, as implemented by each module). i.e. For displays with dots between each digit :
  * 0000 (0)
  * 0.000 (0b10000000)
  * 00.00 (0b01000000)
  * 000.0 (0b00100000)
  * 0.0.0.0 (0b11100000)
  * 00:00 (0b01000000) (for displays with just a colon)
  * 0.0:0.0 (0b11100000) (for displays with dots and a colon).
- **leading_zero** : when true, leading zeros are displayed. Otherwise unnecessary digits are blank
- **length** : the number of digits to set
- **pos** : the position of first digit (0 - leftmost, 3 - rightmost)
	
#### TMshowDouble(double x)
Displays a double as 00.00

## Examples

The provided examples use the following circuit :

![](../circuit.png)

## Credits
- Author : Pierre Brial, 2021-01-24
- Based on Arduino [TM1637 library](https://github.com/avishorp/TM1637) by avishorp
- Depends on [Wiring Pi library](http://wiringpi.com/) by Gordon Henderson.
