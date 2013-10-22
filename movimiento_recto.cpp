#include "movimiento_recto.h"
#include "motor.h"

namespace movimiento
{
vel_mot mov_recto::actualizar_imp(util::micros_t tiempo_pasado)
{
	if(tiempo_pasado >= tiempo_restante_)
	{
		terminar();
		
	} else {
		tiempo_restante_ -= tiempo_pasado;
	}
	return {1, 1};
}

mov_recto::mov_recto(float distancia)
: tiempo_restante_(distancia / hw::motores::mm_us)
{}
}