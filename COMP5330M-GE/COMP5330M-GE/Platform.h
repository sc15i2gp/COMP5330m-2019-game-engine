#pragma once
#include <Windows.h>
#include <stdio.h>
#include "Maths.h"

enum Keyboard_Key
{
	KEY_START = 0,
	KEY_SHIFT = 0x10,
	KEY_ESC = 0x1b,
	KEY_LEFT = 0x25,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_A = 0x41,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_COUNT //This KEY_COUNT is used to limit loops over keys and such
};

enum Mouse_Button
{
	BUTTON_START = 0,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_COUNT
};

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
	bool was_key_pressed[KEY_COUNT];
	bool was_mouse_button_pressed[BUTTON_COUNT];

	//These are used to track the cursor's position in a frame
	Vector2 initial_cursor_position;
	Vector2 final_cursor_position;
	bool was_mouse_moved;
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

bool __was_key_pressed(Platform_Table*, Keyboard_Key key);
bool __was_mouse_button_pressed(Platform_Table*, Mouse_Button button);
bool __was_mouse_moved(Platform_Table*);

Vector2 __get_initial_mouse_position(Platform_Table*);
Vector2 __get_final_mouse_position(Platform_Table*);

//NOTE: HERE IS THE PUBLIC API
#define initialise_platform(instance)		__initialise_platform(&__platform, instance)
#define shutdown_platform()					__shutdown_platform(&__platform)
#define get_window()						__get_window(&__platform)
#define get_window_width()					__get_window_width(&__platform)
#define get_window_height()					__get_window_height(&__platform)
#define get_window_aspect_ratio()			__get_window_aspect_ratio(&__platform)
#define should_window_close()				__should_close(&__platform)
#define handle_input()						__handle_input(&__platform)
#define swap_window_buffers()				__swap_window_buffers(&__platform)
#define alloc_mem(size)						__alloc_mem(&__platform, size)
#define dealloc_mem(ptr)					__dealloc_mem(&__platform, ptr)
#define was_key_pressed(key)				__was_key_pressed(&__platform, key)
#define was_mouse_button_pressed(button)	__was_mouse_button_pressed(&__platform, button)
#define get_initial_mouse_position()		__get_initial_mouse_position(&__platform)
#define get_final_mouse_position()			__get_final_mouse_position(&__platform)
#define was_mouse_moved()					__was_mouse_moved(&__platform)