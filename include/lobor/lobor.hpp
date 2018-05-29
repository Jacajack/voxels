#ifndef LOBOR_HPP
#define LOBOR_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "texmodel.hpp"
#include "scene.hpp"
#include "framebuffer.hpp"
#include "window.hpp"

//3D engine main namespace
namespace lobor
{
	//Init and destroy functions
	extern void init_glfw( );
	extern void init_glew( );
	extern void destroy( );

	//Utils
	extern void __attribute__( ( format ( printf, 2, 3 ) ) ) log( int level, const char *format, ... );
	extern void __attribute__( ( format ( printf, 1, 2 ) ) ) log( const char *format, ... );
	#define LOBOR_NOTICE 0
	#define LOBOR_LOG LOBOR_NOTICE
	#define LOBOR_INFO LOBOR_NOTICE
	#define LOBOR_WARN   1
	#define LOBOR_ERROR  2
	#define LOBOR_DEBUG  3
	#define LOBOR_DEBUG_WARN 4
};

#endif