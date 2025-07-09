#pragma once
#include <Arduino.h>
#include <Ewma.h>
#include <math.h>

const float DEFAULT_ALPHA = 0.8;
const float DEFAULT_DEAD_ZONE = 0.55;

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
      double alpha = DEFAULT_ALPHA,
      float deadZone = DEFAULT_DEAD_ZONE) : filter(new Ewma(alpha)), deadZone(deadZone)
  {
  }

  ~WCalculator()
  {
    delete filter;
  }

  float getWInRadBySec(float valueDiff, float deltaTimeMs, bool applyFilter=true)
  {
    if (deltaTimeMs <= 0)
      return 0.0f;

    // Convierte la diferencia de unidades crudas a radianes
    float deltaRadians = valueDiff * RAW_TO_RADIANS;

    // Convierte el delta time a segundos
    float deltaTimeSec = deltaTimeMs / 1000.0f;

    // Calcula la velocidad angular en radianes por segundo
    float wInRadBySec = deltaRadians / deltaTimeSec;

    if(applyFilter) {
      wInRadBySec = filter->filter(wInRadBySec);
    }

    return fabs(wInRadBySec) <= deadZone ? 0.0f : wInRadBySec;
  }
};