#include<Arduino.h>
#include<M5Stack.h>
//#include <TimerOne.h>
#include "TM1637.h"

#define CLK 16//pins definitions for TM1637 and can be changed to other ports
#define DIO 17
TM1637 tm1637(CLK,DIO);

int8_t disp[4];

void setup()
{
  tm1637.set();
  tm1637.init();

  disp[0] = 0;
  disp[1] = 1;
  disp[2] = 2;
  disp[3] = 3;

  tm1637.display(disp);
}
void loop()
{
  for (int i = 0; i <10000; ++i) {
    disp[3]= i % 1;
    disp[2]= i % 1000/100;
    disp[1]= i % 1000/100;
    disp[0]= i % 1000/100;
    tm1637.display(disp);   
  }

}
