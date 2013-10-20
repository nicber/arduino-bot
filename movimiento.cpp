#include <Arduino.h>
#include "movimiento.h"
#include "componentes.h"
#include "util_move.h"
#include "movimiento_bezier_mov.h"
#include "movimiento_recto.h"

namespace movimiento
{
bool sub_mov::terminado() const
{
	return terminado_;
}

void sub_mov::agregar_sub(util::unique_ptr<sub_mov> ptr)
{
	movimiento& mov = componentes.buscar<movimiento>();
	ptr->padre_ = util::move(mov.mov_);
	mov.mov_ = util::move(ptr);
}

vel_mot sub_mov::actualizar()
{
	if(comienzo_ == 0)
	{
		comienzo_ = micros();
		return actualizar_imp(0);
	} else {
		util::micros_t act(micros());
		vel_mot res(actualizar_imp(act - comienzo_));
		comienzo_ = act;
		return res;
	}
}

void sub_mov::terminar()
{
	terminado_ = true;
}

void movimiento::actualizar()
{
	if(mov_)
	{
		while(mov_ && mov_->terminado()) //Si el sub movimiento terminó seguimos con el padre.
		{
			mov_ = util::move(mov_->padre_);
		}
	}
	
	if(mov_)
	{
		vel_mot res = mov_->actualizar();
		hw::motores& mot = componentes.buscar<hw::motores>();
		mot.set_velocidad(hw::motores::lado::der, res.der);
		mot.set_velocidad(hw::motores::lado::izq, res.izq);
	}
}

void movimiento::mover(float der, float ad, float ang)
{
	if(der == 0 && ang == 0){
		mov_ = util::make_unique<mov_recto>(ad);
	} else {
		mov_ = util::make_unique<bezier_mov>(util::vec2{der, ad}, ang);
	}
}
}
