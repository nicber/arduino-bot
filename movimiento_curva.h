#pragma once
#include "movimiento.h"

namespace movimiento
{
class mov_curva_circ : public sub_mov
{
protected:
	vel_mot actualizar_imp(util::micros_t) override;
public:
	enum class lado
	{
		izq, der
	};
	
	mov_curva_circ(float, float, lado);
};
}