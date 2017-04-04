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
#include"introduction.h"
#include"cfgnew.h"
#include"intro.h"
using namespace std;
void se::gl_enable2D()
{
 int vPort[4];

 glGetIntegerv(GL_VIEWPORT, vPort);

 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
 glLoadIdentity();

 glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
 glLoadIdentity();
}

void se::gl_disable2D()
{
 glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(GL_MODELVIEW);
 glPopMatrix();
}



static int eventfilter(const SDL_Event * event)
{
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_RETURN:
			if (event->key.keysym.mod & KMOD_ALT)
				game->fullscreen();
			return 0;
		default:
			break;
		}
	}
	return 1;
}

static SDL_Surface *LoadIconSurface(char *file, Uint8 ** maskp)
{
	SDL_Surface *icon;
	Uint8 *pixels;
	Uint8 *mask;
	Uint32 mlen, i;

	*maskp = NULL;

	/* Load the icon surface */
	icon = SDL_LoadBMP(file);
	if (icon == NULL) {
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return (NULL);
	}

	/* Check width and height */
	if ((icon->w % 8) != 0) {
		fprintf(stderr, "Icon width must be a multiple of 8!\n");
		SDL_FreeSurface(icon);
		return (NULL);
	}
	if (icon->format->palette == NULL) {
		fprintf(stderr, "Icon must have a palette!\n");
		SDL_FreeSurface(icon);
		return (NULL);
	}

	/* Set the colorkey */
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, *((Uint8 *) icon->pixels));

	/* Create the mask */
	pixels = (Uint8 *) icon->pixels;
	/*printf("Transparent pixel: (%d,%d,%d)\n",
	   icon->format->palette->colors[*pixels].r,
	   icon->format->palette->colors[*pixels].g,
	   icon->format->palette->colors[*pixels].b); */
	mlen = icon->w * icon->h;
	mask = (Uint8 *) malloc(mlen / 8);
	if (mask == NULL) {
		fprintf(stderr, "Out of memory!\n");
		SDL_FreeSurface(icon);
		return (NULL);
	}
	memset(mask, 0, mlen / 8);
	for (i = 0; i < mlen;) {
		if (pixels[i] != *pixels)
			mask[i / 8] |= 0x01;
		++i;
		if ((i % 8) != 0)
			mask[i / 8] <<= 1;
	}
	*maskp = mask;
	return (icon);
}


/** Initialize sound card, and display */
se::se()
{
	//!XML Settings
	settings *settings_engine;
	SDL_Color clrFg,clrBg;
	Uint8 *iconmask;
#ifdef DEBUG
	cout << "Initializing XML Settings" << endl;
#endif
	// Read XML Settings
	settings_engine = new settings("settings.xml");
#ifdef DEBUG
	cout << "Initializing SDL" << endl;
#endif
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}
	
	set_language(settings_engine->get_setting("lang", "current") );

#ifdef DEBUG
	cout << "Opening video" << endl;
