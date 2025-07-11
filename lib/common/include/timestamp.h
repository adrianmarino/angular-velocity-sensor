#pragma once
#include <micro_ros_platformio.h>
#include <time.h>

const long AR_UTC_TIME_OFFSET_IN_SECONDS = -3 * 3600;

/**
 * Configura el servidor NTP. Es CRUCIAL que el ESP32 sincronice su reloj con un servidor NTP para
 * que las marcas de tiempo de los mensajes de odometría (y cualquier otro sensor interno)
 * coincidan con el tiempo del sistema en tu PC (donde corre ROS).
 * Reemplaza "pool.ntp.org" con un servidor NTP más cercano a tu ubicación si lo deseas,
 * aunque pool.ntp.org suele funcionar bien.
 */
void syncClockTimeStamp(const long utcOffsetInSeconds = AR_UTC_TIME_OFFSET_IN_SECONDS);