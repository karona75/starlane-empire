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
#include"cfgnew.h"
#include"state_rollover.h"
#include"menu_rollover.h"
#include"color_rollover.h"

rollover* gcfgnew::difficulty = NULL;
rollover* gcfgnew::races = NULL;
rollover* gcfgnew::density = NULL;
rollover* gcfgnew::menu = NULL;
gscreen* gcfgnew::myself = NULL;

int gcfgnew::lua_difficulty_rollover(lua_State *s) {
	char* a1[3] = {"10_3cfgnew.png", "11_3cfgnew.png", "12_3cfgnew.png"};
	Uint32 x = (Uint32) lua_tonumber(s, 1);
	Uint32 y = (Uint32) lua_tonumber(s, 2);
	char *title = (char*) lua_tostring(s, 3);
	difficulty = new srollover(3,x, y, title, a1);
	difficulty->bind(myself);
	return 0;
}

int gcfgnew::lua_races_rollover (lua_State *s) {
	char* a2[5] = {"5_3cfgnew.png", "6_3cfgnew.png", "7_3cfgnew.png", "8_3cfgnew.png", "9_3cfgnew.png"};
	Uint32 x = (Uint32) lua_tonumber(s, 1);
	Uint32 y = (Uint32) lua_tonumber(s, 2);
	char *title = (char*) lua_tostring(s, 3);
	races = new srollover(5, x, y, title, a2);
	races->bind(myself);
	return 0;
}

int gcfgnew::lua_density_rollover (lua_State *s) {
	char* a3[4] = {"1_3cfgnew.png", "2_3cfgnew.png", "3_3cfgnew.png", "4_3cfgnew.png"};
	Uint32 x = (Uint32) lua_tonumber(s, 1);
	Uint32 y = (Uint32) lua_tonumber(s, 2);
	char *title = (char*) lua_tostring(s, 3);
	density = new srollover(4, x, y, title, a3);
	density->bind(myself);
	return 0;
}

int gcfgnew::lua_races_menu (lua_State *s) {
	//load menu background
	SDL_Surface *ring = game->image_load("7_racering.png");
	
	//provide imagenames of Racelogos
	char* a4[21] = {"0_smraces.png", "1_smraces.png", "2_smraces.png", "3_smraces.png", "4_smraces.png", "5_smraces.png", "6_smraces.png", "7_smraces.png", "8_smraces.png", "9_smraces.png", "10_smraces.png", "11_smraces.png", "12_smraces.png", "13_smraces.png", "14_smraces.png", "15_smraces.png", "16_smraces.png", "17_smraces.png", "18_smraces.png", "19_smraces.png", "20_smraces.png"};

	Uint32 x = (Uint32) lua_tonumber(s, 1);
	Uint32 y = (Uint32) lua_tonumber(s, 2);
	//create races menu
	menu = new mrollover(3, 21, x, y, a4, ring);
	menu->bind(myself);
	return 0;
}



