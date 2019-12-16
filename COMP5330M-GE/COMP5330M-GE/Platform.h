#pragma once
#include <Windows.h>
#include <stdio.h>

void OutputDebugStringf(const char* debug_string_f, ...);
char* read_file(const char* path);
void* alloc(int size);
void dealloc(void* ptr);

float get_window_height(HWND window);
float get_window_width(HWND window);
float get_aspect_ratio(HWND window);