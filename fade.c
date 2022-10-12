#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

#define RED 24
#define GREEN 23


void askMode();


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

        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);

        break;

        case 1:

        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, HIGH);

        break;
      }

    }
    else
    {
      askMode();
    }

  }    



}


void askMode()
{
  printf("\nEnter mode 0, 1, 2, 3: ");
  scanf("%d", &choice);

}





