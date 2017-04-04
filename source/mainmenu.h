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
#include"basic_rollover.h"


/**this class handles the main game menu*/
class gmainmenu : public gscreen {
	private:
		//!rollovers on menu
		static rollover *main_menu_entries[7];
		//!number of the activated rollover, 0 if none
		Uint8 rollover_activated;
		//!pointer on myself (same as this but static)
		static gscreen *myself;
		//!counter for number of rollovers
		static Uint32 last_entry;
		//!lua method to create rollovers
		static int lua_menu_entry(lua_State *s);

	public:
		//!constructor, loads images,...
		gmainmenu();
		//!destructor, frees images,...
		~gmainmenu();
		//!handle (see README.Hackers)
		void handle();
		
};
