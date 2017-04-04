/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"se.h"
#include"rollover.h"

rollover::rollover() {}
rollover::~rollover() {}
Uint8 rollover::is_in(Sint32 x, Sint32 y) {
return 0;
}

void rollover::deactivate() {}

/**this method activates the rollover
 * \par x : x position of mouse pointer
 * \par y : y postion of mouse pointer
 * only corresponding rollover subpart should be activated*/
void rollover::activate(Sint32 x, Sint32 y) {}
void rollover::display() {}


void rollover::bind(gscreen* screen) {
	screen->add_rollover(this);
}

/**this method is called by the screen class when 
 * mouse is clicked inside the rollover*/
void rollover::click() {}
bool rollover::activated() {
	return false;
}

void rollover::move(Uint32 x, Uint32 y) {
	position.x = x;
	position.y = y;
}
