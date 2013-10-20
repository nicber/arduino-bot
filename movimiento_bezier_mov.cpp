#include <Arduino.h>

#include "movimiento_bezier_mov.h"
#include "movimiento_curva.h"
#include "movimiento_recto.h"
#include "util_bezier.h"

using util::vec2;

namespace 
{
vec2 intersec(const vec2& p0, float tan0, const vec2& p1, float tan1)
{
	vec2 res{(p0.y() - p1.y() - p0.x() * tan0 + p1.x() * tan1), 0};
	res.x(res.x() / (tan1 - tan0));
	res.y(tan0 * res.x() + p0.y() - tan0 * p0.x());
	return res;
}

float angulo_puntos_ce(const vec2& c, const vec2& ex1,  const vec2& ex2, float dist_c_ex1, float dist_c_ex2)
{
	vec2 c_ex1(ex1 - c);
	vec2 c_ex2(ex2 - c);
	float dot_prod((c_ex1.x() * c_ex2.x()) + (c_ex1.y() * c_ex2.y()));
	return acos(dot_prod / (dist_c_ex1 * dist_c_ex2));
}

/**
 * Devuelve true si el punto c está a la izquierda de la linea que pasa primero por a y luego por b.
 * Basado en http://stackoverflow.com/a/3461533
 */
bool lado_izq_punto(const vec2& a, const vec2& b, const vec2& c)
{
	return ((b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x())) > 0;
}
}

namespace movimiento
{
vel_mot bezier_mov::actualizar_imp(util::micros_t)
{
	if(paso_ == 100)
	{
		terminar();
		return {1, 1};
	}
	
	vec2 pos_actual(util::bezier(100/float(paso_), p_, 4));
	float pos_act_ang;
	{
		vec2 ayuda_tan(util::bezier(1000000/float(1+(int(paso_)*1000)), p_, 4));
		vec2 dif(ayuda_tan - pos_actual);
		pos_act_ang = atan2(dif.y(), dif.x());
	}
	
	vec2 sig_pos(util::bezier(100/float(++paso_), p_, 4));
	float sig_pos_ang;
	{
		vec2 ayuda_tan(util::bezier(1000000/float(1+(int(paso_)*1000)), p_, 4));
		vec2 dif(ayuda_tan - sig_pos);
		sig_pos_ang = atan2(dif.y(), dif.x());
	}
	
	return mover_giro(pos_actual, pos_act_ang, sig_pos, sig_pos_ang);
}

vel_mot bezier_mov::mover_giro(const vec2& pos_actual, float pos_act_ang, const vec2& sig_pos, float sig_pos_ang)
{
	while(pos_act_ang < 0)
		pos_act_ang += TWO_PI;
	
	while(sig_pos_ang < 0)
		sig_pos_ang += TWO_PI;
	
	vec2 intersec_tan(intersec(pos_actual, tan(pos_act_ang), sig_pos, tan(sig_pos_ang)));
	float distancia_inter_pos_act((pos_actual - intersec_tan).distance());
	float distancia_inter_sig_pos((sig_pos - intersec_tan).distance());
	
	vec2 centro_giro;
	float radio_giro;
	float angulo_giro;
	enum class rel_gr{giro_antes, giro_despues, solo_giro} rel_giro_recto;
	util::unique_ptr<mov_curva_circ> mov_curva;
	util::unique_ptr<mov_recto> mov_rect;
	float pos_act_m90_tan(tan(pos_act_ang + HALF_PI));
	float sig_pos_m90_tan(tan(sig_pos_ang + HALF_PI));
	
	if(distancia_inter_sig_pos > distancia_inter_pos_act)
	{
		vec2 auxiliar(sig_pos + (intersec_tan - sig_pos) * ((distancia_inter_sig_pos - distancia_inter_pos_act) / distancia_inter_sig_pos));
		centro_giro = intersec(pos_actual, pos_act_m90_tan, auxiliar, sig_pos_m90_tan);
		radio_giro = (auxiliar - centro_giro).distance();
		angulo_giro = angulo_puntos_ce(centro_giro, auxiliar, pos_actual, radio_giro, radio_giro);
		rel_giro_recto = rel_gr::giro_antes;
	} else if (distancia_inter_pos_act > distancia_inter_sig_pos) {
		vec2 auxiliar(pos_actual + (intersec_tan - pos_actual) * ((distancia_inter_pos_act - distancia_inter_sig_pos) / distancia_inter_pos_act));
		centro_giro = intersec(auxiliar, pos_act_m90_tan, sig_pos, sig_pos_m90_tan);
		radio_giro = (auxiliar - centro_giro).distance();
		angulo_giro = angulo_puntos_ce(centro_giro, auxiliar, sig_pos, radio_giro, radio_giro);
		rel_giro_recto = rel_gr::giro_despues;
	} else {
		centro_giro = intersec(pos_actual, pos_act_m90_tan, sig_pos, sig_pos_m90_tan);
		radio_giro = (pos_actual - centro_giro).distance();
		angulo_giro = angulo_puntos_ce(centro_giro, pos_actual, sig_pos, radio_giro, radio_giro);
		rel_giro_recto = rel_gr::solo_giro;
	}
	
	mov_curva_circ::lado lado(lado_izq_punto(pos_actual, intersec_tan, centro_giro) ? mov_curva_circ::lado::izq : mov_curva_circ::lado::der);
	mov_curva = util::make_unique<mov_curva_circ>(angulo_giro, radio_giro, lado);
	
	vel_mot res;
	switch(rel_giro_recto)
	{
		case rel_gr::giro_despues:
			mov_rect = util::make_unique<mov_recto>(fabs(distancia_inter_pos_act - distancia_inter_sig_pos));
			res = mov_rect->actualizar();
			agregar_sub(util::move(mov_curva));
			agregar_sub(util::move(mov_rect));
			break;
		case rel_gr::giro_antes:
			mov_rect = util::make_unique<mov_recto>(fabs(distancia_inter_pos_act - distancia_inter_sig_pos));
			res = mov_curva->actualizar();
			agregar_sub(util::move(mov_rect));
			agregar_sub(util::move(mov_curva));
			break;
		case rel_gr::solo_giro:
			res = mov_curva->actualizar();
			agregar_sub(util::move(mov_curva));
			break;
	}
	return res;
}

bezier_mov::bezier_mov(vec2 v, float ang)
{
	float ratio(v.distance() / 3);
	p_[0] = {0, 0};
	p_[1] = {0, ratio};
	p_[2] = v - vec2{cos(ang) * ratio, sin(ang) * ratio};
	p_[3] = util::move(v);
}
}