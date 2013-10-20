#pragma once
#include "movimiento.h"
#include "vector3.h"

namespace movimiento
{
/**
 * Clase para representar un movimiento hasta un punto
 * donde el robot termina con determinada orientacion;
 */
class bezier_mov : public sub_mov
{
unsigned char paso_ = 0;
util::vec2 p_[4];
vel_mot actualizar_imp(util::micros_t) override;
vel_mot mover_giro(const util::vec2&, float, const util::vec2&, float);

public:
	/**
	 * Constructor que toma el punto final y el ángulo en
	 * el que se quiere terminar respecto al eje x.
	 */
	bezier_mov(util::vec2, float);
};
}