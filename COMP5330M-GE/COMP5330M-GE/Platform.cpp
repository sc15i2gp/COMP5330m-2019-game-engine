#include "Platform.h"

void OutputDebugStringf(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	//TODO: Implement vsprintf_s
	char str_buff[2048] = {};
	vsprintf_s(str_buff, format, args);
	OutputDebugString(str_buff);
}

char* read_file(const char* path)
{
	HANDLE file_handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringf("Failed to open file: %s\n", path);
		return NULL;
	}

	DWORD size = GetFileSize(file_handle, NULL);
	CHAR* buffer = (CHAR*)malloc(size+1);
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

//NOTE: alloc and free will almost definitely need changing as our memory needs change
void* alloc(int size)
{
	HANDLE process_heap = GetProcessHeap();
	void* buffer = HeapAlloc(process_heap, HEAP_ZERO_MEMORY, size);
	if (!process_heap) OutputDebugStringf("Allocation failed\n");
	return buffer;
}

void dealloc(void* ptr)
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

float get_window_height(HWND window)
{
	RECT window_rect = get_window_rect(window);
	return (float)(window_rect.bottom - window_rect.top);
}

float get_window_width(HWND window)
{
	RECT window_rect = get_window_rect(window);
	return (float)(window_rect.right - window_rect.left);
}

float get_aspect_ratio(HWND window)
{
	return get_window_width(window) / get_window_height(window);
}