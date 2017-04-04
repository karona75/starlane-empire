/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"menu_rollover.h"
#include"basic_rollover.h"


/**constructs a menu with images
 * \par visible_elements : number of elements you may see on screen
 * \par elements : total number of elements in menu
 * \par x : x position of menu
 * \par y : y position of menu
 * \par files : file names of images (should be equal numbers as elements)
 * NOTE: image files are loaded through the image engine
 * */
mrollover::mrollover(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, char** files) {

	SDL_Surface** images = (SDL_Surface**) malloc(elements * sizeof (SDL_Surface*));

	for (Uint8 i = 0; i < elements; i++) {
		images[i] = game->images->load(files[i])->get_surface();
	}

	init(visible_elements, elements, x, y, images);
	free(images);
}


/**constructs a menu with images
 * \par visible_elements : number of elements you may see on screen
 * \par elements : total number of elements in menu
 * \par x : x position of menu
 * \par y : y position of menu
 * \par files : file names of images (should be equal numbers as elements)
 * \par background : background for our image
 * */
mrollover::mrollover(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, char** files, SDL_Surface* background) {

	SDL_Surface** images = (SDL_Surface**) malloc(elements * sizeof (SDL_Surface*));
	
	for(Uint8 i = 0; i < elements; i++) {

		//create image
		images[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,
				background->w,
				background->h,
				game->screen->format->BitsPerPixel,
				game->screen->format->Rmask,
				game->screen->format->Gmask,
				game->screen->format->Bmask,
				game->screen->format->Amask);

		//add an ounce of blue
		SDL_FillRect(images[i], NULL, SDL_MapRGB(images[i]->format, 12, 32, 48));

		//load image from file
		image *imgtmp = new image(files[i]);
		SDL_Surface* tmp = imgtmp->get_surface();

		//center it on background
		SDL_Rect r;
		r.w = tmp->w;
		r.h = tmp->h;
		r.x = (background->w - tmp->w)/2;
		r.y = (background->h - tmp->h)/2;

		//blit it on background
		SDL_BlitSurface(tmp, NULL, images[i], &r);
		SDL_FreeSurface(tmp);
		
		//copy background over surface 
		//TODO: gee i guess we should really name it foreground :)
		SDL_BlitSurface(background, NULL, images[i], NULL);
	}

	init(visible_elements, elements, x, y, images);
	free(images);
}



/**constructs a menu with images
 * \par visible_elements : number of elements you may see on screen
 * \par elements : total number of elements in menu
 * \par x : x position of menu
 * \par y : y position of menu
 * \par images : images (should be equal numbers as elements)
 * */
void mrollover::init(Uint8 visible_elements, Uint8 elements, Uint32 x, Uint32 y, SDL_Surface** images) {

	//first allocates structs
	entries = (rollover**) malloc (elements * sizeof(rollover*));
	visible_entries = (rollover**) malloc (visible_elements * sizeof(rollover*));

	//load elements
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	for (Uint8 i = 0; i < elements; i++) {
		pos.w = images[i]->w;
		pos.h = images[i]->h;
		image *tmp = new image(images[i]);
		entries[i] = new brollover(tmp, pos);
		pos.y += pos.h;
	}

	for (Uint8 i = 0; i < visible_elements; i++) {
		visible_entries[i] = entries[i];
	}

	//now place everything
	position.x = x;
	position.y = y;
	position.w = 25 + images[0]->w;
	position.h = visible_elements * images[0]->h;

	//load buttons
	pos.x = x + images[0]->w;
	pos.y = y;
	pos.w = 25;
	pos.h = 163;
	up_arrow = new brollover("0_listbox.png", pos, 0, 0);
	pos.y = y + images[0]->h * visible_elements - 163;
	down_arrow = new brollover("1_listbox.png", pos, 0, 138);

	//misc init
	num_entries = elements;
	num_vis_entries = visible_elements;
	first_entry = 0;
	entry_height = images[0]->h;
}



mrollover::~mrollover() {
	for(Uint8 i = 0 ; i < num_entries; i++)
		delete entries[i];
	delete up_arrow;
	delete down_arrow;
}

void mrollover::display() {
	for (Uint8 i = 0; i < num_vis_entries; i++)
		visible_entries[i]->display();
	up_arrow->display();
	down_arrow->display();
}

void mrollover::activate(Sint32 x, Sint32 y) {
	if (up_arrow->is_in(x, y)) {
		down_arrow->deactivate();
		deactivate_visibles();
		focused_entry = -1;
		return;
	}
	
	if (down_arrow->is_in(x, y)) {
		up_arrow->deactivate();
		deactivate_visibles();
		focused_entry = -1;
		return;
	}
	
	for (Uint8 i = 0; i < num_vis_entries; i++)
		if (visible_entries[i]->is_in(x, y)) {
			focused_entry = i + first_entry;
			for (Uint8 j = 0; j < num_vis_entries; j++)
				if (j!=i) visible_entries[j]->deactivate();
			up_arrow->deactivate();
			down_arrow->deactivate();
			return;
		}
}

void mrollover::deactivate_visibles() {
	for (Uint8 i = 0; i < num_vis_entries; i++)
		visible_entries[i]->deactivate();
}

void mrollover::deactivate() {
	up_arrow->deactivate();
	down_arrow->deactivate();
	deactivate_visibles();
}

void mrollover::click() {
	//do we clicked on down arrow ?
	if (down_arrow->activated()) {
		//check if we're not already at bottom
		if (first_entry != num_entries - num_vis_entries) {
			first_entry++;
			for(Uint8 i = 0; i < num_vis_entries; i++) {
				visible_entries[i] = entries[first_entry+i];
				visible_entries[i]->move(position.x, position.y + i*entry_height);
				visible_entries[i]->deactivate();
				game->screen2update = true;
			}
		}
		return;
	}

	//do we clicked on up arrow ?
	if (up_arrow->activated()) {
		//check if we're not already at top
		if (first_entry != 0) {
			first_entry--;
			for(Uint8 i = 0; i < num_vis_entries; i++) {
				visible_entries[i] = entries[first_entry+i];
				visible_entries[i]->move(position.x, position.y + i*entry_height);
				visible_entries[i]->deactivate();
				game->screen2update = true;
			}
		}
		return;
	}
}

Uint8 mrollover::is_in(Sint32 x, Sint32 y) {
		if ((position.x <= x)&&(x <= position.x + position.w)&&(position.y <= y)&&(y <= position.y + position.h)) 
	{
		activate(x, y);
		return true;
	}
	else return false;
}

Sint32 mrollover::get_focused_entry() {
	return focused_entry;
}
