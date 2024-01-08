#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <wiringPi.h>
#include <tm1637.h>
#include <bcm2835.h>
#include <tm1638.h>

#define TM1638_STB 22
#define TM1638_CLK 21
#define TM1638_DIO 17

#define TM1637_CLK 9
#define TM1637_DIO 21

#define Segment_A 15
#define Segment_B 16
#define Segment_C 1
#define Segment_D 4
#define Segment_E 5
#define Segment_F 6
#define Segment_G 10

#define RESET 28

#define STATUS_LED_SUCCESS 12
#define STATUS_LED_FAILURE 13
#define STATUS_LED_PLAYING 14

// Function declarations
void setup();
void chooseRandomChars();
void displayChosenHex();
void update1BitLED();
void checkButtonPress();
void gameCompletionCheck();
void gameCompletionFailure();
void displayTimer();
void restartProgram();

// Global variables
bool gameInProgress = true;
char chosenHexChars[8];
char targetHex;
int button;
int successfulPresses;
int repeat = 0;
time_t start_time;
tm1638_p tm1638_Module;

int main() 
{
  if (wiringPiSetup() == -1) 
  {
    printf("Unable to initialize wiringPi.\n");
    return 1;
  }

  if (!bcm2835_init())
  {
    printf("Unable to initialize bcm2835.\n");
    return 1;
  }

  // Initialize TM1637 module pins
  TMsetup(TM1637_CLK, TM1637_DIO);
  TMsetBrightness(7);

  // Initialize the TM1638 module pins
  tm1638_Module = tm1638_alloc(TM1638_DIO, TM1638_CLK, TM1638_STB);

  if (!tm1638_Module) 
  {
    printf("Unable to initialize TM1638 module.\n");
    return 1;
  }

  setup();

  // Seed the random number generator
  srand(time(NULL));

  // Turn off all status LEDs
  digitalWrite(STATUS_LED_PLAYING, LOW);
  digitalWrite(STATUS_LED_SUCCESS, LOW);
  digitalWrite(STATUS_LED_FAILURE, LOW);

  // Turn off tm1638 8-bit 7 segment LED display
  tm1638_send_cls(tm1638_Module);

  // Turn off 1 bit seven segment LED display
  digitalWrite(Segment_A, LOW);
  digitalWrite(Segment_B, LOW);
  digitalWrite(Segment_C, LOW);
  digitalWrite(Segment_D, LOW);
  digitalWrite(Segment_E, LOW);
  digitalWrite(Segment_F, LOW);
  digitalWrite(Segment_G, LOW);

  // Clear timer
  TMclear();

  // Read button input and assign as button
  button = tm1638_read_8buttons(tm1638_Module);

  // Press any to start game
  tm1638_set_7seg_text(tm1638_Module, "PressAny", 0x00);
  while(button == 0)
  {
    button = tm1638_read_8buttons(tm1638_Module);
  }
  delay(200);

  // Start game
  gameInProgress = true;

  // Reset successful presses
  successfulPresses = 0;

  // Generate and display 8 random characters
  chooseRandomChars();

  // Display the chosen Hex characters on the TM1638 module
  displayChosenHex();

  // Turn on the playing LED indicator
  digitalWrite(STATUS_LED_PLAYING, HIGH);

  // Choose a target hex from the chosenHexChars array
  targetHex = chosenHexChars[rand() % 8];

  // Update the 1-bit LED with the target hex value
  update1BitLED();

  // Mark start time
  time(&start_time);


  // Primary game loop
  while (gameInProgress) 
  {
    int m;
    // Loop 8 times once for each LED on the tm1638 display
    for(int j = 0; j < 8 && repeat == 0; j++)
    {
      // Bit shift by increasing value J to display the Jth LED at bit m
      m = 128 >> j;
      tm1638_set_8leds(tm1638_Module, m, 0);

      // Check for reset button press
      if (digitalRead(RESET) == HIGH)
      {
        restartProgram();
      }

      // Check for user input and return results
      checkButtonPress();

      // Display stopwatch timer
      displayTimer();
    }

    // Loop 8 times once for each LED on the tm1638 display
    for(int j = 0; j < 8 && repeat == 0; j++)
    {
      // Bit shift by increasing value J to display the Jth LED at bit m
      m = 1 << j;
      tm1638_set_8leds(tm1638_Module, m, 0);

      // Check for reset button press
      if (digitalRead(RESET) == HIGH)
      {
          restartProgram();
      }

      // Check for user input and return results
      checkButtonPress();

      // Display stopwatch timer
      displayTimer();
    }
  }

  while (!gameInProgress)
  {    
    if (digitalRead(RESET) == HIGH)
    {
      restartProgram();
    }
  }
}

void setup() 
{
  // Set up 1-bit LED display
  pinMode(Segment_A, OUTPUT);
  pinMode(Segment_B, OUTPUT);
  pinMode(Segment_C, OUTPUT);
  pinMode(Segment_D, OUTPUT);
  pinMode(Segment_E, OUTPUT);
  pinMode(Segment_F, OUTPUT);
  pinMode(Segment_G, OUTPUT);

  // Set up reset button
  pinMode(RESET, INPUT);

  // Set up status LEDs
  pinMode(STATUS_LED_SUCCESS, OUTPUT);
  pinMode(STATUS_LED_FAILURE, OUTPUT);
  pinMode(STATUS_LED_PLAYING, OUTPUT);
}

