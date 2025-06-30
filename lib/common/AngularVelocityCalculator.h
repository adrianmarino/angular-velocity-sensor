#pragma once
#include <Arduino.h>

class AngularVelocityCalculator
{

private:
  unsigned long previousUpdateTimeMs;

public:
  AngularVelocityCalculator(): previousUpdateTimeMs(0)
  {
  }

  float perform(
      float valueDiff,
      float deltaTimeMs)
  {
    if(deltaTimeMs > 0) {
      // Convierte la diferencia de unidades crudas a grados
      float deltaDegrees = valueDiff / 4096.0 * 360.0;

      // Calcula la velocidad angular en grados por segundo
      return deltaDegrees / (deltaTimeMs / 1000.0);
    }
    return 0.0;
  }
};