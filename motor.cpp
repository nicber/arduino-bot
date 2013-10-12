#include "motor.h"

namespace hw
{
const float motores::sep_lados = 160;
const float motores::mm_ms = 0.5;

void motores::actualizar()
{

}

void motores::set_velocidad(motores::lado l, float v)
{
	if(v > 1)
	{
		v = 1;
	} else if (v < -1)
	{
		v = -1;
	}
	
	if(l == lado::der)
	{
		vel_der = v;
	} else if (l == lado::izq)
	{
		vel_izq = v;
	}
}

float motores::get_velocidad(motores::lado lad) const
{
	return lad == lado::der ? vel_der : vel_izq;
}
}