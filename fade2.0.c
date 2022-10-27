#include <wiringPi.h>
#include <pigpio.h>
#include <stdio.h>
#include <softPwm.h>
#include <stdbool.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>

#define RED 24    // GPIO 24 (Pin 16)
#define GREEN 23  // GPIO 23 (Pin 18)

void askMode();
void blink2Hz();
void blink8Hz();
void turnON();
void turnOFF();
bool kbhit();     // Detect keypresses

int choice = 0;

int main()
{
  wiringPiSetupGpio();  // Initialises Wiring Pi library
  gpioInitialise();     // Initialises Pi GPIO library

  if(gpioInitialise() < 0)
  {
    printf("GPIO initialisation failed!\n");
    return(0);
  }

  else
  {
    ("GPIO initialisation successful!\n");
  }

  pinMode(RED, OUTPUT);       // Sets GPIO 24 (Pin 16) as output mode for Wiring Pi library
  pinMode(GREEN, OUTPUT);     // Sets GPIO 23 (Pin 18) as output mode for Wiring Pi library
  gpioSetMode(RED, OUTPUT);   // Sets GPIO 24 (Pin 16) as output mode for Pi GPIO library
  gpioSetMode(GREEN, OUTPUT); // Sets GPIO 23 (Pin 18) as output mode for Pi GPIO library

  while(1)
  {
    askMode();  // Prompts user for input 0, 1, 2 or 3

    if(choice == 0 || choice == 1 || choice == 2 || choice == 3)
    {
      switch(choice)
      {
        case 0: // Turns OFF both LEDs
          turnOFF();
          break;

        case 1: // Turns ON both LEDs
          turnON();
          break;

        case 2: // Blinks both LEDs at 2Hz
          while(choice == 2)
          {
            blink2Hz();

            if(kbhit()) // Detects keyboard input to interrupt blinking
            {
              int k = getchar();
              if(k == '0')
              {
                turnOFF();
                break;
              }
              if(k == '1')
              {
                turnON();
                break;
              }
              if(k == '2')
              {
                blink2Hz();
              }
	            if(k == '3')
              {
                blink8Hz();
              }
            }
          }

        case 3: // Blinks RED at 8Hz, GREEN at 2Hz and 4 times dimmer
          while(choice == 3)
          {
            blink8Hz();

            if(kbhit()) // Detects keyboard input to interrupt blinking
            {
              int k = getchar();
              if(k == '0')
              {
                turnOFF();
                break;
              }
              if(k == '1')
              {
                turnON();
                break;
              }
              if(k == '2')
              {
		            blink2Hz();
              }
              if(k == '3')
              {
                blink8Hz();
	            }
            }
          }
          break;

        default:
          break;
      }
    }

    else if(choice != 0 || choice != 1 || choice != 2 || choice != 3)
    {
      printf("\nInvalid input!\n");
      break;
    }
  }

return 0;
}

void askMode() // Prompts user for input
{
  printf("\nEnter mode 0, 1, 2, 3: ");
  scanf("\n%d", &choice);
}

void blink2Hz()
{
  // First cycle : 0.5s, both LEDs turn ON for 250ms
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  delay(250);

  // Second cycle : 0.5s, both LEDs turn OFF for 250ms
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  delay(250);
}

void blink8Hz()
{
  /***************************/
  /*Half second loop function*/
  /***************************/

  // First cycle: 0.25s, GREEN turns ON, RED blinks 2 times 
  gpioWrite(RED, 1);
  gpioPWM(GREEN, 32); // 255 max PWM * 12.5% duty cycle
  delay(62.5);
  gpioWrite(RED, 0);
  delay(62.5);
  gpioWrite(RED, 1);
  delay(62.5);
  gpioWrite(RED, 0);
  delay(62.5);

  // Second cycle: 0.25s, GREEN turns OFF, RED blinks 2 times 
  gpioWrite(RED,1);
  gpioPWM(GREEN, 0); // 0 PWM, 0% duty cycle
  delay(62.5);
  gpioWrite(RED, 0);
  delay(62.5);
  gpioWrite(RED, 1);
  delay(62.5);
  gpioWrite(RED, 0);
  delay(62.5);
}

void turnON()
{
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
}

void turnOFF()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
}

bool kbhit()
{
  struct termios original;
  tcgetattr(STDIN_FILENO, &original);
  struct termios term;
  memcpy(&term, &original, sizeof(term));
  term.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  int characters_buffered = 0;
  ioctl(STDIN_FILENO, FIONREAD, &characters_buffered);
  tcsetattr(STDIN_FILENO, TCSANOW, &original);
  bool pressed = (characters_buffered != 0);
  return pressed;
}
