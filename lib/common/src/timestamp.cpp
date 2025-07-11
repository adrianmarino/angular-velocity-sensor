#include "timestamp.h"


void syncClockTimeStamp(const long utcOffsetInSeconds)
{
  // Inicializa la configuración de tiempo. El segundo parámetro es el horario de verano.
  // Si no usas horario de verano, déjalo en 0.
  configTime(
      utcOffsetInSeconds,
      0,
      "pool.ntp.org",
      "time.nist.gov" // Segundo servidor opcional para redundancia
  );

  // Esperar a que el tiempo NTP se sincronice.
  // La función 'time(nullptr)' devuelve un timestamp Unix. Un valor válido es un número grande (> 10^9).
  // Si el tiempo no se sincroniza (por ejemplo, no hay internet), el bucle se podría quedar colgado.
  // Asegúrate de que el ESP32 tenga conexión a Internet para acceder al servidor NTP.
  Serial.print("Waiting for NTP time synchronization");
  time_t now = 0;
  while (now < 1000000000)
  { // Un timestamp Unix válido es > 10^9 segundos
    now = time(nullptr);
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nNTP time synchronized.");
}