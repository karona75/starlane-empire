/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include<png.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<errno.h>
#include<limits.h>
#include<strings.h>
#include<unistd.h>

typedef struct s_entry
{
  int offset;
  char *name;
}
entry;

char *pal;
FILE *palfile;

int
write_png (char *filename, char *pixels, png_uint_32 width,
	   png_uint_32 height)
{
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep row_pointers[height];
  png_uint_32 k;

  fp = fopen (filename, "wb");
  if (fp == NULL)
    return (-1);

  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_ptr == NULL)
    {
      fclose (fp);
      return (-1);
    }

  info_ptr = png_create_info_struct (png_ptr);

  if (info_ptr == NULL)
    {
      fclose (fp);
      png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
      return (-1);
    }

  if (setjmp (png_jmpbuf (png_ptr)))
    {
      fclose (fp);
      png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
      return (-1);
    }

  png_init_io (png_ptr, fp);
  png_set_IHDR (png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);

  png_write_info (png_ptr, info_ptr);


  for (k = 0; k < height; k++)
    row_pointers[k] = pixels + k * width * 3;

  png_write_image (png_ptr, row_pointers);
  png_write_end (png_ptr, info_ptr);

  png_destroy_write_struct (&png_ptr, &info_ptr);
  fclose (fp);

  return (0);

}

int
extract (entry * files, FILE * src, int files_number)
{
  char *dname;
  char *pixbuf;
  unsigned int pixbufcount = 0;
  int count;
  int w = 0, h = 0;
  int pixels;
  int size = 0;
  int lines = 0;
  unsigned char index;
  unsigned char transparent[3];	// transparent pixel
  int file_count;

  transparent[0] = 0;
  transparent[1] = 254;
  transparent[2] = 0;



  dname = (char *) malloc (50 * sizeof (char));
  if (dname == NULL)
    {
      perror ("unable to allocate mem:");
      exit (-2);
    }

  // do all images
  for (file_count = 0; file_count < files_number; file_count++)
    {
      // first go to file offset
      if (fseek (src, (long) files[file_count].offset, SEEK_SET) < 0)
	{
	  perror ("Unable to seek in file:");
	  exit (-5);
	}
      // drop first six ints

      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);


      // before writing image make a first pass to DEDUCE width and
      // height
      // h and w will be final values
      w = 0;
      h = 0;
      count = 1;
      pixels = 0;		// current line's width
      while ((count > 0) && (ftell (src) < files[file_count + 1].offset))
	{
	  count = fread (&index, sizeof (char), 1, src);
	  if (count <= 0)
	    break;
	  if (index == 0)
	    {
	      h++;
	      w = (w > pixels) ? w : pixels;
	      pixels = 0;
	      continue;
	    }

	  if (index == 1)
	    {
	      count = fread (&index, sizeof (char), 1, src);
	      pixels += index;
	      continue;
	    }

	  if ((index % 2) == 0)
	    {
	      count = index / 2;
	      pixels += count;
	      count = fread (&index, sizeof (char), 1, src);
	    }
	  else
	    {
	      count = (index - 1) / 2;
	      pixels += count;
	      fseek (src, count, SEEK_CUR);
	    }

	}

      // printf("width: %d, height: %d\n", w, h);

      // allocate pixels buffer
      pixbuf = (char *) malloc (w * h * 3);
      pixbufcount = 0;

      count = 1;
      pixels = 0;
      fseek (src, files[file_count].offset, SEEK_SET);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);
      fread (&count, sizeof (int), 1, src);

      while ((count > 0) && (ftell (src) < files[file_count + 1].offset))
	{
	  unsigned int temp;
	  count = fread (&index, sizeof (char), 1, src);
	  if (count <= 0)
	    break;
	  if (index == 0)
	    {
	      // now we need to complete the line
	      for (temp = 0; temp < (w - pixels); temp++)
		{
		  pixbuf[pixbufcount] = transparent[0];
		  pixbuf[pixbufcount + 1] = transparent[1];
		  pixbuf[pixbufcount + 2] = transparent[2];
		  pixbufcount += 3;
		}
	      // fwrite((const void*)(&transparent), sizeof(char), 3,
	      // dest);
	      size += pixels;
	      lines++;
	      pixels = 0;
	      continue;
	    }

	  if (index == 1)
	    {
	      fread (&index, sizeof (char), 1, src);
	      for (temp = 0; temp < index; temp++)
		{
		  pixbuf[pixbufcount] = transparent[0];
		  pixbuf[pixbufcount + 1] = transparent[1];
		  pixbuf[pixbufcount + 2] = transparent[2];
		  pixbufcount += 3;
		}
	      // fwrite((const void*)(&transparent), sizeof(char), 3,
	      // dest);
	      pixels += index;
	      continue;
	    }

	  if ((index % 2) == 0)
	    {
	      count = index / 2;
	      fread (&index, sizeof (char), 1, src);
	      for (temp = 0; temp < count; temp++)
		{
		  pixbuf[pixbufcount] = pal[index * 3];
		  pixbuf[pixbufcount + 1] = pal[index * 3 + 1];
		  pixbuf[pixbufcount + 2] = pal[index * 3 + 2];
		  pixbufcount += 3;
		}
	      // fwrite((const void*)(pal+index*3), sizeof(char), 3,
	      // dest);
	      pixels += count;
	    }
	  else
	    {
	      count = (index - 1) / 2;
	      for (temp = 0; temp < count; temp++)
		{
		  fread (&index, sizeof (char), 1, src);
		  pixbuf[pixbufcount] = pal[index * 3];
		  pixbuf[pixbufcount + 1] = pal[index * 3 + 1];
		  pixbuf[pixbufcount + 2] = pal[index * 3 + 2];
		  pixbufcount += 3;
		  // fwrite((const void*)(pal+index*3), sizeof(char), 3, 
		  // dest);
		}
	      pixels += count;
	    }

	}

      // now write png file
      write_png (files[file_count].name, pixbuf, w, h);
    }

  fclose (src);

  return 0;
}

