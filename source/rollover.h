/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GROLLOVER
#define GROLLOVER
#include"ose.h"

class rollover;

#include"se.h"

/**rollover class, see main menu entries for example
 * basically it's just an image changing background color
 * when it gets focus from the mouse
 * all different types of rollover will inherit from this class
 * */
class rollover: public ose {
	public:
		//! constructor (unused)
		rollover();
		//! destructor
		virtual ~rollover();
		//! deactivate rollover and display
		virtual void deactivate();
		//! activate rollover
		virtual void activate(Sint32 x, Sint32 y);
		//! display on screen
		virtual void display();
		//! are coordinates inside of rollover ?
		virtual Uint8 is_in(Sint32 x, Sint32 y);
		//! register with screen
		void bind(gscreen* screen);
		//! click action
		virtual void click();
		//! move baby move!
		void move(Uint32 x, Uint32 y);
		//!is rollover activated ?
		virtual bool activated();
};

#endif
