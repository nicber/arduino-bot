#include "movimiento_curva.h"
#include "motor.h"
#include "componentes.h"

namespace movimiento
{
mov_curva_circ::mov_curva_circ(float ang, float distancia, mov_curva_circ::lado lad)
{
	/*
	 * Si 2pi -> 2 * radio * pi
	 *    x -> x * 2 * radio * pi / 2 * pi
	 * Simplificando:
	 *    x -> x * radio
	 */
	float rec_externo = ang * (distancia + hw::motores::sep_lados);
	float rec_interno = ang * (distancia - hw::motores::sep_lados);
	
	(lad == lado::der ? vel_der_ : vel_izq_) = rec_interno / rec_externo;
	(lad == lado::der ? vel_izq_ : vel_der_) = 1;
	
	tiempo_restante_ = rec_externo / hw::motores::mm_us;
}

vel_mot mov_curva_circ::actualizar_imp(util::micros_t tiempo_pasado)
{
	if(tiempo_pasado >= tiempo_restante_){
		terminar();
		return {1, 1};
	}else {
		tiempo_restante_ -= tiempo_pasado;
		return {vel_izq_, vel_der_};
	}
}
}