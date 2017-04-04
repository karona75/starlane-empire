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
#include"introduction.h"

gintroduction::gintroduction()
{
#ifdef DEBUG
	cout << "Introduction's constructor" << endl;
#endif
	logo = new image("logo.gif");
	image_container *imc = new image_container(logo);
	imc->bind(this);
	game->images->set_gamma(0.0f);

	game->frame = 0;			//we start at frame number 0
	game->framerate = 10;		//each frame = 10msec
}

gintroduction::~gintroduction()
{
#ifdef DEBUG
	cout << "Introduction's destructor" << endl;
#endif
	delete logo;
}

/**this method handles the logo fade out*/
void
 gintroduction::handle()
{
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->frame = 255;
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			game->quit();
			break;
		}
	}
	
	if (game->frame >= 255) {
		game->mainmenu();
	} else {
		//fade image
		game->images->set_gamma((float) game->frame / 128.0f);
		game->screen2update = true;
	}
}
