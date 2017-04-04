/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"color_rollover.h"

crollover::crollover() {

	SDL_Surface *logos;
	SDL_Rect tmp_pos;
	
	position.x = 379;
	position.y = 382;
	position.w = 116;
	position.h = 93;

	current_image = 0;

	//add text
	SDL_Surface *text = game->bigfont->render_text("Colors");

	//allocate imgs space
	imgs_activated = (image **) malloc(RACES*sizeof(image*));
	imgs_deactivated = (image **) malloc(RACES*sizeof(image*));


	//allocate all logos surface
	//the surface contains all different colors
	logos = SDL_CreateRGBSurface(SDL_SWSURFACE,
			position.w,
			position.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	//set transparent color key
	SDL_SetColorKey(logos, SDL_SRCCOLORKEY, SDL_MapRGB(logos->format, 0, 254, 0));
	

	char* filename = (char*) malloc ((3+strlen("_raceflag.png"))*sizeof(char));
	for(unsigned int i = 0 ; i < RACES ; i++) {

		//fill with a transparent color
		SDL_FillRect(logos, NULL, SDL_MapRGB(logos->format, 0, 254, 0));

		snprintf(filename, 16, "%d_raceflag.png", i);
		//load initial race logo in all colors
		SDL_Surface **logo = game->color_images_load(filename);
	
		tmp_pos.x = 3;
		tmp_pos.y = 34;
		tmp_pos.w = logo[0]->w;
		tmp_pos.h = logo[0]->h;
		for(unsigned int j = 0 ; j < RACECOLORS ; j++) {
			SDL_BlitSurface(logo[j], NULL, logos, &tmp_pos);
			tmp_pos.x += 16;
		}

		for(unsigned int k = 0 ; k < RACECOLORS ; k++)
			SDL_FreeSurface(logo[k]);
		free(logo);

		//init img

		SDL_Surface* tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE,
				position.w,
				position.h,
				game->screen->format->BitsPerPixel,
				game->screen->format->Rmask,
				game->screen->format->Gmask,
				game->screen->format->Bmask,
				game->screen->format->Amask);
		
		imgs_deactivated[i] = new image(tmp2);

		SDL_FillRect(tmp2, NULL, SDL_MapRGB(tmp2->format, 12, 32, 48));


		tmp_pos.y = 5;
		tmp_pos.x = (tmp2->w - text->w) / 2;
		tmp_pos.w = text->w;
		tmp_pos.h = text->h;

		SDL_BlitSurface(text, NULL, tmp2, &tmp_pos);

		
		//blit logos on target surface 
		SDL_BlitSurface(logos, NULL, tmp2, NULL);
		//create corresponding activated img
		imgs_activated[i] = imgs_deactivated[i]->create_activated_image();

	
	}
	free(filename);
	free(logos);


	//allocate img_activated and img_deactivated
	
	SDL_Surface* ia = SDL_CreateRGBSurface(SDL_SWSURFACE,
			position.w,
			position.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	img_activated = new image(ia);

	SDL_Surface* id = SDL_CreateRGBSurface(SDL_SWSURFACE,
			position.w,
			position.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	img_deactivated = new image(id);

	//blit images onto it
	SDL_BlitSurface(imgs_activated[current_image]->get_surface(), NULL, ia, NULL);
	SDL_BlitSurface(imgs_deactivated[current_image]->get_surface(), NULL, id, NULL);
	
	//modify images to show which color is currently selected
	set_color(0);
	img = img_deactivated;
	display();
}

crollover::~crollover() {
	for(Uint32 i = 0 ; i < RACECOLORS ; i++) {
		delete imgs_activated[i];
		delete imgs_deactivated[i];
	}
	delete img_activated;
	delete img_deactivated;
	free(imgs_deactivated);
	free(imgs_activated);
}

void crollover::deactivate() {
	img = img_deactivated;
	display();
}

void crollover::activate(Sint32 x, Sint32 y) {
	img = img_activated;
	display();
}

void crollover::display() {
	img->display(&position);
}

Uint8 crollover::click(Sint32 x, Sint32 y) {
	Uint8 new_color = 255;
	//go to local coordinates
	x -= position.x;
	y -= position.y;
	//check if we clicked on a color
	for(Uint8 color = 0; color < RACECOLORS ; color++) {
		SDL_Rect box;
		box.x = 2+16*color;
		box.y = 33;
		box.w = 17;
		box.h = 27;
		if ((x > box.x)&&(x < box.x + box.w)&&(y > box.y)&&(y < box.y + box.h)) {
			new_color = color;
			break;
		}
	}

	//if we don't change color simply ignore that we got the same
	if (new_color == current_color) new_color = 255;
	//check and change rollover state
	if (new_color != 255) set_color(new_color);
	
	//return the color we clicked on for changes on cfgnew
	return new_color;
}

Uint8 crollover::is_in(Sint32 x, Sint32 y) {
	if ((x >= position.x)&&(x <= position.x + position.w)&&(y >= position.y)&&(y <= position.y + position.h)) {
		activate(x,y);
		return true;
	}
	return false;
}

bool crollover::activated() {
	if (img == img_activated) return true;
	return false;
}

void crollover::set_image(Uint32 level) {
	current_image = level;
	SDL_BlitSurface(imgs_activated[current_image]->get_surface(), NULL, img_activated->get_surface(), NULL);
	SDL_BlitSurface(imgs_deactivated[current_image]->get_surface(), NULL, img_deactivated->get_surface(), NULL);
	set_color(current_color);
	game->screen2update = true;
}

void crollover::set_color(Uint32 color) {
	current_color = color;
	//convert images to original status
	SDL_BlitSurface(imgs_activated[current_image]->get_surface(), NULL, img_activated->get_surface(), NULL);
	SDL_BlitSurface(imgs_deactivated[current_image]->get_surface(), NULL, img_deactivated->get_surface(), NULL);
	//put a new white box on corresponding color
	SDL_Rect box;
	box.x = 2+16*color;
	box.y = 33;
	box.w = 17;
	box.h = 27;
	SDL_Color col;
	col.r = 255;
	col.g = 255;
	col.b = 255;
	img_activated->draw_box(box, col);
	img_deactivated->draw_box(box, col);
	game->screen2update = true;
}

SDL_Surface* crollover::get_img()
{
	return img->get_surface();
}
