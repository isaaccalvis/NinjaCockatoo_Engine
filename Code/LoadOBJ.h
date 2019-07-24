#ifndef __LoadOBJ_H__
#define __LoadOBJ_H__

#include "Globals.h"
#include <vector>
#include "glut\glut.h"

bool loadOBJ( const char * path, std::vector < vec3 > & out_vertices, std::vector < vec2 > & out_uvs, std::vector < vec3 > & out_normals);

#endif