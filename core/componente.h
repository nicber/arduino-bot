#include <stdlib.h>

namespace util
{
template <typename T, typename U>
struct is_same {
	enum { value = 0 };
};
template <typename T>
struct is_same<T, T> {
	enum { value = 1 };
};

/**
 * Funcion que toma un array de bools, y su tamaño y devuelve el resultado de reducirlo con OR.
 */
template <typename B, typename... Bs>
struct or_templ;

template <typename B>
struct or_templ<B>
{
	static const bool value = B::value;
};

template <typename B, typename... Bs>
struct or_templ
{
	static const bool value = B::value || or_templ<Bs...>::value;
};
}

namespace core
{
template <typename... Comp>
class cont_comp;

/**
 * Template para terminar la recursion.
 */
template <>
class cont_comp<>
{};

/**
 * Contenedor para componentes.
 * Cada componente debe tener un constructor default e implementar la funcion
 * actualizar().
 */
template <typename PComp, typename... Comp>
class cont_comp<PComp, Comp...>
{
	PComp comp_;
	cont_comp<Comp...> resto_comp_;
	
	/**
	 * Template recursiva para buscar un tipo.
	 */
	template <typename CompBus, bool IsSame>
	struct _rec_buscar;
	
	/**
	 * Caso usado cuando el tipo buscado esta en este nivel del contenedor.
	 */
	template <typename CompBus>
	struct _rec_buscar<CompBus, true>
	{
		inline CompBus& operator()(cont_comp<PComp, Comp...>& cont)
		{
			return cont.comp_;
		}
	};
	
	/**
	 * Caso para cuando el tipo está en un subnivel del contenedor.
	 */
	template <typename CompBus>
	struct _rec_buscar<CompBus, false>
	{
		inline CompBus& operator()(cont_comp<PComp, Comp...>& cont)
		{
			return cont.resto_comp_.buscar<CompBus>();
		}
	};

	/**
	 * Template recursiva para actualizar los sistemas.
	 */
	template <typename... ActComp>
	struct _rec_actualizar;

	/**
	 * Caso recursivo.
	 */
	template <typename PActComp, typename SActComp, typename... ActComp>
	struct _rec_actualizar<PActComp, SActComp, ActComp...>
	{
		inline void operator()(cont_comp<PComp, SActComp, ActComp...>& cont)
		{
			cont.comp_.actualizar();
			cont.resto_comp_.actualizar();
		}
	};
	
	/**
	 * Solo se usa cuando sizeof...(ActComp) == 0.
	 */
	template <typename PActComp, typename... ActComp>
	struct _rec_actualizar<PActComp, ActComp...>
	{
		inline void operator()(cont_comp<PActComp, ActComp...>& cont)
		{
			static_assert(sizeof...(ActComp) == 0, "debería usarse la anterior especializacion");
			cont.comp_.actualizar();
		}
	};

public:
	/**
	 * Funcion para buscar un componente.
	 */
	template <typename CompBus>
	inline CompBus& buscar()
	{
		static_assert(util::or_templ<util::is_same<CompBus, PComp>, util::is_same<CompBus, Comp>...>::value, "El tipo buscado no está en el contenedor");
		return _rec_buscar<CompBus, util::is_same<CompBus, PComp>::value>()(*this);
	}
	
	/**
	 * Funcion que actualiza los componentes de acuerdo con el
	 * estado del mundo exterior.
	 */
	void actualizar()
	{
		_rec_actualizar<PComp, Comp...>()(*this);
	}
};
}
