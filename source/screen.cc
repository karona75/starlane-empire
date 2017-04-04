/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"screen.h"
#include"se.h"

gscreen::gscreen()
{
	focused_rollover = NULL;
}

gscreen::~gscreen()
{
}

void gscreen::add_rollover(rollover* r) {
	rollovers.push_back(r);
	oses.push_back((ose*)r);
}

void gscreen::add_ose(ose* o) {
	oses.push_back(o);
}

Uint8 gscreen::handle_rollover(Sint32 x, Sint32 y) {
	Uint8 activated_number = 0;
	for(vector<rollover*>::iterator it = rollovers.begin(); it != rollovers.end(); it++)
	{
		activated_number++;
		if ((*it)->is_in(x, y)) {
			
			if ((focused_rollover != NULL)&&(focused_rollover != (*it))) 					focused_rollover->deactivate();
			focused_rollover = (*it);
			return activated_number;
		}
	}
	if (focused_rollover != NULL) focused_rollover->deactivate();
	focused_rollover = NULL;
	return 0;
}

void gscreen::handle()
{
}

void gscreen::display()
{
	//display all elements on screen
	for(vector<ose*>::iterator it = oses.begin(); it != oses.end(); it++)
		(*it)->display();
	//put mouse on top
	game->mousepointer->display();
}

