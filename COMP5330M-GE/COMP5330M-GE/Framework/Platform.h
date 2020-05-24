#pragma once
#include <Windows.h>
#include <stdio.h>
#include "../Tools/Maths.h"

#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"

enum Keyboard_Key
{
	KEY_START = 0,
	KEY_SHIFT = 0x10,
	KEY_ESC = 0x1b,
	KEY_RETURN = 0x0D,
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

class Platform_Table
{
public:
	char* __read_file(const char* path);
	void __copy_mem(void* src, void* dst, int length);
	void* __alloc_mem(int size);
	void __dealloc_mem(void* ptr);

	float __get_window_height();
	float __get_window_width();
	float __get_window_aspect_ratio();

	bool __should_close();
	HWND __get_window();
	bool __initialise_platform(HINSTANCE);
	void __shutdown_platform();

	void __swap_window_buffers();

	void __handle_input();

	bool __was_key_pressed(Keyboard_Key key);
	bool __was_mouse_button_pressed(Mouse_Button button);
	bool __was_mouse_moved();

	Vector2 __get_initial_mouse_position();
	Vector2 __get_final_mouse_position();

	/*For setting up the fake context*/
	PIXELFORMATDESCRIPTOR createDummyPFD(HDC dummyDC);



private:
	bool running;

	HWND dummyWindow; // A dummy window for allowing OpenGL 3.2+
	HWND window;

	void* file_buffer; //A buffer for storing file data
	void* mem_buffer; //A buffer for dynamic allocations
	bool was_key_pressed[KEY_COUNT];
	bool was_mouse_button_pressed[BUTTON_COUNT];

	//These are used to track the cursor's position in a frame
	Vector2 initial_cursor_position;
	Vector2 final_cursor_position;
	bool was_mouse_moved;

	friend LRESULT CALLBACK window_event_handler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);


};

ATOM registerWindowClass(HINSTANCE instance);

extern Platform_Table __platform;

void OutputDebugStringf(const char* debug_string_f, ...);

//NOTE: HERE IS THE PUBLIC API
#define initialise_platform(instance)		__platform.__initialise_platform(instance)
#define shutdown_platform()					__platform.__shutdown_platform()
#define get_window()						__platform.__get_window()
#define get_window_width()					__platform.__get_window_width()
#define get_window_height()					__platform.__get_window_height()
#define get_window_aspect_ratio()			__platform.__get_window_aspect_ratio()
#define should_window_close()				__platform.__should_close()
#define handle_input()						__platform.__handle_input()
#define swap_window_buffers()				__platform.__swap_window_buffers()
#define alloc_mem(size)						__platform.__alloc_mem(size)
#define dealloc_mem(ptr)					__platform.__dealloc_mem(ptr)
#define was_key_pressed(key)				__platform.__was_key_pressed(key)
#define was_mouse_button_pressed(button)	__platform.__was_mouse_button_pressed(button)
#define get_initial_mouse_position()		__platform.__get_initial_mouse_position()
#define get_final_mouse_position()			__platform.__get_final_mouse_position()
#define was_mouse_moved()					__platform.__was_mouse_moved()
#define read_file(path)						__platform.__read_file(path)
#define copy_mem(src, dst, length)			__platform.__copy_mem(src, dst, length)