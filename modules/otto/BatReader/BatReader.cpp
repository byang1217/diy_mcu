/******************************************************************************
* Zowi Battery Reader Library
* 
* @version 20150831
* @author Raul de Pablos Martin
*
******************************************************************************/

#include "BatReader.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

BatReader::BatReader() {
}

double BatReader::readBatVoltage(void) {
	return 4.2;
}

double BatReader::readBatPercent(void) {
	return 100;
}

