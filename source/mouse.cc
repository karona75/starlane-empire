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

mouse::mouse()
{
	//first load all different mouse images
	mouses[0] = new image("0_mouse.png");
	mouses[1] = new image("1_mouse.png");
	mouses[2] = new image("2_mouse.png");
	mouses[3] = new image("3_mouse.png");
	mouses[4] = new image("4_mouse.png");
	mouses[5] = new image("8_mouse.png");
	mouses[6] = new image("9_mouse.png");
	mouses[7] = new image("10_mouse.png");
	mouses[8] = new image("11_mouse.png");

	//now choose the first mouse
	img = mouses[0];
	xdec = 0;
	ydec = 0;
	SDL_WarpMouse(0, 0);
	mousepos.x = 0;
	mousepos.y = 0;
	mousepos.w = img->get_surface()->w;
	mousepos.h = img->get_surface()->h;

	SDL_ShowCursor(0);
	disable();
}

mouse::~mouse()
{
#ifdef DEBUG
	cout << "Shutting down mouse" << endl;
#endif
	delete mouses[0];
	delete mouses[1];
	delete mouses[2];
	delete mouses[3];
	delete mouses[4];
	delete mouses[5];
	delete mouses[6];
	delete mouses[7];
}

/**this moves the mouse, takes new mask, draw new mouse, updates coordinates*/
void mouse::move(SDL_Event * event)
{
	//update mouse coordinates
	mousepos.x = event->motion.x;
	mousepos.y = event->motion.y;
	if (active == true)
		game->screen2update = true;
}

void mouse::move(Sint32 x, Sint32 y) {
	//update mouse coordinates
	mousepos.x = x;
	mousepos.y = y;
	if (active == true)
		game->screen2update = true;
}

/**displays mouse (should be displayed at last)*/
void mouse::display() {
	if (active==true) {
		img->display(&mousepos);
	}
}

Sint16 mouse::get_x() {
	return mousepos.x;
}

Sint16 mouse::get_y() {
	return mousepos.y;
}

void mouse::enable() {
	active = true;
	game->screen2update = true;
}

void mouse::disable() {
	active = false;
}
