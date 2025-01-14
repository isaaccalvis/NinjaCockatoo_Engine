#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

#define LOG_IDE(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define LOG_CONSOLE(format, ...) logConsole(__FILE__, __LINE__, format, __VA_ARGS__);
#define LOG_CONSOLE_IDE(format, ...) LOG_IDE(format) LOG_CONSOLE(format)

void log(const char file[], int line, const char* format, ...);
void logConsole(const char file[], int line, const char* format, ...);

//#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
//
//void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;
typedef unsigned long int uuid_unit;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum MESH_TYPE
{
	PRIMITIVE_NONE = 0,
	PRIMITIVE_CUBE,
	PRIMITIVE_PLANE,
	PRIMITIVE_SPHERE,
	CUSTOM_MESH
};