#pragma once
#include "util_tiempo.h"

namespace hw
{
namespace sensor
{
enum class ultrason_lado : unsigned int
{
	ad = 0, atr = 1, izq = 2, der = 3
};

class sensores_ultrasonicos
{
friend class ayuda_act;
static const int interrupt;
static const int pin_in;
util::micros_t comienzo_ping_[4];
volatile util::micros_t tiempos_[4];
volatile bool esperando_[4];
public:
	sensores_ultrasonicos();
	void actualizar();
	util::micros_t tiempo(ultrason_lado) const;
};
}
}