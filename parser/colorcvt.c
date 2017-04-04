/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.

	this program takes a 24bpp png image with only green channel
	and generate images for all other colors
	usefull to generate races flags of different colors for example
   
*/

#include<png.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>


png_structp png_ptr;
png_infop info_ptr;
	
void save_image(char* name, char* pixels) {
	FILE* fp;
	unsigned int row;
	unsigned int width = png_get_image_width(png_ptr, info_ptr);
	unsigned int height = png_get_image_height(png_ptr, info_ptr);
	png_structp png_ptrw;
	png_infop info_ptrw;
	
	fp = fopen(name, "wb");
	if (!fp) {
		perror("unable to open file");
		exit(10);
	}

	png_ptrw = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptrw = png_create_info_struct (png_ptrw);

    if (setjmp (png_jmpbuf (png_ptrw)))
    {
      fclose (fp);
      png_destroy_write_struct (&png_ptrw, (png_infopp) NULL);
      return;
    }

	//init file writing
	png_init_io(png_ptrw, fp);

    png_set_IHDR (png_ptrw, info_ptrw, width, height, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptrw, info_ptrw);
	
	//allocate row_pointers
	info_ptrw->row_pointers = (png_bytepp) malloc(height * sizeof(png_bytep));

	//setting row pointers
	for(row = 0 ; row < height; row++)
		info_ptrw->row_pointers[row] = &pixels[row*width*3];

	//write image
	png_write_image(png_ptrw, info_ptrw->row_pointers);
	png_write_end(png_ptrw, info_ptrw);
	png_destroy_write_struct (&png_ptrw, &info_ptrw);
			
	fclose(fp);
}


int main(int argc, char **argv) {
	FILE *fp;
	char *header;
	char is_png;
	unsigned int bit_depth;
	unsigned int color_type;
	char* pixels;
	unsigned int width, height, i, j, pass;
	unsigned char tmp;
	char *filename;
	char letter = 'a' - 1;

	//check some basics
	if (argc !=2) {
		fprintf(stderr,"use: %s image.png\n", argv[0]);
		exit(1);
	}

    fp = fopen(argv[1], "rb");
    if (!fp)
    {
		perror("unable to open file");
        return (1);
    }

	header = (char*) malloc(8);
	
    fread(header, 1, 8, fp);
	free((void*)header);

    //is_png = png_sig_cmp(header, 0, 8);
    is_png = 0;
    if (is_png)
    {
		fprintf(stderr,"this file (%s) is not a valid png file\n", argv[1]);
		fclose(fp);
        return (2);
    }

	//libpng stuff
    png_ptr = png_create_read_struct
       (PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    if (!png_ptr) {
		fprintf(stderr, "unable to allocate libpng structs\n");
		fclose(fp);
        return (3);
	}

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,
           (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
        return (4);
    }

	//start reading source file
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	//done with file, closing
	fclose(fp);
	
	//verify we're in paletized mode
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	bit_depth *= png_get_channels(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	if (bit_depth != 24) {
		fprintf(stderr,"this image isn't in 24bpp, exiting\n");
		return(6);
	}

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);

	//allocate pixels
	pixels = (char*) malloc (width * height * 3);
	//allocate space for file name
	filename = (char*) malloc (strlen(argv[1])+1);
	//initialize filename
	for(i = 0 ; i <= strlen(argv[1]) ; i++)
		filename[i+1] = argv[1][i];

	//now generate all 7 images
	for (pass = 0 ; pass < 7 ; pass++) {
	
		letter++;
		filename[0] = letter;
		//copy pixels 
		for(i = 0 ; i < width * 3 ; i++)
			for(j = 0 ; j < height ; j++)
				pixels[j*width*3+i] = info_ptr->row_pointers[j][i];

		//check what color we generate
		switch(pass) {

		case 0:
			//green
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = 0;
					pixels[j*width*3+i*3+1] = tmp;
					pixels[j*width*3+i*3+2] = 0;
				}
			break;
		case 1:
			//purple
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = tmp;
					pixels[j*width*3+i*3+1] = tmp/2;
					pixels[j*width*3+i*3+2] = tmp;
				}
			break;
		case 2:
			//yellow
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = tmp;
					pixels[j*width*3+i*3+1] = tmp;
					pixels[j*width*3+i*3+2] = tmp/2;
				}
			break;
		case 3:
			//pink
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = tmp;
					pixels[j*width*3+i*3+1] = tmp/2;
					pixels[j*width*3+i*3+2] = tmp/2;
				}
			break;
		case 4:
			//brown
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = tmp/2;
					pixels[j*width*3+i*3+1] = tmp/3;
					pixels[j*width*3+i*3+2] = tmp/3;
				}
			break;
		case 5:
			//brown
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = 0;
					pixels[j*width*3+i*3+1] = tmp/2;
					pixels[j*width*3+i*3+2] = tmp;
				}
			break;
		case 6:
			//orange
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = tmp;
					pixels[j*width*3+i*3+1] = tmp/2;
					pixels[j*width*3+i*3+2] = 0;
				}
			break;

		default:
			//handle pixels
			//tmp contains green component (only one in orig image)
			//we do all possible permutations
			for(i = 0 ; i < width ; i++)
				for(j = 0 ; j < height ; j++) {
					tmp = pixels[j*width*3+i*3+1];
					pixels[j*width*3+i*3] = ((pass%2)==0)?tmp:0;
					pixels[j*width*3+i*3+1] = ((pass&2)!=0)?tmp:0;
					pixels[j*width*3+i*3+2] = ((pass&4)!=0)?tmp:0;
				}
		}
	
		//save image
		save_image(filename, pixels);

	}

	//TODO: check the problem
	//png_destroy_write_struct(&png_ptr, &info_ptr);
	free(pixels);
	free(filename);
	return 0;
}

