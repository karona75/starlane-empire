/*
   Copyright (C) 2004 by Wagner Frederic <wagnerf@loria.fr>
   Part of the Starlane Empire Project http://mozenix.org/projects.html

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

typedef struct s_entry
{
  int offset;
  char *name;
}
entry;

int
extract (entry * files, FILE * src, int files_number)
{
  int file_count;
  FILE *dest;
  int position;
  char c;

  //extract all files
  for (file_count = 0; file_count < files_number - 1; file_count++)
    {
      printf ("Extracting file: %s, offset: %d\n", files[file_count].name,
	      files[file_count].offset);
      //first go to file offset
      if (fseek (src, (long) files[file_count].offset, SEEK_SET) < 0)
	{
	  perror ("Unable to seek in file:");
	  exit (-5);
	}
      //now open new file
      dest = fopen (files[file_count].name, "w");
      if (dest == NULL)
	{
	  perror ("Unable to open dest file:");
	  exit (-6);
	}

      //now fill the file
      for (position = files[file_count].offset;
	   position < files[file_count + 1].offset; position++)
	{
	  fread (&c, sizeof (char), 1, src);
	  fwrite (&c, sizeof (char), 1, dest);
	}

      //close it
      fclose (dest);
    }

  //handle last file now
  printf ("Extracting file: %s, offset: %d\n", files[file_count].name,
	  files[file_count].offset);
  //first go to file offset
  if (fseek (src, (long) files[file_count].offset, SEEK_SET) < 0)
    {
      perror ("Unable to seek in file:");
      exit (-5);
    }
  //now open new file
  dest = fopen (files[file_count].name, "w");
  if (dest == NULL)
    {
      perror ("Unable to open dest file:");
      exit (-6);
    }

  //now fill the file
  while (fread (&c, sizeof (char), 1, src) > 0)
    fwrite (&c, sizeof (char), 1, dest);

  //close it
  fclose (dest);

  return 0;
}


int
main (int argc, char **argv)
{

  FILE *cob;
  FILE *shp;
  int files_number = 0;
  int file_count;
  char buffer[50];
  int l;
  entry *files;


  if (argc != 2)
    {
      printf ("usage: parser file.cob\n");
      exit (-1);
    }

  //check if we're parsing .cob or .shp

  l = strlen (argv[1]);
  if ((argv[1][l - 1] == 'b')
      && (argv[1][l - 2] == 'o') && (argv[1][l - 3] == 'c'))
    {
      printf ("Parsing a .cob file\n");

      printf ("Starting to parse %s\n", argv[1]);

      cob = fopen (argv[1], "r");
      if (cob == NULL)
	{
	  perror ("unable to open file : ");
	  exit (-2);
	}

      //read number of files stored in .cob
      fread (&files_number, sizeof (int), 1, cob);

      printf ("Extracting %d files...\n", files_number);

      files = (entry *) malloc (files_number * sizeof (entry));

      if (files == NULL)
	{
	  perror ("Unable to allocate memory:");
	  exit (-4);
	}

      //read files names
      for (file_count = 0; file_count < files_number; file_count++)
	{
	  int i = 0;
	  //read file name
	  fread (&buffer, sizeof (char), 50, cob);
	  //now change dos filenames into unix ones
	  while (buffer[i] != '\0')
	    {
	      if (buffer[i] == '\\')
		buffer[i] = '/';
	      i++;
	    }
	  //and store the finale name in our entry struct
	  sscanf ((const char *) &buffer, "%as", &(files[file_count].name));
	}

      //now read files offsets
      for (file_count = 0; file_count < files_number; file_count++)
	fread (&(files[file_count].offset), sizeof (int), 1, cob);

      //create 'data' subdirectory (yes it's ugly)
      if ((mkdir ("data", S_IRUSR | S_IWUSR | S_IXUSR) < 0)
	  && (errno != EEXIST))
	{
	  perror ("unable to create data subdirectory:");
	  exit (-7);
	}

      extract (files, cob, files_number);

      //close main file
      fclose (cob);
    }
  else if (((argv[1][l - 1] == 'p') && (argv[1][l - 2] == 'h')
	   && (argv[1][l - 3] == 's'))||((argv[1][l - 1] == 'p')&&(argv[1][l - 2] == 'm')
	   && (argv[1][l - 3] == 't')))
    {
      printf ("Parsing a .shp or .tmp file\n");
      printf ("Starting to parse: %s\n", argv[1]);

      //open src file read only
      shp = fopen (argv[1], "r");

      if (shp == NULL)
	{
	  perror ("unable to open file:");
	  exit (-10);
	}

      //read serial number
      fread (&l, sizeof (int), 1, shp);

      if (l != 808529457)
	{
	  printf ("This is not a real .shp! Wrong serial\n");
	  fclose (shp);
	  exit (-11);
	}

      //read number of files
      fread (&files_number, sizeof (int), 1, shp);

      //allocate entry
      files = (entry *) malloc (files_number * sizeof (entry));

      if (files == NULL)
	{
	  perror ("Unable to allocate memory:");
	  fclose (shp);
	  exit (-12);
	}

      //filling entry
      for (file_count = 0; file_count < files_number; file_count++)
	{
	  //first add name
	  char *temp = (char *) malloc (50 * sizeof (char));
	  long long int offset;
	  if (snprintf (temp, 50, "%d_%s", file_count, argv[1]) < 0)
	    printf ("Warning: name truncated\n");

	  files[file_count].name = temp;
	  //then add offset
	  fread (&offset, sizeof (long long int), 1, shp);
	  files[file_count].offset = (int) offset;
	}

      //extract all files
      extract (files, shp, files_number);

      fclose (shp);

    }
  else
    {
      printf ("This is not a file I know (by its extension)\n");
      exit (-9);
    }
  exit (0);
}
