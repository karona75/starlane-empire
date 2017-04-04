/*
   Copyright (C) 2005 by Pablo Nicolás Núñez Pölcher <talosds@gmx.de>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "flc_decoder.h"
#include "image.h"

#define READ16(out, in) *((Uint16 *)out) = ((Uint8 )*(in + 1) << 8) + (Uint8 )*(in);
#define READ32(out, in) *((Uint32 *)out) = (((((((Uint8 )*(in + 3) << 8) + \
				((Uint8 )*(in + 2))) << 8) + \
				((Uint8 )*(in + 1))) << 8) + \
				(Uint8 )*(in));
using namespace std;

int nextpoweroftwo(int);
int _round(double);

flc_decoder::flc_decoder() {
	flc_stream	= NULL;
	image_surface	= NULL;
	frame_number	= 0;
	last_ticks	= 0;
	palette		= new FLIC_PALETTE[256];
	vPort		= new GLint[4];
	
	glGetIntegerv(GL_VIEWPORT, vPort);
}

flc_decoder::~flc_decoder() {
	if(flc_stream != NULL) flc_stream->close();
	if(image_surface != NULL) SDL_FreeSurface(image_surface);
	delete palette;
	delete vPort;
}

void flc_decoder::decode_black() {
	memset(image_surface->pixels, 0,
		image_surface->pitch * header->height);
}

void flc_decoder::decode_color256() {
	Uint16 packet_count, count_value;
	Uint8 skip_count;
	Uint16 palette_position = 0;
	Uint8 *source_buffer = (Uint8 *)(chunk_buffer + chunk_pointer);

	READ16(&packet_count, source_buffer);
	source_buffer += 2;

	while(packet_count--) {
		skip_count = *(source_buffer++);
		count_value = *(source_buffer++);
		if(!count_value) count_value = 256;

		palette_position += (Uint16 )skip_count;
		while(count_value--) {
			palette[palette_position].red = *(source_buffer++);
			palette[palette_position].blue = *(source_buffer++);
			palette[palette_position].green = *(source_buffer++);
			palette_position++;
		}
	}
}

void flc_decoder::decode_ss2() {
	Uint8 *source_buffer = (Uint8 *)(chunk_buffer + chunk_pointer);
	Uint8 *target_buffer = (Uint8 *)image_surface->pixels;
	Uint8 *temp_target;
	Uint16 lines, color_index, color_index2;
	Sint16 count;
	Uint8 column_skip;
	Sint8 count_data;

	READ16(&lines, source_buffer);
	source_buffer += 2;

	while(lines--) {
		READ16(&count, source_buffer);
		source_buffer += 2;

		while(count & 0xc000) {
			// Bits 14 and 15 = 11, skip lines.
			if((count & 0xc000) == 0xc000) {
				target_buffer +=
					(-count) * image_surface->pitch;
			}

			// Bits 14 and 15 =  01, last pixel.
			if((count & 0xc000) == 0x4000) {
				// TODO: implement last pixel.
			}

			READ16(&count, source_buffer);
			source_buffer += 2;
		}

		if((count & 0xc000) == 0x0000) {
			temp_target = target_buffer;
			while(count--) {
				column_skip = *(source_buffer++);
				temp_target += column_skip * 4;
				count_data = *(source_buffer++);
				if(count_data > 0) {
					while(count_data--) {
						color_index = *(source_buffer++);

						*(temp_target++) =
							palette[color_index].red;
						*(temp_target++) =
							palette[color_index].blue;
						*(temp_target++) =
							palette[color_index].green;
						*(temp_target++) = 255; // alpha.

						color_index = *(source_buffer++);

						*(temp_target++) =
    							palette[color_index].red;
						*(temp_target++) =
							palette[color_index].blue;
						*(temp_target++) =
							palette[color_index].green;
						*(temp_target++) = 255; // alpha.
					}
				} else {
					if(count_data < 0) {
//						count_data = (0x100 - count_data);
						color_index = *(source_buffer++);
						color_index2 = *(source_buffer++);

						while(count_data++) {
						*(temp_target++) =
							palette[color_index].red;
						*(temp_target++) =
							palette[color_index].green;
						*(temp_target++) =
							palette[color_index].blue;
						*(temp_target++) = 255; // alpha.

						*(temp_target++) =
							palette[color_index2].red;
						*(temp_target++) =
							palette[color_index2].green;
						*(temp_target++) =
							palette[color_index2].blue;
						*(temp_target++) = 255; // alpha.
						}
					}
				}
			}
			target_buffer += image_surface->pitch;
		}
	}
}

void flc_decoder::decode_copy() {
	Uint16 lines = header->height, color_index, width;
	Uint8 *source_buffer = (Uint8 *)(chunk_buffer + chunk_pointer);
	Uint8 *target_buffer = (Uint8 *)image_surface->pixels;

	while(lines-- > 0) {
		width = header->width;

		while(width-- > 0) {
			color_index	= *(source_buffer++);

			*(target_buffer++)	= 255;
			*(target_buffer++)	= palette[color_index].blue;
			*(target_buffer++)	= palette[color_index].green;
			*(target_buffer++)	= palette[color_index].red;
		}
	}
}

void flc_decoder::decode_color64() {
}

void flc_decoder::decode_lc() {
	Uint8 *source_buffer;
	Uint8 *target_buffer;
	Uint8 *temp_target;
	Sint8 data_count;
	Uint8 skip_count, color_index;
	Uint16 lines, tmp;
	int packet_count;
	Uint16 depth = image_surface->format->BytesPerPixel;

	source_buffer = (Uint8 *)(chunk_buffer + chunk_pointer);
	target_buffer = (Uint8 *)image_surface->pixels;

	READ16(&tmp, source_buffer);
	source_buffer += 2;
	target_buffer += tmp * image_surface->pitch;

	READ16(&lines, source_buffer);
	source_buffer += 2;

	while(lines--) {
		temp_target	= target_buffer;
		packet_count	= *(source_buffer++);
		while(packet_count--) {
			skip_count = *(source_buffer++);
			temp_target += (skip_count * depth);
			data_count = *(source_buffer++);
			if(data_count > 0) {
				while(data_count--) {
					color_index = *(source_buffer++);
					*(temp_target++) =
						palette[color_index].red;
					*(temp_target++) =
						palette[color_index].green;
					*(temp_target++)=
						palette[color_index].blue;
					*(temp_target++) = 255; // alpha
				}
			} else {
				if(data_count < 0) {
					data_count = (0x100 - data_count);
					color_index = *(source_buffer++);
					while(data_count--) {
					*(temp_target++) =
						palette[color_index].red;
					*(temp_target++) =
						palette[color_index].green;
					*(temp_target++) =
						palette[color_index].blue;
					*(temp_target++) = 255; // alpha
					}
				}
			}
		}
		target_buffer += image_surface->pitch;
	}
}

void flc_decoder::decode_brun() {
	Uint8 *source_buffer, *target_buffer, *temp_target;
	Uint8 color_index;
	Sint8 data_count;
	Uint16 height_count, packet_count;

	height_count = header->height;
	source_buffer = (Uint8 *)(chunk_buffer + chunk_pointer);
	target_buffer = (Uint8 *)image_surface->pixels;

	while(height_count--) {
		temp_target = target_buffer;
		packet_count = *(source_buffer++);
		while(packet_count--) {
			data_count = *(source_buffer++);
			if(data_count > 0) {
				color_index = *(source_buffer++);
				while(data_count--) {
					*(temp_target++) =
						palette[color_index].red;
					*(temp_target++) =
						palette[color_index].green;
					*(temp_target++) =
						palette[color_index].blue;
					*(temp_target++) = 255; // alpha
    			    }
			} else {
				if(data_count < 0) {
					data_count = (0x100 - data_count);
					while(data_count--) {
						color_index = *(source_buffer++);
						*(temp_target++) =
							palette[color_index].red;
						*(temp_target++) =
							palette[color_index].green;
						*(temp_target++) =
							palette[color_index].blue;
						*(temp_target++) = 255; // alpha
					}
				}
			}
		}
		target_buffer += image_surface->pitch;
	}
}

void flc_decoder::decode_chunk() {
	Uint32 chunk_size;
	Uint16 chunk_n, chunk_type;

	SDL_LockSurface(image_surface);
	chunk_pointer = 10;
	READ16(&chunk_n, chunk_buffer);
	
	while(chunk_n--) {
		READ32(&chunk_size, chunk_buffer + chunk_pointer);
		chunk_pointer += 4;
		READ16(&chunk_type, chunk_buffer + chunk_pointer);
		chunk_pointer += 2;
		
		switch(chunk_type)
		{
			case FLI_COLOR256:
				decode_color256();
				break;
			case FLI_SS2:
				decode_ss2();
				break;
			case FLI_COLOR64:
				decode_color64();
				break;
			case FLI_LC:
				decode_lc();
				break;
			case FLI_BLACK:
				decode_black();
				break;
			case FLI_BRUN:
				decode_brun();
				break;
			case FLI_COPY:
				decode_copy();
				break;
			default:
				break;
		}
		chunk_pointer += (chunk_size - 6);
	}

	SDL_UnlockSurface(image_surface);
}

int flc_decoder::handle() {
	char buffer[256];
	Uint32 chunk_size;
	Uint16 chunk_type;

	if(frame_number > header->frames) return 0;
	if((last_ticks != 0) && (SDL_GetTicks() - last_ticks) <= flc_speed) return 0;

	flc_stream->read((char *)&chunk_size, 4);
	flc_stream->read((char *)&chunk_type, 2);
	
	switch(chunk_type) {
		case 0xF100:
			flc_stream->seekg(chunk_size - 6, ios::cur);
			break;
		case 0xF1FA:
			chunk_buffer = new char[chunk_size];
			flc_stream->read(chunk_buffer, chunk_size - 6);
			decode_chunk();
			delete []chunk_buffer;
			break;
		default:
			int chunk_pos = flc_stream->tellg();
			sprintf(buffer, "chunk type 0x%X at file position %d sized %d", chunk_type, chunk_pos, chunk_size);
			break;
	}
	
	last_ticks = SDL_GetTicks();

	frame_number++;
	return 1;
}

int flc_decoder::open(const char *filename) {
	flc_stream = new ifstream(filename, ios::in | ios::binary);
	if(!flc_stream->is_open()) {
		cerr << "Error opening file " << filename << endl;
		return 0;
	}
	
	header = new FLIC_HEADER;

	flc_stream->read((char *)header, sizeof(FLIC_HEADER));

	if((header->type != 0xAF12) && (header->type != 0xAF11)) {
		cerr << "Not a valid FLI/FLC file." << endl;
		delete []header;
		flc_stream->close();
		return 0;
	}
	
	if(header->type == 0xAF11) {
	    flc_speed = (Uint32 )((float )header->speed * (1.0f / 70.0f * 1000.0f));
	}
	else {
	    flc_speed = header->speed;
	}

        int w = nextpoweroftwo(header->width);
        int h = nextpoweroftwo(header->height);

	image_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
						0xff000000, 0x00ff0000,
						0x0000ff00, 0x000000ff);
						
	return 1;
}

void flc_decoder::display()
{
    GLuint texture;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
/*    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, intermediary->pitch / intermediary->format->BytesPerPixel);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, image_surface->pitch / image_surface->format->BytesPerPixel);    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, image_surface->w, image_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	    image_surface->pixels);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(image_surface->w, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(image_surface->w, image_surface->h);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, image_surface->h);
    glEnd();
    
    glFinish();

    glDeleteTextures(1, &texture);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    SDL_GL_SwapBuffers();
}
