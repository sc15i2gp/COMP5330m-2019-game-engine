#include "Platform.h"

Platform_Table __platform;

bool was_key_down_before_this_frame(LPARAM lparam)
{
	int key_state_mask = 1 << 30;
	return lparam & key_state_mask;
}

Vector2 get_cursor_position(HWND window)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(window, &p);
	Vector2 v(p.x, p.y );
	return v;
}
//This is here so that imgui inputs work
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
//Function is called for every event passed to the process by Windows
LRESULT CALLBACK window_event_handler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam)) return true;
	LRESULT result = 0;
	switch (message)
	{
	case WM_CLOSE: //Case: Window 'X' has been pressed
		__platform.running = false; //Set the program to stop running
		OutputDebugString("Close pressed\n");
		break;
	case WM_LBUTTONDOWN:
		__platform.was_mouse_button_pressed[BUTTON_LEFT] = true;
		break;
	case WM_LBUTTONUP:
		__platform.was_mouse_button_pressed[BUTTON_LEFT] = false;
		break;
	case WM_KEYDOWN:
		__platform.was_key_pressed[wparam] = true;
		break;
	case WM_KEYUP:
		__platform.was_key_pressed[wparam] = false;
		break;
	case WM_MOUSEMOVE:
		__platform.final_cursor_position = get_cursor_position(__platform.window);
		__platform.was_mouse_moved = true;
		break;
	case WM_EXITSIZEMOVE:
		__platform.window_resized = true;
		break;
	default: //Anything else
		result = DefWindowProc(window, message, wparam, lparam); //Call the default window handling routine for the given message
		break;
	}
	return result;
}

bool Platform_Table::__initialise_platform(HINSTANCE instance)
{
	//Create window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_event_handler;
	window_class.lpszClassName = "GameEngineClass";

	//Attempt to register the window class
	if (RegisterClass(&window_class))
	{//If the class registation was successful
		this->window = CreateWindow(window_class.lpszClassName, "Game Engine Test",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, instance, 0);

		this->running = true;
		this->initial_cursor_position = get_cursor_position(this->window);
		return true;
	}
	else return false;
}

void Platform_Table::__shutdown_platform()
{
}

HWND Platform_Table::__get_window()
{
	return this->window;
}

void OutputDebugStringf(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	//TODO: Implement vsprintf_s
	char str_buff[2048] = {};
	vsprintf_s(str_buff, format, args);
	OutputDebugString(str_buff);
}

char* Platform_Table::__read_file(const char* path)
{
	HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringf("Failed to open file: %s\n", path);
		return NULL;
	}

	DWORD size = GetFileSize(file_handle, NULL);
	CHAR* buffer = (CHAR*)alloc_mem(size+1);
	DWORD size_read = 0;
	BOOL file_read = ReadFile(file_handle, buffer, size-1, &size_read, NULL);
	if (!file_read)
	{
		OutputDebugStringf("Failed to read file: %s\n", path);
		return NULL;
	}
	OutputDebugStringf("%d bytes read of: %s\n", size_read, path);
	buffer[size_read] = 0;
	CloseHandle(file_handle);
	return buffer;
}

void Platform_Table::__copy_mem(void* src, void* dst, int length)
{
	CopyMemory(dst, src, length);
}

//NOTE: alloc and free will almost definitely need changing as our memory needs change
void* Platform_Table::__alloc_mem(int size)
{
	HANDLE process_heap = GetProcessHeap();
	void* buffer = HeapAlloc(process_heap, 0, size);
	if (!buffer)
	{
		OutputDebugStringf("Allocation failed\n");
		return NULL;
	}
	ZeroMemory(buffer, size);
	return buffer;
}

void Platform_Table::__dealloc_mem(void* ptr)
{
	HANDLE process_heap = GetProcessHeap();
	BOOL freed = HeapFree(process_heap, 0, ptr);
	if (!freed) OutputDebugStringf("Free failed\n");
}

RECT get_window_rect(HWND window)
{
	RECT window_rect;
	GetClientRect(window, &window_rect);
	return window_rect;
}

float Platform_Table::__get_window_height()
{
	RECT window_rect = get_window_rect(this->window);
	return (float)(window_rect.bottom - window_rect.top);
}

float Platform_Table::__get_window_width()
{
	RECT window_rect = get_window_rect(this->window);
	return (float)(window_rect.right - window_rect.left);
}

float Platform_Table::__get_window_aspect_ratio()
{
	return get_window_width() / get_window_height();
}

void Platform_Table::__handle_input()
{
	this->initial_cursor_position = this->final_cursor_position;
	this->was_mouse_moved = false;

	//Handle input events
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{//Process each input event message given since the last frame
		//Push messages to thread's input event queue
		TranslateMessage(&message);
		//Send the message to the window event handler function
		DispatchMessage(&message);
	}
}

bool Platform_Table::__was_mouse_button_pressed(Mouse_Button button)
{
	return this->was_mouse_button_pressed[button];
}

bool Platform_Table::__was_key_pressed(Keyboard_Key key)
{
	return this->was_key_pressed[key];
}

bool Platform_Table::__was_window_resized()
{
	bool resized = this->window_resized;
	this->window_resized = false;
	return resized;
}

bool Platform_Table::__should_close()
{
	return !this->running;
}

Vector2 Platform_Table::__get_initial_mouse_position()
{
	return this->initial_cursor_position;
}

Vector2 Platform_Table::__get_final_mouse_position()
{
	return this->final_cursor_position;
}

bool Platform_Table::__was_mouse_moved()
{
	return this->was_mouse_moved;
}

void Platform_Table::__swap_window_buffers()
{
	HDC device_context = GetDC(this->window);
	SwapBuffers(device_context);
}