#pragma once
#include <Arduino.h>
#include <Ewma.h>
#include <math.h>

class AngularVelocityCalculator
{

private:
  // Factor de conversión de unidades crudas del encoder (12-bit, 4096 pasos) a radianes.
  // 2 * PI radianes en una rotación completa.
  static constexpr float RAW_TO_RADIANS = (2.0f * M_PI) / 4096.0f;

  Ewma *filter;
  float deadZone;

  public : AngularVelocityCalculator(
    double alpha = 0.8,
    float deadZone = 0.45) : filter(new Ewma(alpha)), deadZone(deadZone)
  {
  }

  ~AngularVelocityCalculator()
  {
    delete filter;
  }

  float perform(float valueDiff, float deltaTimeMs)
  {
    if(deltaTimeMs <= 0) return 0.0f;

    // Convierte la diferencia de unidades crudas a radianes
    float deltaRadians = valueDiff * RAW_TO_RADIANS;

    // Calcula la velocidad angular en radianes por segundo
    float value = deltaRadians / (deltaTimeMs / 1000.0f);

    value = filter->filter(value);

    return abs(value) <= deadZone ? 0.0f : value;
  }
};