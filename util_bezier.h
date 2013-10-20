#pragma once
#include <stdlib.h>
#include "vector3.h"

namespace util
{
/**
 * Devuelve un punto en una curva bezier de size_t puntos, pasados con
 * el segundo argumento, cuando t es igual al primer argumento.
 */
util::vec2 bezier(float, util::vec2*, size_t);
}