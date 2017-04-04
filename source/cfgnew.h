/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include"screen.h"
#include"rollover.h"
#include"color_rollover.h"
#include"basic_rollover.h"
#include"starmap.h"
#include"raceobjs.h"
#include<vector>

/**this class handles the creating new game menu*/
class gcfgnew : public gscreen {
	private:
		//!game level rollovers
		static rollover *difficulty;
		//!races table
		static rollover *races;
		//!stellar density
		static rollover *density;
		//! races menu
		static rollover *menu;
		//QADH TODO correct the type of rolloverpointers
		crollover *raceflag;
		//! start button
		rollover *start_button;
		//!current rollover selected (0 = none)
		Uint8 rollnumber;

		//!big starmap !
		starmap* stars;

		//!rollover to choose game color
		rollover *colors;

		//!race ring (picture)
		racering *rr;
		
		//!planet of current race (pictures)
		stack_image *planets;
		
		//!colored flag of current race
		stack_image *raceflags;
		
		//!racename
		image_container* racename;
		
		//!race text
		image_container* pRace_text;
		
		//!race text table
		race_texts* pRace_texts_table;
		
		//!Static Text above Raceselection
		image_container* pRaceselect;
		
		//!Just a Hint for Devs
		image_container* pHint;
		
		//!current race
		Uint32 current_race;

		//!current color
		Uint8 current_color;

		//!pointer on myself (same as this but static)
		static gscreen *myself;

		//!change the currently chosen race
		void change_race(Uint32 newrace);

		//!change the currently chosen color
		void change_color(Uint8 color);

		/*all methods from here are used for lua interface*/
		//!build difficulty rollover 
		static int lua_difficulty_rollover(lua_State *s);
		//!build races rollover 
		static int lua_races_rollover (lua_State *s);
		//!build density rollover 
		static int lua_density_rollover (lua_State *s);
		//!build the races menu
		static int lua_races_menu (lua_State *s);

	public:
		//!constructor
		gcfgnew();
		//!destructor
		~gcfgnew();
		//!handle (see README.hackers)
		void handle();
};
