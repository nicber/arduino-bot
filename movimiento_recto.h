#pragma once
#include "movimiento.h"

namespace movimiento
{
/**
 * Movimiento de linea recta.
 */
class mov_recto : public sub_mov
{
protected:
	vel_mot actualizar_imp(util::micros_t) override;
public:
	/**
	 * Cantidad a moverse para adelante.
	 */
	mov_recto(float);
};
}