void chooseRandomChars() 
{
  // Initialize an array of hex characters
  char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'b', 'C', 'd', 'E', 'F'};
  
  // Initialize an array to keep track of chosen indices
  bool chosenIndices[16] = {false};

  for (int i = 0; i < 8; ++i) 
  {
    int randomIndex;

    do 
    {
      // Generate a random index
      randomIndex = rand() % 16;
    } 
    while (chosenIndices[randomIndex]);

    // Mark the index as chosen
    chosenIndices[randomIndex] = true;

    // Copy the chosen character to chosenHexChars
    chosenHexChars[i] = hexChars[randomIndex];
  }
}

void displayChosenHex()
{
  tm1638_set_7seg_text(tm1638_Module, &chosenHexChars[0], 0x00);
}

void update1BitLED() 
{
  switch (targetHex) 
  {
    case '0':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, LOW);
        break;
    case '1':
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, LOW);
        break;
    case '2':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, LOW);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, HIGH);
        break;
    case '3':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, HIGH);
        break;
    case '4':
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case '5':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case '6':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case '7':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, LOW);
        break;
    case '8':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case '9':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, LOW);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case 'A':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case 'b':
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case 'C':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, LOW);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, LOW);
        break;
    case 'd':
        digitalWrite(Segment_A, LOW);
        digitalWrite(Segment_B, HIGH);
        digitalWrite(Segment_C, HIGH);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, LOW);
        digitalWrite(Segment_G, HIGH);
        break;
    case 'E':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, LOW);
        digitalWrite(Segment_D, HIGH);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
    case 'F':
        digitalWrite(Segment_A, HIGH);
        digitalWrite(Segment_B, LOW);
        digitalWrite(Segment_C, LOW);
        digitalWrite(Segment_D, LOW);
        digitalWrite(Segment_E, HIGH);
        digitalWrite(Segment_F, HIGH);
        digitalWrite(Segment_G, HIGH);
        break;
  }
}

void displayTimer() 
{
  time_t end_time;
  time(&end_time);
  double elapsed_time = difftime(end_time, start_time);
  TMshowNumber(elapsed_time, 0b10000000, false, 4, 0);
}

void checkButtonPress() 
{
  button = tm1638_read_8buttons(tm1638_Module);
  switch(button)
  {
    case 128:
        if (chosenHexChars[0] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 64:
        if (chosenHexChars[1] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 32:
        if (chosenHexChars[2] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 16:
        if (chosenHexChars[3] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 8:
        if (chosenHexChars[4] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 4:
        if (chosenHexChars[5] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 2:
        if (chosenHexChars[6] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
    case 1:
        if (chosenHexChars[7] == targetHex)
        {
          successfulPresses++;
          gameCompletionCheck();
        }
        else
        {
          gameCompletionFailure();  
        }
        break;
  }
}

void gameCompletionCheck()
{
  if (successfulPresses == 10) 
  {
    // Stop game loop
    gameInProgress = false;
    repeat = 1;

    // Turn off 8-bit LED
    tm1638_set_8leds(tm1638_Module, 0, 0);

    // Set game status to SUCCESS
    digitalWrite(STATUS_LED_SUCCESS, HIGH);
    digitalWrite(STATUS_LED_PLAYING, LOW);
    digitalWrite(STATUS_LED_FAILURE, LOW);

    // Display success on tm1638 display
    tm1638_set_7seg_text(tm1638_Module, "SUCCESS", 0x00);

    // Turn off 1 bit seven segment LED display
    digitalWrite(Segment_A, LOW);
    digitalWrite(Segment_B, LOW);
    digitalWrite(Segment_C, LOW);
    digitalWrite(Segment_D, LOW);
    digitalWrite(Segment_E, LOW);
    digitalWrite(Segment_F, LOW);
    digitalWrite(Segment_G, LOW);
  } 
  else 
  {
    delay(200);

    // Generate and display 8 random characters
    chooseRandomChars();

    // Display the chosen Hex characters on the TM1638 module
    displayChosenHex();


    // Choose new a target hex from the chosenHexChars array
    targetHex = chosenHexChars[rand() % 8];

    // Update the 1-bit LED with the target hex value
    update1BitLED();
  }
} 

void gameCompletionFailure()
{
  // Stop game loop
  gameInProgress = false;
  repeat = 1;

  // Turn off 8-bit LED
  tm1638_set_8leds(tm1638_Module, 0, 0);

  // Set game status to FAILURE
  digitalWrite(STATUS_LED_SUCCESS, LOW);
  digitalWrite(STATUS_LED_PLAYING, LOW);
  digitalWrite(STATUS_LED_FAILURE, HIGH);

  // Display failure on tm1638 display
  tm1638_set_7seg_text(tm1638_Module, "FAILURE", 0x00);

  // Turn off 1 bit seven segment LED display
  digitalWrite(Segment_A, LOW);
  digitalWrite(Segment_B, LOW);
  digitalWrite(Segment_C, LOW);
  digitalWrite(Segment_D, LOW);
  digitalWrite(Segment_E, LOW);
  digitalWrite(Segment_F, LOW);
  digitalWrite(Segment_G, LOW);
}

void restartProgram()
{
  const char* command = "./game";
  system(command);
  exit(0);
}
