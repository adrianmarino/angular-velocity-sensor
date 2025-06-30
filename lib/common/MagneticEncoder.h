#pragma once
#include "AS5600Sensor.h"
#include <Arduino.h> // Necesario para millis() y Serial.println()
#include <math.h>    // Necesario para abs()

typedef void (*AngularVelocityChangeCallback)(int rawValue, float diffRaw, float deltaTimeMs);

const int DEFAULT_THRESHOULD = 1; // Umbral por defecto: 1 unidad cruda (aprox. 0.087 grados)
                                  // Este umbral se aplica al cambio de ángulo crudo.

const int DEFAULT_SAMPLE_INTERVAL_MS = 80;

class MagneticEncoder
{

private:
  AS5600Sensor *sensor;               // Puntero al objeto AS5600Sensor subyacente
  uint16_t previousRawValue;          // Almacena el valor crudo del ángulo de la lectura anterior exitosa
  unsigned long previousUpdateTimeMs; // Almacena el tiempo en ms de la lectura anterior exitosa
  unsigned long sampleIntervalMs;     // Intervalo de muestreo deseado en milisegundos
  uint16_t valueChangeThreshold;      // Umbral de cambio en unidades crudas para activar el callback

  AngularVelocityChangeCallback angularVelocityChangeCallback; // Función a llamar cuando la velocidad angular cambia

public:
  // Constructor de la clase MagneticEncoder
  MagneticEncoder(
      AS5600Sensor *sensor,
      AngularVelocityChangeCallback cb, // Ahora espera un callback para int (valor crudo) y float (velocidad angular)
      unsigned long sampleIntervalMs = DEFAULT_SAMPLE_INTERVAL_MS,
      uint16_t threshold = DEFAULT_THRESHOULD) : sensor(sensor), // Inicializa el puntero al sensor
                                                 sampleIntervalMs(sampleIntervalMs),
                                                 angularVelocityChangeCallback(cb), // Asigna la nueva función de callback
                                                 valueChangeThreshold(threshold),
                                                 previousRawValue(0), // Inicializa el valor anterior a 0
                                                 previousUpdateTimeMs(0)
  {
  } // Inicializa el tiempo anterior a 0

  // Inicializa el sensor de la rueda
  bool begin()
  {
    if (sensor->begin())
    {                                        // Intenta inicializar el sensor AS5600
      previousRawValue = sensor->getValue(); // Obtiene el valor inicial
      previousUpdateTimeMs = millis();       // Registra el tiempo de inicio
      return true;
    }
    else
    {
      Serial.println("MagneticEncoder: Fallo al inicializar el AS5600 subyacente.");
      return false;
    }
  }

  // Actualiza el estado del sensor de la rueda y calcula la velocidad angular
  void update()
  {
    unsigned long currentTimeMs = millis(); // Obtiene el tiempo actual una vez

    // Verifica si ha pasado el intervalo de muestreo deseado
    if ((currentTimeMs - previousUpdateTimeMs) >= sampleIntervalMs)
    {
      sensor->update(); // Realiza una nueva lectura del sensor AS5600

      if (sensor->isSuccessful())
      {
        uint16_t currentRawValue = sensor->getValue();

        // Calcula la diferencia de ángulo crudo, manejando el "wrap-around" (0-4095)
        int diffRaw = (int)currentRawValue - (int)previousRawValue;
        if (diffRaw > 2048)
        {                  // Si el salto es grande y positivo (ej. 4000 -> 100)
          diffRaw -= 4096; // Ajusta para el wrap-around
        }
        else if (diffRaw < -2048)
        {                  // Si el salto es grande y negativo (ej. 100 -> 4000)
          diffRaw += 4096; // Ajusta para el wrap-around
        }

        // Calcula la diferencia de tiempo en milisegundos
        unsigned long deltaTimeMs = currentTimeMs - previousUpdateTimeMs;

        // Solo calcula y reporta la velocidad angular si ha habido un cambio significativo
        // basado en el umbral de cambio de ángulo crudo.
        if (abs(diffRaw) > valueChangeThreshold)
        {
          angularVelocityChangeCallback(currentRawValue, diffRaw, deltaTimeMs);
        }

        // Actualiza los valores anteriores para la próxima iteración,
        // independientemente de si se superó el umbral. Esto asegura que
        // la velocidad angular siempre se calcule desde la última lectura exitosa.
        previousRawValue = currentRawValue;
        previousUpdateTimeMs = currentTimeMs;
      }
      else
      {
        Serial.println("MagneticEncoder: ERROR: Fallo en la lectura del AS5600.");
        // Si la lectura del sensor falla, previousRawValue y previousUpdateTimeMs no se actualizan.
        // Esto significa que la próxima lectura exitosa calculará la velocidad desde el último punto bueno.
      }
    }
  }
};