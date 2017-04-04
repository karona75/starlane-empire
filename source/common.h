/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GCOMMON
#define GCOMMON
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include<SDL.h>
#include<SDL_timer.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<GL/gl.h>
#include<GL/glu.h>

#include <ext/hash_map>
#include<vector>

#include<iostream>

extern "C" {
#include<lua5.2/lua.h>
#include<lua5.2/lualib.h>
#include<lua5.2/lauxlib.h>
}
	
#define DEBUG TRUE
#define RACECOLORS 7
#define RACES 21
using namespace __gnu_cxx;
using namespace std;

//string comparison function for hash tables
struct eqstr
{
	  bool operator()(const char* s1, const char* s2) const
			  {
					    return strcmp(s1, s2) == 0;
				}
};

#endif