#endif
	SDL_WM_SetCaption("Starlane Empire", "Starlane Empire");
	SDL_Surface *icon = LoadIconSurface(image::rename("icon.bmp"), &iconmask);
	SDL_WM_SetIcon(icon, iconmask);

	screen = SDL_SetVideoMode(640, 480, 0, SDL_OPENGL);
	if (screen == NULL) {
		cout << "Unable to set video mode: " << SDL_GetError() << endl;
		exit(2);
	}
	//init GL
	glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    glViewport(0, 0, screen->w, screen->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, screen->w / (float )screen->h, 0.1f, 50.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	

	//initializing font engine and loading fonts
#ifdef DEBUG
	cout << "initializing fonts" << endl;
#endif
	
	TTF_Init();
	// Get Font settings from settings.xml
	clrFg.r = 255; clrFg.g = 255; clrFg.b = 255; clrFg.unused = 0;
	clrBg.r = 0; clrBg.g = 254; clrBg.b = 0; clrFg.unused = 0;
	bigfont = new font(settings_engine->get_setting("font","bigfont"), clrFg, clrBg, atoi(settings_engine->get_setting("font","bigsize")));
	clrFg.r = 0; clrFg.g = 0; clrFg.b = 240; clrFg.unused = 0;
	clrBg.r = 0; clrBg.g = 254; clrBg.b = 0; clrBg.unused = 0;
	smallfont = new font(settings_engine->get_setting("font","smallfont"), clrFg, clrBg, atoi(settings_engine->get_setting("font","smallsize")));
	TTF_Quit();
	//stopping settings!
	delete settings_engine;
	//init event loop filter
	SDL_SetEventFilter(&eventfilter);

#ifdef DEBUG
	cout << "opening lua" << endl;
#endif
	lst = luaL_newstate();
	luaL_openlibs(lst);
	//lua_iolibopen(lst);
	//lua_strlibopen(lst);
	//lua_mathlibopen(lst);

	//init sound
	sound_engine = new sound();

	screen2update = false;
	sintroduction = NULL;
	smainmenu = NULL;
	scfgnew = NULL;
	sintro = NULL;

	//init image engine
	images = new image_engine();

	//init mouse
	mousepointer = new mouse();
}

/** shuts down audio and video, and freeing images, music*/
se::~se()
{
#ifdef DEBUG
	cout << "Shutting down game" << endl;
#endif
	delete sound_engine;

	lua_close(lst); //ooooh schon fertig

	//stopping SDL
	SDL_Quit();
	exit(0);
}

/** loads an image 
 * \param filename : relative filename */
SDL_Surface *se::image_load(char* filename)
{
#ifdef DEBUG
	if (filename == NULL) {
		cerr << "Warning : trying to load an unknown image" << endl;
		return NULL;
	}
#endif
	char* realname = image::rename(filename);
	SDL_Surface *image = IMG_Load(realname);
#ifdef DEBUG
	if (image == NULL) {
		cerr << "Warning : unable to load : " << realname << " : " <<
			SDL_GetError() << endl;
		return NULL;
	}
#endif
	free(realname);
	//TODO: should I use a colorkey on non transparent images ?? slowing things ??
	SDL_SetColorKey(image, SDL_SRCCOLORKEY,
					SDL_MapRGB(image->format, 0, 254, 0));
	SDL_Surface *final = SDL_DisplayFormat(image);	//TODO: is that always 16bits ????
	SDL_FreeSurface(image);
	return final;
}

/**loads number of colors images
 * \param filename : base filename
 * for example "0_raceflag.png" will load "a0_raceflag.png" "b0_raceflag.png"
 * ..... */
SDL_Surface** se::color_images_load(char* filename) {
	SDL_Surface** result = (SDL_Surface**) malloc(RACECOLORS*sizeof(SDL_Surface*));
	char* realname = (char*) malloc((strlen(filename)+2)*sizeof(char));
	char letter = 'a';
	realname[0] = 1;
	realname[1] = 0;
	strcat(realname, filename);
	for(unsigned int i = 0 ; i < RACECOLORS ; i++) {
		realname[0] = letter;
		result[i] = image_load(realname);
		letter++;
	}
	free(realname);
	return result;
}


/**converts filename for matching installation path,
 * remember to free strings manually 
 * \param shortname: the relative filename
 * we first try the currently chosen theme
 * and if the file doesn't exist we fall back to the default theme*/
