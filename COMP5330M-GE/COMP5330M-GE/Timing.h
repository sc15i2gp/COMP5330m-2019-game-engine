#pragma once
#include "Platform.h"

struct timer
{
	LARGE_INTEGER clock_frequency;
	LARGE_INTEGER start_time;
	LARGE_INTEGER stop_time;
};

void start_timer(timer*);
void stop_timer(timer*);
long int elapsed_time(timer*);

//Basically just a timer which can be used to easily time a function
class Profiler
{
public:
	Profiler(const char* function_name);
	~Profiler();

private:
	const char* function_name;
	timer t;
};

#define TIME_FUNCTION Profiler __func_profiler(__FUNCTION__)