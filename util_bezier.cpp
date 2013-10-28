#include "util_bezier.h"

namespace util
{
util::vec2 bezier(float t, util::vec2* ps, size_t cant)
{
	if(cant == 1)
	{
		return *ps;
	}
	
	return (bezier(t, ps, cant - 1) * (1 - t)) + (bezier(t, ps + 1, cant - 1) * t);
}
}