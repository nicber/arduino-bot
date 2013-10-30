
#include "sensor_luz.h"
#include "sensor_ultrason.h"

namespace hw
{
namespace sensor
{
const int sensor_luz::interrupt = 0;

const int sensores_ultrasonicos::interrupt = 0;
const int sensores_ultrasonicos::pin_in = 0;
const util::micros_t sensores_ultrasonicos::no_data = 0;
/**
 * Largo del tatami = 2m
 * --------------------- * 1.000.000 (para convertirlo a usec) * 2 (es ida y vuelta)
 * 340m/s (vel. sonido)
 * 
 * Escribo '2.0' para que se calcule sin descartar los decimales
 * porque se convierte en una expresion float.
 */
const unsigned int sensores_ultrasonicos::timeout = (2.0 / 340) * 1E6 * 2;
}
}