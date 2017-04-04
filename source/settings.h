/*
   Copyright (C) 2004 by Gert-Jan de Boer <boerg@duncan.nl.eu.org>
   Part of the Starlane Empire Project http://home.gna.org/starlane

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef SETTING
#define SETTING
#include"common.h"
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

// Starlane Settings Class
class settings {
	private:
		// Settingstree
	        xmlNode *current_node, *child_node;
        	xmlChar *field_length, *alternate_name;
        	xmlDocPtr settings_file;

	public:
		// Constructor
		settings(char *filename);
		// Get setting from setting file
		char* get_setting(char *element, char *attribute);
		// Destructor
		~settings();
};

#endif
