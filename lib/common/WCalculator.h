#pragma once
#include <Arduino.h>
#include <Ewma.h>
#include <math.h>

class WCalculator
{

private:
  // Factor de conversión de unidades crudas del encoder (12-bit, 4096 pasos) a radianes.
  // 2 * PI radianes en una rotación completa.
  static constexpr float RAW_TO_RADIANS = (2.0f * M_PI) / 4096.0f;

  Ewma *filter;
  float deadZone;

public:
  WCalculator(
      double alpha = 0.8,
      float deadZone = 0.45) : filter(new Ewma(alpha)), deadZone(deadZone)
  {
  }

  ~WCalculator()
  {
    delete filter;
  }

  float getWInRadBySec(float valueDiff, float deltaTimeMs)
  {
    if (deltaTimeMs <= 0)
      return 0.0f;

    // Convierte la diferencia de unidades crudas a radianes
    float deltaRadians = valueDiff * RAW_TO_RADIANS;

    // Convierte el delta time a segundos
    float deltaTimeSec = deltaTimeMs / 1000.0f;

    // Calcula la velocidad angular en radianes por segundo
    float wInRadBySec = deltaRadians / deltaTimeSec;

    wInRadBySec = filter->filter(wInRadBySec);

    return abs(wInRadBySec) <= deadZone ? 0.0f : wInRadBySec;
  }
};