char* se::rename(char* shortname) {
	char *basepath;
	char *themedir;
	//!XML Settings
	settings *settings_engine;
	Uint32 size;
	settings_engine = new settings("settings.xml");
	//TODO: get rid of all that static stuff 
	// It's gone.
	basepath = settings_engine->get_setting("tileset","path");
	//basepath="../themes/";
	themedir = settings_engine->get_setting("tileset","current");
	//themedir="starlane/";
	delete settings_engine;
	size = 1 + strlen(basepath) + strlen (themedir) + strlen("data/") + strlen(shortname);
	char* realname = (char *) malloc(size * sizeof(char));
	realname[0] = 0;
	strcat(realname, basepath);
	strcat(realname, themedir);
	strcat(realname, "data/");
	strcat(realname, shortname);
	//now test if file exist and we can read it
	if (access(realname, R_OK) == 0)	
		return realname;
	//didn't work so use default theme instead
	//first free old name
	free(realname);
	//themedir = settings_engine->get_setting("default");
	themedir = "ascend/";

	size = 1 + strlen(basepath) + strlen (themedir) + strlen("data/") + strlen(shortname);
	realname = (char *) malloc(size * sizeof(char));
	realname[0] = 0;
	strcat(realname, basepath);
	strcat(realname, themedir);
	strcat(realname, "data/");
	strcat(realname, shortname);
	return(realname);
}


/** updates the screen flip or updaterect */
void se::update()
{

	// Clear the buffers and load the identity matrix.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Enable 2D drawing mode. (ortho mode)
    gl_enable2D();

    // Disable depth testing.
    glDisable(GL_DEPTH_TEST);


	if (screen2update == true) {
		game->gamescreen->display();
		SDL_GL_SwapBuffers();
	}
	screen2update = false;
	
	// Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Back to 3D mode.
    gl_disable2D();


}

void se::quit()
{
	cout << "Quitting game" << endl;
	delete game;
}

void se::fullscreen()
{
	if (SDL_WM_ToggleFullScreen(screen)) {
		cout << "Toggled fullscreen mode - now "
			<< ((screen->flags & SDL_FULLSCREEN) ? "fullscreen" :
				"windowed")
			<< endl;
		screen2update = true;
	} else {
		cout << "Unable to toggle fullscreen mode" << endl;
	}
}


void se::handle()
{
	gamescreen->handle();
}

void se::introduction()
{
	sintroduction = new gintroduction();
	gamescreen = sintroduction;
}

void se::mainmenu()
{
#ifdef DEBUG
	cout << "Starting mainmenu" << endl;
#endif
	
	 sound_engine->play_music("theme00.ogg");

	//first time ??
	if (smainmenu == NULL) {
		//free introduction structures
		if (sintroduction != NULL)
			delete sintroduction;
		//start new menu
		smainmenu = new gmainmenu();
	}
	if(sintro != NULL)
		delete sintro;
	game->framerate = 35;
	smainmenu->display();
	gamescreen = smainmenu;
	//flush event queue
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
	};
}

void se::cfgnew()
{
#ifdef DEBUG
	cout << "Starting new game" << endl;
#endif
	if (scfgnew == NULL) {
		scfgnew = new gcfgnew();
		gamescreen = scfgnew;
		//flush event queue
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
		};
	}
}

void se::intro()
{
#ifdef DEBUG
	cout << "Starting a new introduction video" << endl;
#endif

	glDisable(GL_DEPTH_TEST);
	sintro = new gintro();
	gamescreen = sintro;
	glEnable(GL_DEPTH_TEST);
}

void se::lua(char *filename) {
//lua_dofile(lst, filename);
}

void se::lua_reg(char *name, lua_CFunction function) {
	lua_register(lst, name, function);
}

char* se::get_racename(Uint32 number) {
		//TODO: change this from static to dynamic
		char *racenames[RACES] = {"Minions", "Snovemdomas", "Orfa", "Kambucha", "Hanshaks", "Fludentri", "Baliflids", "Swaparamans", "Frutmaka", "Shevar", "Govorom", "Ungooma", "Dubtaks", "Capelons", "Mebes", "Oculons", "Arbryls", "Marmosians", "Chronomyst", "Chamachies", "Nimbuloids"};
	return racenames[number];
}

char* se::get_language()
{
	return language;
}

void se::set_language(char* lang)
{
	language = lang;
}