int
main (int argc, char **argv)
{

  FILE *shp;
  int files_number = 0;
  int file_count;
  int l, serial;
  entry *files;
  char *filename;
  int count;

  if (argc != 2)
    {
      printf ("usage: imgbuild file\n");
      exit (-1);
    }

  l = strlen (argv[1]);
  if (((argv[1][l - 1] == 'p') && (argv[1][l - 2] == 'h')
       && (argv[1][l - 3] == 's')) || ((argv[1][l - 1] == 'p')
				       && (argv[1][l - 2] == 'm')
				       && (argv[1][l - 3] == 't')))
    {
      printf ("Parsing a .shp or .tmp file\n");
      printf ("Starting to parse: %s\n", argv[1]);

      // open src file read only
      shp = fopen (argv[1], "r");

      if (shp == NULL)
	{
	  perror ("unable to open file:");
	  exit (-10);
	}
      // read serial number
      fread (&serial, sizeof (int), 1, shp);

      if (serial != 808529457)
	{
	  printf ("This is not a real .shp! Wrong serial\n");
	  fclose (shp);
	  exit (-11);
	}
      // read number of files
      fread (&files_number, sizeof (int), 1, shp);

      // allocate entry
      files = (entry *) malloc ((files_number + 1) * sizeof (entry));

      if (files == NULL)
	{
	  perror ("Unable to allocate memory:");
	  fclose (shp);
	  exit (-12);
	}

      //trunc file name (delete .shp"
      filename = strdup (argv[1]);
      filename[l - 4] = 0;
      // filling entry
      for (file_count = 0; file_count < files_number; file_count++)
	{
	  // first add name
	  char *temp = (char *) malloc (50 * sizeof (char));
	  long long int offset;
	  if (snprintf (temp, 50, "%d_%s.png", file_count, filename) < 0)
	    printf ("Warning: name truncated\n");

	  files[file_count].name = temp;
	  // then add offset
	  fread (&offset, sizeof (long long int), 1, shp);
	  files[file_count].offset = (int) offset;
	}

//now put a limit for the last file
      files[files_number].offset = INT_MAX;

      //create destination directory
      if (mkdir (filename, S_IRWXU) != 0)
	{
	  perror ("warning: unable to create destination directory:");
	}

      //allocate palette
      pal = (char *) malloc (3 * 256 * sizeof (char));
      if (pal == NULL)
	{
	  perror ("unable to allocate mem:");
	  exit (-2);
	}
      // load palette
      palfile = fopen ("game.pal", "r");
      if (palfile == NULL)
	{
	  perror ("unable to load palette:");
	  exit (-3);
	}

      for (count = 0; count < 256; count++)
	{
	  if (fread (&(pal[count * 3]), sizeof (char), 3, palfile) < 3)
	    {
	      printf ("unable to complete palette loading\n");
	      break;
	    }
	  pal[count * 3] *= 4;
	  pal[count * 3 + 1] *= 4;
	  pal[count * 3 + 2] *= 4;
	}

      fclose (palfile);
      //now change to new directory

      chdir (filename);

      // extract all files
      extract (files, shp, files_number);

      //come back to cwd
      chdir ("..");

      fclose (shp);

    }
  else
    {
      printf ("This is not a file I know (by its extension)\n");
      exit (-9);
    }
  exit (0);
}
