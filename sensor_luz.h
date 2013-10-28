#pragma once
namespace hw
{
namespace sensor
{
/**
 * Clase para representar un sensor de luz.
 */
class sensor_luz
{
	friend class actualizar_ayuda;
	volatile bool estado_{false};
public:
	static const int interrupt;
	sensor_luz();
	/**
	 * Lee y actualiza el estado.
	 */
	void actualizar();
	
	/**
	 * Devuelve si hay una linea blanca debajo del sensor.
	 */
	bool leer() const;
};
}
}