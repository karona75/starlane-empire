/*
   Copyright (C) 2004 by Mario Diehl <spider@meteorsoft.de>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "assistant.h"

// the 'int_to_string' function --> very useful
std::string convert::int_to_string(int value)
{
	std::ostringstream streamOut;
	streamOut << value;
	return streamOut.str();
}

