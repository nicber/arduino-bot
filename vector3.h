#pragma once
#include <list>

namespace util
{
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

typedef vector3<float> vector;
}