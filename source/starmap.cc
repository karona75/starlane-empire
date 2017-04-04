/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"starmap.h"
#include"math.h"
#include"assistant.h"

//For using the converter -> MD
convert* conv;


starmap::starmap(Uint16 sn, SDL_Rect pos) {
	//copy information
	position.x = pos.x;
	position.y = pos.y;
	position.w = pos.w;
	position.h = pos.h;

	//objects are needed to convert
	conv = new convert;

	stars_number = sn;

	//allocate memory
	stars = (star*) malloc (stars_number * sizeof(star));

	//initialize image
	img = SDL_CreateRGBSurface(SDL_SWSURFACE,
			pos.w,
			pos.h,
			game->screen->format->BitsPerPixel,
			game->screen->format->Rmask,
			game->screen->format->Gmask,
			game->screen->format->Bmask,
			game->screen->format->Amask);

	//fill background
	SDL_FillRect(img, NULL, SDL_MapRGB(img->format, 0, 0, 0));

	//load stars images
	//So, i've automized the loading Process -> MD in the mom 40 images
	for(Uint16 i = 0; i < 10; i++)
	{
		for(Uint16 j = 0; j < 4; j++)
		{
			//TODO: is 25 too static ?
			char filename[25];
			strcpy(filename, (
			conv->int_to_string(i * 4 + j) + "_cos_star.png").c_str());
			star_img[i][j] = game->images->load(filename)->get_surface();
		}
	}
	//initialize stars position
	//TODO: find a better algorithm
	for(Uint16 i = 0; i < stars_number ; i++) {
		stars[i].r = (random() % 40000) + 25535;
		stars[i].phi = random();
		stars[i].theta = random();
		stars[i].iStartype = 0 + (rand() % (9-0+0));
	}


	//initialize global coordinates
	//r = 65535;
	r = 50000;
	phi = 0;
	theta = 0;
}


starmap::~starmap() {
	delete conv;
	free((void*)stars);
	//TODO: free stars images
}

void starmap::reinit(Uint16 sn) {
	//first free the old stars
	free((void*)stars);

	stars_number = sn;

	//allocate new stars
	stars = (star*) malloc (stars_number * sizeof(star));


	//initialize stars position
	//TODO: find a better algorithm
	for(Uint16 i = 0; i < stars_number ; i++) {
		stars[i].r = (random() % 50000) + 15535;
		stars[i].phi = random();
		stars[i].theta = random();
		stars[i].iStartype = 0 + (rand() % (9-0+0));
	}
}


void starmap::display() {
	//game->display(img, NULL, &position);
}

/** do the projections, and update main image*/
void starmap::update() {
	//clear the entire background
	SDL_FillRect(img, NULL, SDL_MapRGB(img->format, 0, 0, 0));

	//for now, don't use gamma
	for(Uint16 i = 0 ; i < stars_number ; i++) {
		Uint16 sr;
		double sphi, stheta;
		Sint16 x, y, z;
		//convert local to global coordinates
		// resize r such that 0 < r < 6553
		double factor = ((double) r) / 65535;
		sr = (Uint16) (stars[i].r * factor);
		sr = sr / 100;
		sphi = stars[i].phi + phi;
		stheta = stars[i].theta + theta;
		//now convert spherical coordinates to standard x,y,z coordinates
		//z is vertical coordinate
		x = (Sint16) (sr * cos(sphi) * cos(stheta));
		//put it back from origin by 1000 units
		y = (Sint16) (sr * sin(sphi) * cos(stheta) + 1000);
		z = (Sint16) (sr * sin(stheta));
		//now project from 3D to 2D
		Sint16 x2, y2;
		//set the Eye
		Sint16 ex, ey, ez;
		ex = 0;
		ey = 0;
		ez = 0;
		//we take as screen a plane of equation y = 300
		Uint16 yplane = 300;
		//now we can compute the projection
		//first find the line parameter
		float t = ((float)(yplane - ey)) / (y - ey);
		//we can now compute the coordinates
		x2 = (Sint16) (ex + t*(x - ex));
		y2 = (Sint16) (ez + t*(z - ez));
		//now we translate coordinates accordingly to starmap height/width
		x2 += img->w/2;
		y2 += img->h/2;
		//do some clipping
		if ((0<=x2)&&(x2<=img->w)&&(0<=y2)&&(y2<=img->h)) {
			//TODO: for now a QADH with everything static, change that
			SDL_Rect dest;
			dest.x = x2;
			dest.y = y2;
			dest.w = star_img[stars[i].iStartype][0]->w;
			dest.h = star_img[stars[i].iStartype][0]->h;
			Uint8 gamma = 0;
			if (y<800) gamma = 3;
			if ((y>=800)&&(y<1000)) gamma = 2;
			if ((y>=1000)&&(y<1200)) gamma = 1;
			if (y>=1200) gamma = 0;
		//	SDL_BlitSurface(star_img[stars[i].iStartype][gamma], NULL, img, &dest);

		}
	}
}

void starmap::deactivate() {
}

void starmap::activate(Sint32 x, Sint32 y) {
}

void starmap::click() {};

Uint8 starmap::is_in(Sint32 x, Sint32 y) {
	if ((x < position.x)||(x > position.x + position.w)||(y < position.y)||(y > position.y + position.h)) {
		return false;
	}
	return true;
}

void starmap::set_r(Uint16 new_r) {
	r = new_r;
}

void starmap::set_phi(double new_phi) {
	phi = new_phi;
}

void starmap::set_theta(double new_theta) {
	theta = new_theta;
}

double starmap::get_phi() {
	return phi;
}

bool starmap::activated () {
	return false;
}
