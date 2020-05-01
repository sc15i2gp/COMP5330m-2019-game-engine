#include "Timing.h"

void start_timer(timer* t)
{
	QueryPerformanceFrequency(&t->clock_frequency);
	QueryPerformanceCounter(&t->start_time);
}

void stop_timer(timer* t)
{
	QueryPerformanceCounter(&t->stop_time);
}

long int elapsed_time(timer* t)
{
	LARGE_INTEGER elapsed;
	elapsed.QuadPart = t->stop_time.QuadPart - t->start_time.QuadPart;
	elapsed.QuadPart *= 1000;
	elapsed.QuadPart /= t->clock_frequency.QuadPart;

	return elapsed.QuadPart;
}

Profiler::Profiler(const char* func_name)
{
	this->function_name = func_name;
	start_timer(&this->t);
}

Profiler::~Profiler()
{
	stop_timer(&this->t);
	long int elapsed = elapsed_time(&this->t);
	OutputDebugStringf("Function %s took %ld ms\n", this->function_name, elapsed);
}