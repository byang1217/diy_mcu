#include "US.h"

//#define US_ENABLE

//****** US ******//
US::US(){
}

US::US(int pinTrigger, int pinEcho){
#ifdef US_ENABLE
  US::init(pinTrigger,pinEcho);
#endif
}

void US::init(int pinTrigger, int pinEcho)
{
#ifdef US_ENABLE
  _pinTrigger = pinTrigger;
  _pinEcho = pinEcho;
  pinMode( _pinTrigger , OUTPUT );
  pinMode( _pinEcho , INPUT );
#endif
}

long US::TP_init()
{
#ifdef US_ENABLE
    digitalWrite(_pinTrigger, LOW);
    delayMicroseconds(2);
    digitalWrite(_pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pinTrigger, LOW);
    long microseconds = pulseIn(_pinEcho,HIGH,40000); //40000
    return microseconds;
#else
    return 40000;
#endif
}

float US::read(){
#ifdef US_ENABLE
  long microseconds = US::TP_init();
  long distance;
  distance = microseconds/29/2;
  if (distance == 0){
    distance = 999;
  }
  return distance;
#else
  return 100;
#endif
}
