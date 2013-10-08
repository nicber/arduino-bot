#include <Arduino.h>
#include <stdlib.h>
#include "log.h"
#include "util_number_str.h"

namespace logger
{
logger::logger()
{
}

void logger::escribir_serial()
{
	for(auto& str : stream)
	{
		Serial.write(str);
		delete[] str;
	}
	Serial.write(0);
	
	//Limpiamos la lista
	stream = util::list<const char *>();
}

logger::~logger()
{
	escribir_serial();
}

logger& operator<<(logger& log, double n)
{
	char* str = util::number_str(n, 2);
	log << str;
	delete[] str;
	return log;
}

logger& operator<<(logger& log, long long n)
{
	char* str = util::number_str(n);
	log << str;
	delete[] str;
	return log;
}

logger& operator<<(logger& log, unsigned long long n)
{
	char* str = util::number_str(n);
	log << str;
	delete[] str;
	return log;
}

logger& operator<<(logger& log, const char * source)
{
	char* str = (char*)malloc(1 + strlen(source));
	strcpy(str, source);
	log.stream.emplace_back(str);
	return log;
}
}
