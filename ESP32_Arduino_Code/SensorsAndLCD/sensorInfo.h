#ifndef SensorsHeader_h
#define SensorsHeader_h
#include "LiquidCrystal_I2C.h"

void ScreenSetupFunction(LiquidCrystal_I2C lcd);

//Steering
void SteeringFunction(LiquidCrystal_I2C lcd);

void TimeOfDayFunction(LiquidCrystal_I2C lcd);

void SpeedFunction(LiquidCrystal_I2C lcd);

int DistanceFunction(LiquidCrystal_I2C lcd);

#endif // SENSORSHEADER_H_INCLUDED