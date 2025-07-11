#pragma once
#include <Arduino.h>

void initSerial(unsigned long baud);
void info(String msg);
void error(String msg);
void warn(String msg);
