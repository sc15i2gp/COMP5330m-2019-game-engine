#pragma once
#include <Windows.h>
#include <stdio.h>

#define BYTES(n) n
#define KILOBYTES(n) 1024*BYTES(n)
#define MEGABYTES(n) 1024*KILOBYTES(n)
#define GIGABYTES(n) 1024*MEGABYTES(n)

struct Platform_Table
{
	bool running;
	HWND window;
	void* file_buffer; //A buffer for storing file data
	void* mem_buffer; //A buffer for dynamic allocations
};

extern Platform_Table __platform;

void OutputDebugStringf(const char* debug_string_f, ...);

char* read_file(const char* path);
void copy_mem(void* src, void* dst, int length);
void* __alloc_mem(Platform_Table*,int size);
void __dealloc_mem(Platform_Table*,void* ptr);

float __get_window_height(Platform_Table*);
float __get_window_width(Platform_Table*);
float __get_window_aspect_ratio(Platform_Table*);

bool __should_close(Platform_Table*);
HWND __get_window(Platform_Table*);
bool __initialise_platform(Platform_Table*, HINSTANCE);
void __shutdown_platform(Platform_Table*);

void __swap_window_buffers(Platform_Table*);

void __handle_input(Platform_Table*);

//NOTE: HERE IS THE PUBLIC API
#define initialise_platform(instance)	__initialise_platform(&__platform, instance)
#define shutdown_platform()				__shutdown_platform(&__platform)
#define get_window()					__get_window(&__platform)
#define get_window_width()				__get_window_width(&__platform)
#define get_window_height()				__get_window_height(&__platform)
#define get_window_aspect_ratio()		__get_window_aspect_ratio(&__platform)
#define should_window_close()			__should_close(&__platform)
#define handle_input()					__handle_input(&__platform)
#define swap_window_buffers()			__swap_window_buffers(&__platform)
#define alloc_mem(size)					__alloc_mem(&__platform, size)
#define dealloc_mem(ptr)				__dealloc_mem(&__platform, ptr)