#include <math.h>
#include <stdlib.h>
#include "util_number_str.h"

namespace
{
	template <typename N>
	size_t num_digitos(N n)
	{
		if(n == 0){
			return 1;
		} else {
			return 1 + log10(abs(n));
		}
	}
	
	template <typename N>
	char* escribir_numero(N n)
	{
		size_t digitos = num_digitos(n);
		char* str = new char[digitos += (n < 0 ? 2 : 1) /*terminador y signo -*/];
		str[digitos] = 0;

		if(n < 0)
		{
			str[0] = '-';
			n = -n;
		}

		--digitos;
		do
		{
			str[digitos] = n % 10 + '0';
			n /= 10;
		} while (n);

		return str;
	}
}

namespace util
{
char* number_str(long long n)
{
	return escribir_numero(n);
}

char* number_str(unsigned long long n)
{
	return escribir_numero(n);
}

char* number_str(double n, size_t decimal)
{
	long long nl = n * pow(10, decimal);
	return number_str(nl);
}
}