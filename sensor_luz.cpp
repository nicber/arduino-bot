#include <Arduino.h>
#include "componentes.h"
#include "sensor_luz.h"

static void real_actualizar();

namespace hw
{
namespace sensor
{
struct actualizar_ayuda
{
	inline void actualizar(sensor_luz& sens)
	{
		/*
		 * Si es LOW, nos estan llamando desde RISING.
		 * Si es HIGH, estamos en FALLING.
		 * Siempre tenemos que attachear con el modo opuesto.
		 */
		auto modo_attach(RISING);
		if(sens.estado_ == LOW)
			modo_attach = FALLING;
		
		sens.estado_ = !sens.estado_;
		attachInterrupt(sensor_luz::interrupt, real_actualizar, modo_attach);
	}
};

void sensor_luz::actualizar()
{
	
}

sensor_luz::sensor_luz()
{
	attachInterrupt(interrupt, real_actualizar, RISING);
}

bool sensor_luz::leer() const
{
	return estado_;
}
}
}

static void real_actualizar()
{
	hw::sensor::actualizar_ayuda act_ay;
	act_ay.actualizar(componentes.buscar<hw::sensor::sensor_luz>());
}