/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GOSE
#define GOSE

class ose;

#include"common.h"
//we need to define gscreen here or else there is a bad loop
//in header files
class gscreen;

/**ose stands for "on screen element"
 * it's just basically something with a position
 * and that can be displayed 
 * */
class ose {
	protected:
		//! position on screen
		SDL_Rect position;
	public:
		//! constructor (unused)
		ose();
		//! destructor 
		virtual ~ose();
		//!display ourselves
		virtual void display();
		//! register with screen
		virtual void bind(gscreen* screen);
};

#endif
