#include "motor.h"

namespace hw
{
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

}