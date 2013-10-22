#pragma once
#include "core_componente.h"

namespace hw
{
/**
 * Clase para representar los 4 motores.
 */
class motores
{
	float vel_der = 0;
	float vel_izq = 0;
public:
	static const float sep_lados; ///< Separación entre los centros de las ruedas al centro en mm.
	static const float mm_us; ///< Máxima velocidad en mm/us
	enum class lado
	{
		der,
		izq
	};
	
	/**
	 * Actualiza el hardware para que doble y vaya a la velocidad
	 * deseada.
	 */
	void actualizar();
	
	/**
	 * Cambia la velocidad del lado a un valor entre 1 y -1.
	 * 1 significa adelente a máxima velocidad y -1 marcha atras.
	 * Cualquier otro valor se ajusta al rango entre 1 y -1, por ejemplo,
	 * si se pasa un 3 guarda un 1.
	 */
	void set_velocidad(lado, float);
	
	/**
	 * Obtener el valor de la velocidad de un lado en el rango [-1; 1].
	 */
	float get_velocidad(lado) const;
};
}