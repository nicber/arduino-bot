#pragma once
#include "movimiento.h"

namespace movimiento
{
/**
 * Movimiento de curva de estilo circular.
 */
class mov_curva_circ : public sub_mov
{
	util::micros_t tiempo_restante_;
	float vel_der_;
	float vel_izq_;
protected:
	vel_mot actualizar_imp(util::micros_t) override;
public:
	/**
	 * Usado para determinar de qué lado está el centro del arco
	 * de giro.
	 */
	enum class lado
	{
		izq, der
	};
	
	/**
	 * Toma el angulo en radianes, la distancia del centro de giro y el lado en el
	 * cual está en centro de giro.
	 */
	mov_curva_circ(float, float, lado);
};
}