/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"state_rollover.h"
#include"stdarg.h"

/**this constructor takes as parameters :
 * \par states : number possible states (images)
 * \par x,y : the position
 * \par legend : text displayed (same for all states)
 * \par files : images displayed (one for each state)
 */
srollover::srollover(Uint8 states, Uint32 x, Uint32 y, char* legend, char **files) {

	//init vars and allocates structs
	state = 0;
	states_number = states;

	imgs_activated = (image**) malloc(states * sizeof(image*));
	imgs_deactivated = (image**) malloc(states * sizeof(image*));
	
	//generate text
	
	//TODO: check font color is white
	SDL_Surface* text = game->bigfont->render_text(legend);
	
	for(Uint8 i = 0 ; i < states; i++) {
		//load image
		image* ti = new image(files[i]);
		SDL_Surface* tmp = ti->get_surface();
		//create empty background
		SDL_Surface* tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE,
							116, 93,
							game->screen->format->BitsPerPixel,
							game->screen->format->Rmask,
							game->screen->format->Gmask,
							game->screen->format->Bmask,
							game->screen->format->Amask);
		imgs_deactivated[i] = new image(tmp2);

		SDL_FillRect(tmp2, NULL,
					 SDL_MapRGB(tmp2->format, 12, 32, 48));

		//blit image on background
		position.y = 24;
		position.h = 93 - 24;
		position.w = tmp->w;
		position.x = (tmp2->w - tmp->w) / 2;
		SDL_BlitSurface(tmp, NULL, tmp2, &position);
		SDL_FreeSurface(tmp);

		//add text
		position.y = 5;
		position.h = text->h;
		position.w = text->w;
		position.x = (tmp2->w - text->w) / 2;
		SDL_BlitSurface(text, NULL, tmp2, &position);

		//now build the corresponding activated image

		imgs_activated[i] = imgs_deactivated[i]->create_activated_image();

	}

	SDL_FreeSurface(text);
	img = imgs_deactivated[0];

	position.x = x;
	position.y = y;
	//TODO: ooooooh bad static stuff
	position.w = 116;
	position.h = 93;
}

srollover::~srollover() {
	for (Uint8 i = 0; i < states_number; i++) {
		delete imgs_activated[i];
		delete imgs_deactivated[i];
	}
}

void srollover::deactivate() {
	img = imgs_deactivated[state];
	game->screen2update = true;
}

void srollover::activate(Sint32 x, Sint32 y) {
	img = imgs_activated[state];
	game->screen2update = true;
}

void srollover::display() {
	img->display(&position);
}

void srollover::click() {
	state = (state + 1) % states_number;
	img = imgs_activated[state];
	game->screen2update = true;
}

Uint8 srollover::is_in(Sint32 x, Sint32 y) {
	if ((position.x <= x)&&(x <= position.x + position.w)&&(position.y <= y)&&(y <= position.y + position.h)) 
	{
		activate(x, y);
		return true;
	}
	else return false;
}

Uint8 srollover::get_state() {
	return state;
}
