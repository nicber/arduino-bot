#pragma once
#include <list>
#include "util_move.h"

namespace util
{
template <typename D>
class vector2
{
	D x_;
	D y_;
	
public:
	vector2(D x, D y, D z):
	x_(util::move(x)),
	y_(util::move(y))
	{}
	
	D x() const
	{
		return x_;
	}
	
	D y() const
	{
		return y_;
	}
	
	void x(D x)
	{
		x_ = util::move(x);
	}
	
	void y(D y)
	{
		y_ = util::move(y);
	}gi
};
typedef vector2<float> vec2;

template <typename D>
class vector3
{
	D x_;
	D y_;
	D z_;
	
public:
	vector3(D x, D y, D z):
	x_(std::move(x)),
	y_(std::move(y)),
	z_(std::move(z))
	{}
};

typedef vector3<float> vec3;
}