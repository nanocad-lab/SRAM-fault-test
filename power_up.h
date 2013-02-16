#ifndef POWER_UP_H
#define POWER_UP_H

#include "mbed.h"

void powerUp(double voltage);
void powerDown();
void powerReset();
void debugPower();
void powerAdjust(double voltage);
void adjustSRAMVoltage(double voltage);
#endif