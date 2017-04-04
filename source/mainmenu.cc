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
#include"mainmenu.h"

gscreen* gmainmenu::myself = NULL;
rollover* gmainmenu::main_menu_entries[7];
Uint32 gmainmenu::last_entry = 0;

int gmainmenu::lua_menu_entry (lua_State *s) {
	SDL_Rect pos;
	//get parameters from lua
	pos.x = (Uint32) lua_tonumber(s, 1);
	pos.y = (Uint32) lua_tonumber(s, 2);
	pos.w = (Uint32) lua_tonumber(s, 3);
	pos.h = (Uint32) lua_tonumber(s, 4);
	char *title = (char*) lua_tostring(s, 5);
	//create rollover
	main_menu_entries[last_entry] = new brollover(title, game->bigfont, pos);
	main_menu_entries[last_entry]->bind(myself);
	//get ready for next rollover
	last_entry++;
	return 0;
}

gmainmenu::gmainmenu()
{
	//recognize upper truth
	myself = this;
	//load background
	image *background = new image("0opening.gif");
	//draw a tux on it :)
	image *tux = game->images->load("tux.png");
	SDL_Rect r;
	r.x = 500;
	r.y = 300;
	r.w = 130;
	r.h = 150;

	SDL_SetAlpha(tux->get_surface(), SDL_SRCALPHA|SDL_RLEACCEL, 124);
	SDL_BlitSurface(tux->get_surface(), NULL, background->get_surface(), &r);
	image_container *i = new image_container(background);
	i->bind(this);

	//create the menu texts 

	game->lua_reg("menu_entry", lua_menu_entry);
	//launch lua script
	game->lua("mainmenu.lua");

	//enable mouse
	game->mousepointer->enable();
	//at start none activated since mouse is at 0,0
	rollover_activated = 0;
	focused_rollover = NULL;
	
	game->images->set_gamma(1.0f);
}

gmainmenu::~gmainmenu()
{
	for(vector<ose*>::iterator it = oses.begin(); it != oses.end() ; it++)
		delete (*it);
}

void
 gmainmenu::handle()
{

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_VIDEOEXPOSE:
			game->screen2update = true;
			break;
		case SDL_MOUSEMOTION:
			rollover_activated = handle_rollover(event.motion.x, event.motion.y);
			game->mousepointer->move(&event);
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (rollover_activated) {
			case 7:
				game->sound_engine->play_sound("button.wav");
				game->quit();
				break;
			case 3:
				game->sound_engine->play_sound("button.wav");
				game->cfgnew();
				break;
			case 1:
				game->sound_engine->play_sound("button.wav");
				game->intro();
				break;
			}
			break;

		case SDL_KEYDOWN:

			switch (event.key.keysym.sym) {
			case SDLK_x:
				if (event.key.keysym.mod & KMOD_LALT)
					game->quit();
				break;
			case SDLK_i:
				game->intro();
				break;
			case SDLK_n:
				game->cfgnew();
				break;
			default:
				break;
			}
			break;
		}
	}
}
