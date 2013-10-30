#pragma once

namespace util
{
typedef unsigned long millis_t;
typedef unsigned long micros_t;

const float mm_p_usec(340.0 / 1E6 * 1E3); //340m/s (vel. sonido) / 1E6 (convertido a m/usec) * 1E3 (conv. a mm/usec)
}