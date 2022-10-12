#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <stdbool.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>

#define RED 24
#define GREEN 23


void askMode();
void blink();
void turnON();
void turnOFF();
bool kbhit(); //detect keypress


int choice = 0;

int main()
{
  wiringPiSetup();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);



  while(1)
  {
    askMode();

    if(choice == 0 || choice == 1 || choice == 2 || choice == 3)
    {
      switch(choice)
      {

        case 0:
          turnOFF();
        break;

        case 1:
          turnON();
        break;

        case 2:
          while(choice == 2)
          {
            blink();
            if(kbhit())
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
                blink();
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
      printf("\nINVALID\n");
      break;
    }

  }    

return 0;

}


void askMode()
{
  printf("\nEnter mode 0, 1, 2, 3: ");
  scanf("\n%d", &choice);

}

void blink()
{
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  delay(500);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  delay(500); 
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
