#pragma once
#include "util_unique_ptr.h"
#include "util_tiempo.h"

namespace movimiento
{
/**
 * Clase auxiliar para informar sobre la velocidad a
 * la que desea que ande cada motor.
 */
struct vel_mot
{
float izq, der;
	vel_mot()
	: izq(0), der(0)
	{}
	
	/**
	 * Izquierda y derecha respectivamente.
	 */
	vel_mot(float, float);
};

/**
 * Clase auxiliar que define un movimiento.
 * Cada movimiento puede tener sub movimientos y cuando
 * éste termina se vuelve automáticamente a actualizar el
 * movimiento padre.
 */
class sub_mov
{
friend class movimiento;
util::micros_t comienzo_ = 0;
util::unique_ptr<sub_mov> padre_;
bool terminado_ = false;

bool terminado() const;
protected:
	/**
	 * Funcion que al ser llamada cambia el estado del
	 * movimiento a terminado.
	 */
	void terminar();
	
	/**
	 * Agrega un submovimiento.
	 */
	void agregar_sub(util::unique_ptr<sub_mov>);
	
	/**
	 * Llamada por actualizar().
	 * El argumento es la cantidad de microsegundos que pasaron
	 * desde la última actualización.
	 */
	virtual vel_mot actualizar_imp(util::micros_t) = 0;
public:
	virtual ~sub_mov(){}
	
	/**
	 * Retorna el estado de los motores necesario para cumplir el
	 * movimiento.
	 */
	vel_mot actualizar();
	
};
/**
 * Es una interfaz de más alto nivel para moverse.
 * Llama a los motores en la implementación.
 */
class movimiento
{
friend class sub_mov;
util::unique_ptr<sub_mov> mov_;

public:
	/**
	 * Los parametros son representan un punto de destino en coordenadas
	 * cartesianas y un ángulo en radianes respecto al sistema de coordenadas
	 * actual en el que se quiere estar orientado.
	 */
	void mover(float, float, float);
	
	/**
	 * Función para la integración con el contenedor de componentes.
	 * Actualiza los motores de acuerdo con dónde se quiere viajar y una estimación de dónde estamos.
	 */
	void actualizar();
};
}