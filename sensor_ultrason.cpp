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
	util::micros_t com = sens.comienzo_ping_[i];
	sens.esperando_[i] = false;
	sens.tiempos_[i] = micros() - com;
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
		if(!esperan[i] || (com - comienzo_ping_[i] > timeout))
		{
			emitido_ping = true;
			comienzo_ping_[i] = com;
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
tiempos_{0, 0, 0, 0},
esperando_{false, false, false, false}
{
	for(unsigned int i = 0; i < 4; ++i)
	{
		pinMode(pin_in + i, OUTPUT);
	}
	
	actualizar();
}

util::micros_t sensores_ultrasonicos::tiempo(ultrason_lado il) const
{
	unsigned int i(static_cast<unsigned int>(il));
	if(esperando_[i]){
		unsigned int dif(micros() - comienzo_ping_[i]);
		if(dif > timeout)
		{
			return no_data;
		}
		else
		{
			return dif;
		}
	} else {
		return tiempos_[i];
	}
}

long unsigned int sensores_ultrasonicos::distancia(ultrason_lado lado) const
{
	static const float mm_p_usec(340.0 / 1E6 * 1E3); //340m/s (vel. sonido) / 1E6 (convertido a m/usec) * 1E3 (conv. a mm/usec)
	auto res(tiempo(lado));
	if(res == no_data)
	{
		return no_data;
	} else 
	{
		return res * mm_p_usec;
	}
}
}
}