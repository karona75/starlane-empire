/*
   Copyright (C) 2005 by Wagner Frederic <wagnerf@loria.fr>>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/*all races images classes : images changing with color
 * and race */

#include "raceobjs.h"
#include "se.h"

racering::racering(Uint32 x, Uint32 y) {
	color = 0;
	race = 0;
	n[0] = "0_smraces.png";
	n[1] = "1_smraces.png";
	n[2] = "2_smraces.png"; 
	n[3] = "3_smraces.png";
	n[4] = "4_smraces.png";
	n[5] = "5_smraces.png";
	n[6] = "6_smraces.png";
	n[7] = "7_smraces.png";
	n[8] = "8_smraces.png";
	n[9] = "9_smraces.png";
	n[10] = "10_smraces.png";
	n[11] = "11_smraces.png";
	n[12] = "12_smraces.png";
	n[13] = "13_smraces.png";
	n[14] = "14_smraces.png";
	n[15] = "15_smraces.png";
	n[16] = "16_smraces.png";
	n[17] = "17_smraces.png";
	n[18] = "18_smraces.png";
	n[19] = "19_smraces.png";
	n[20] = "20_smraces.png";

	r[0] = "a0_racering.png";
	r[1] = "b0_racering.png";
	r[2] = "c0_racering.png";
	r[3] = "d0_racering.png";
	r[4] = "e0_racering.png";
	r[5] = "f0_racering.png";
	r[6] = "g0_racering.png";
	
	position.x = x;
	position.y = y;
	position.w = game->images->get(r[0])->get_surface()->w;
	position.h = game->images->get(r[0])->get_surface()->h;
	raceimg_position.x = position.x + 5;
	raceimg_position.y = position.y + 5;
	raceimg_position.w = position.w - 10;
	raceimg_position.h = position.h - 10;
}

racering::~racering() {
}

void racering::set_color(Uint32 color_number) {
	color = color_number;
}

void racering::set_race(Uint32 race_number) {
	race = race_number;
}

void racering::display() {
	game->images->display(n[race], &raceimg_position);
	game->images->display(r[color], &position);
}

void racering::bind(gscreen *screen) {
	screen->add_ose(this);
}

/**this is the constructor for the stack_image class
 * this class represents an image with contents changing between
 * several images (for exemple planets in the cfgnew screen)
 * \par Uint32 x : x coordinates
 * \par Uint32 y : y coordinates
 * \par Uint32 size : number of images on the stack
 * \par char** img_names : names of all images 
 * PLEASE NOTE THAT img_names are NOT COPIED and shall not be freed
 * after instanciating the new object
 * also note that all images are supposed to have the SAME SIZE*/
stack_image::stack_image(Uint32 x, Uint32 y, Uint32 size, char** img_names) {
#ifdef DEBUG
	cout<<"creating new stacked image at "<<x<<","<<y<<endl;
#endif
	names = img_names;
	max_level = size;
	current_level = 0;
	position.x = x;
	position.y = y;
	position.w = game->images->get(names[0])->get_surface()->w;
	position.h = game->images->get(names[0])->get_surface()->h;
}

/**this is the constructor for the stack_image class
 * this class represents an image with contents changing between
 * several images (for exemple planets in the cfgnew screen)
 * \par Uint32 x : x coordinates
 * \par Uint32 y : y coordinates
 * \par Uint32 size : number of images on the stack
 * \par char* basename : common part of the name for all images
 * for example if size is 3 and basename is "_smhome.png"
 * images are "0_smhome.png" "1_smhome.png" and "2_smhome.png" 
 * also note that all images are supposed to have the SAME SIZE*/

stack_image::stack_image(Uint32 x, Uint32 y, Uint32 size, char* basename) {
#ifdef DEBUG
	cout<<"creating new stacked image at "<<x<<","<<y<<endl;
#endif
	names = (char**) malloc(size*sizeof(char*));
	for (Uint32 i = 0 ; i < size ; i++) {
		Uint32 stringsize = size/10 + strlen(basename) + 1;
		names[i] = (char*) malloc (stringsize);
		snprintf(names[i], stringsize, "%d%s", i, basename);
	}
	max_level = size;
	current_level = 0;
	position.x = x;
	position.y = y;
	position.w = game->images->get(names[0])->get_surface()->w;
	position.h = game->images->get(names[0])->get_surface()->h;
}

