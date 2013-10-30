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
		if(!esperan[i])
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
	actualizar();
}

util::micros_t sensores_ultrasonicos::tiempo(ultrason_lado il) const
{
	unsigned int i(static_cast<unsigned int>(il));
	if(esperando_[i]){
		return micros() - comienzo_ping_[i];
	} else {
		return tiempos_[i];
	}
}
}
}