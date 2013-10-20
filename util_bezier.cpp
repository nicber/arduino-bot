#include "util_bezier.h"

namespace util
{
util::vec2 bezier(size_t cant, float t, util::vec2* ps)
{
	using util::vec2;
	if(cant == 1)
	{
		return *ps;
	}
	
	return (bezier(cant - 1, t, ps) * (1 - t)) + (bezier(cant - 1, t, ps + 1) * t);
}
}