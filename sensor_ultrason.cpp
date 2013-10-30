#include "sensor_ultrason.h"
#include "componentes.h"
#include <Arduino.h>

namespace hw
{
namespace sensor
{
class sensores_ultrasonicos;
struct ayuda_act
{
void actualizar(int i, sensores_ultrasonicos& sens)
{
	util::micros_t com = sens.lects_[i].comienzo_ping_;
	sens.esperando_[i] = false;
	sens.lects_[i].tiempo_respuesta_ = micros() - com;
	detachInterrupt(sens.interrupt + i);
}
};
}
}

namespace
{
void real_act(int i)
{
	hw::sensor::ayuda_act act;
	act.actualizar(i, componentes.buscar<hw::sensor::sensores_ultrasonicos>());
}

void (* real_acts[])(){
	[](){real_act(0);},
	[](){real_act(1);},
	[](){real_act(2);},
	[](){real_act(3);}
};
}

namespace hw
{
namespace sensor
{
void sensores_ultrasonicos::actualizar()
{
	bool esperan[4]{esperando_[0], esperando_[1], esperando_[2], esperando_[3]};
	bool emitido_ping(false);
	
	util::micros_t com = micros();
	for(unsigned int i = 0; i < 4; ++i)
	{
		/*
		 * No hay respuesta que esperar o ya pasó demasiado tiempo y
		 * no llegó.
		 */
		if(!esperan[i] || (com - lects_[i].comienzo_ping_ > timeout))
		{
			emitido_ping = true;
			lects_[i].comienzo_ping_ = com;
			digitalWrite(pin_in + i, HIGH);
			esperando_[i] = true;
		}
	}
	
	if(!emitido_ping)
		return;
	
	delayMicroseconds(5);
	
	for(unsigned int i = 0; i < 4; ++i)
	{
		if(!esperan[i])
		{
			digitalWrite(pin_in + i, LOW);
			attachInterrupt(interrupt + i, real_acts[i], FALLING);
		}
	}
}

sensores_ultrasonicos::sensores_ultrasonicos():
esperando_{false, false, false, false}
{
	for(unsigned int i = 0; i < 4; ++i)
	{
		pinMode(pin_in + i, OUTPUT);
	}
	
	actualizar();
}

lectura_ultrason sensores_ultrasonicos::lectura(ultrason_lado il) const
{
	return lects_[static_cast<unsigned int>(il)];
}

long unsigned int lectura_ultrason::distancia() const
{
	return tiempo_respuesta_ * util::mm_p_usec;
}

util::micros_t lectura_ultrason::tiempo() const
{
	return tiempo_respuesta_;
}
}
}