gcfgnew::gcfgnew()
{
#ifdef DEBUG
	cout << "Creating cfgnew" << endl;
#endif
	//initialize some static stuff (remember : only one instance of this class)
	myself = this;

	//init game parameters
	current_race = 0;
	current_color = 0;

	//get background
	image *bcj = new image("0_3cfgnew.png");
	//create image same size in black
	image *background = new image(bcj->get_surface()->w, bcj->get_surface()->h);
	//blit background on black
	SDL_BlitSurface(bcj->get_surface(), NULL, background->get_surface(), NULL);
	delete bcj;
	image_container *imc = new image_container(background);
	imc->bind(this);

	
	//make some C++ accessible from within lua
	game->lua_reg("difficulty_rollover", lua_difficulty_rollover);
	game->lua_reg("races_rollover", lua_races_rollover);
	game->lua_reg("density_rollover", lua_density_rollover);
	game->lua_reg("races_menu", lua_races_menu);
	//launch lua script
	game->lua("cfgnew.lua");

	//color chooser
	colors = new crollover();
	colors->bind(this);

	//race ring
	rr = new racering(345, 40);
	rr->bind(this);

	//home planet
	planets = new stack_image(370, 175, RACES, "_smhome.png");
	planets->bind(this);

	//race flags
	raceflags = new stack_image(435, 190, RACES, RACECOLORS, "_raceflag.png");
	raceflags->bind(this);

	//race names
	racename = new image_container(370, 15, game->get_racename(0), game->bigfont);
	racename->bind(this);
	
	//!gimme object with all Race texts
	pRace_texts_table = new race_texts(game->get_language() );


	//!Paint the short race description
	pRace_text = new image_container(350, 240, pRace_texts_table->get_short_race_desc(0), game->smallfont, 140);

	pRace_text->bind(this);
		
	//!Static Text above Raceselection
	pRaceselect = new image_container(510, 15, "Player's race", game->bigfont);
	pRaceselect->bind(this);
	
	//Just a Hint for Devs
	pHint = new image_container(100, 200, "Starcluster coming soon", game->bigfont);
	pHint->bind(this);

	//display start button
	SDL_Rect start_pos;
	start_pos.x = 502;
	start_pos.y = 381;
	start_pos.w = 131;
	start_pos.h = 92;
	start_button = new brollover("start game", game->bigfont, start_pos);
	start_button->bind(this);
	
	//now eventually activate rollover
	rollnumber = 0;
	Sint32 mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	game->screen2update = true;

}

gcfgnew::~gcfgnew()
{
#ifdef DEBUG
	cout << "destroying cfgnew" << endl;
#endif
	//delete all objects on screen
	for(vector<ose*>::iterator it = oses.begin(); it != oses.end(); it++)
		delete (*it);
}



void gcfgnew::handle()
{
	while(SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_VIDEOEXPOSE:
				game->screen2update = true;
				break;
			case SDL_MOUSEMOTION:
				rollnumber = handle_rollover(event.motion.x, event.motion.y);
				game->mousepointer->move(&event);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				if((rollnumber!=0)&&(rollnumber!=5)&&(rollnumber!=6))
					rollovers[rollnumber-1]->click();
				if(rollnumber==3) {
					//change number of stars displayed
					//srollover* s = (srollover*) rollovers[2];
					//Uint16 sn[5] = {20, 40, 60, 80, 100};
					//stars->reinit(sn[s->get_state()]);
				}
				if(rollnumber==4) {
					mrollover* m = (mrollover*) rollovers[3];
					//we change race
					Sint32 race = m->get_focused_entry();
					if (race >= 0)
						change_race(race);
				}
				//we clicked on color rollover
				if(rollnumber==5) {
					Sint32 x = game->mousepointer->get_x();
					Sint32 y = game->mousepointer->get_y();
					//check and eventually change color on rollover
					crollover *rol = (crollover*) colors;
					Uint8 color = rol->click(x, y);
					//should we change colors ?
					if (color != 255)
						change_color(color);
				}
				//we clicked on start button
				if(rollnumber==6) {
					//init a new game
					srollover* races = (srollover*) rollovers[1];
					srollover* difficulty = (srollover*) rollovers[0];
					game->rules = new rules_engine(3+races->get_state(), current_race, current_color, difficulty->get_state());
					//go to next screen
				}
				break;
			
			case SDL_QUIT:
				game->quit();
				break;
		}
	}


	//adjust stars rotation according to current frame number
	//stars->set_phi(((double) game->frame) / 60);
	//stars->update();
	//game->screen2update = true;
}

void gcfgnew::change_race(Uint32 newrace) {
	current_race = newrace;
	crollover *rol = (crollover*) colors;
	rol->set_image(newrace);
	rr->set_race(newrace);
	planets->set_level(newrace);
	raceflags->set_level(RACES*current_color + current_race);
	racename->set_text(game->get_racename(current_race), game->bigfont);
	pRace_text->set_text(pRace_texts_table->get_short_race_desc(current_race), game->smallfont, 140);
}

void gcfgnew::change_color(Uint8 color)
{
	current_color = color;
	rr->set_color(color);
	raceflags->set_level(RACES*current_color + current_race);
}
