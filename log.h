#pragma once
#include "util_list.h"
namespace logger
{

/**
Class used for logging.
Users should use operator<< for writing logs.
When it's destroyed it writes everything to stderr.
*/
class logger
{
util::list<const char *> stream; ///< Temporarily stores the log until it's written.
void escribir_serial();
friend logger& operator<<(logger&, const char *);
public:
	/**
	Default constructor.
	*/
	logger();
	
	/**
	Destructor that writes the log to std::cerr.
	*/
	~logger();
};

/**
 * Used for logging.
 */
logger& operator<<(logger&, long long);
logger& operator<<(logger&, unsigned long long);
logger& operator<<(logger&, double);
logger& operator<<(logger&, const char *);
/**
Returns a logger instance which prepends the function's name, the file from where
it's calling and thet line number.
*/
#define LOG logger::logger() << __PRETTY_FUNCTION__ << ": [" \
	 << __FILE__ << ":" << __LINE__ << "]: "
}