/**this is the constructor for the stack_image class
 * this class represents an image with contents changing between
 * several images (for exemple planets in the cfgnew screen)
 * \par Uint32 x : x coordinates
 * \par Uint32 y : y coordinates
 * \par Uint32 size : number of images on the stack
 * \par Uint32 colors : number of different colors for each image
 * WARNING : the real total number of images is size * colors
 * \par char* basename : common part of the name for all images
 * for example if size is 2, color is 2 and basename is "_raceflag.png"
 * images are "a0_raceflag.png" "a1_raceflag.png" "b0_raceflag.png" "b1_raceflag.png"
 * also note that all images are supposed to have the SAME SIZE*/

stack_image::stack_image(Uint32 x, Uint32 y, Uint32 size, Uint32 colors, char* basename) {
#ifdef DEBUG
	cout<<"creating new stacked image at "<<x<<","<<y<<endl;
#endif
	names = (char**) malloc(size*colors*sizeof(char*));
	for (Uint32 c = 0 ; c < colors ; c++)
		for (Uint32 i = 0 ; i < size ; i++) {
			Uint32 stringsize = size/10 + strlen(basename) + 3;
			names[c*size+i] = (char*) malloc (stringsize);
			snprintf(names[c*size+i], stringsize, "%c%d%s", 'a'+c, i, basename);
		}
	max_level = size*colors;
	current_level = 0;
	position.x = x;
	position.y = y;
	position.w = game->images->get(names[0])->get_surface()->w;
	position.h = game->images->get(names[0])->get_surface()->h;
}

stack_image::~stack_image() {
	//don't free any image since they are maybe used somewhere else in game
	for (Uint32 i = 0;  i < max_level; i++) 
		free((void*)names[i]);
	free((void*)names);
}

void stack_image::set_level(Uint32 level) {
#ifdef DEBUG
	if (level >= max_level) cerr<<"stack_image::set_level : WARNING: level set higher than max allowed\n"<<endl;
#endif
	current_level = level%max_level;
}

void stack_image::next_level() {
	current_level++;
	current_level = current_level % max_level;
}

void stack_image::display() {
	game->images->display(names[current_level], &position);
}

void stack_image::bind(gscreen *screen) {
	screen->add_ose(this);
}

race_texts::race_texts(char* lang)
{
	settings_lang = new settings("settings.xml");	
	char* file;
	file = settings_lang->get_setting("lang", "path");
	strcat(file, lang);
	strcat(file, "short_race_desc.txt");

cout << file << endl;

	load_short_race_desc(file);
}

void race_texts::load_short_race_desc(char* file)
{
	//QADH hmm nearly not anymore
	//TODO rename the dirty_little_helper
	int count = 0;
	cout << "trying to load racetexts" << endl;
	pstrShort_race_desc = (char**) malloc(21*sizeof(char*));
	
	ifShort_race_desc.open(file);

	if(ifShort_race_desc == NULL)
	{
		cout << "Can't open " << file << endl;
		return;
	}
	
	ifShort_race_desc.seekg(0, ios::beg);
	while( count < 21 )
	{	
		ifShort_race_desc.getline(dirty_little_helper, 210);
		dirty_little_helper[strlen(dirty_little_helper)-1] = '\0';
		pstrShort_race_desc[count] = (char*) malloc(sizeof(dirty_little_helper));
		strcpy(pstrShort_race_desc[count], dirty_little_helper);		
		count++;
	}
	ifShort_race_desc.close();

	if(pstrShort_race_desc != NULL)
	{
		cout << "Successfully loaded racetexts" << endl;
	}
	
}

race_texts::~race_texts()
{
	delete pstrShort_race_desc;
}

char* race_texts::get_short_race_desc(Uint32 index)
{
	cout << "return racetexts" << endl;
	return pstrShort_race_desc[index];
}
