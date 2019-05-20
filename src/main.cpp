/***************************************************************************
 *   Copyright (C) 2003-2011 by Tux Football development team              *
 *   Authors: Jason Wood <jasonwood@blueyonder.co.uk>                      *
 *            Christoph Brill <egore911@egore911.de>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_GETTEXT
#include <locale.h>
#include "gettext.h"
#endif

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#ifdef _WIN32
#include "windows.h"
#endif

#include "gameengine.h"
#include "logger/logger.h"

void die(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(1);
}

/** Execution starts here. */
int main(int argc, char *argv[])
{
#ifdef PACKAGE_VERSION
	INFO("Starting Tux Football " << PACKAGE_VERSION);
#else
	INFO("Starting Tux Football");
#endif

	bool fullscreen = false;
	if(argc > 1) // first arg is path - we don't need it for now
	for(int i = argc-1; i > 0; i--) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				default:
				case 'h':
					printf("\nTux Football [-switch]\n\nswitch can be:\n\n-h\tprint this help\n-w\trun Tux Football in windowed mode (default)\n-w\trun Tux Football in fullscreen mode\n-d\tEnable debug logging\n\n");
					return 0;
				case 'w':
					fullscreen = false;
					break;
				case 'f':
					fullscreen = true;
					break;
				case 'd':
					Logger::setLevel(Logger::Debug);
					break;
			}
		}
	}

#ifdef HAVE_GETTEXT
	INFO("Reading locales from " << LOCALEDIR);
	// Intialize gettext
	setlocale (LC_ALL, "");
	bindtextdomain(PACKAGE_NAME, LOCALEDIR);
	textdomain(PACKAGE_NAME);
#else
	DEBUG("Gettext support not available due to missing config.h support");
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
		die("Couldn't initialize SDL: %s\n", SDL_GetError());
	atexit(SDL_Quit);

	GameEngine engine(fullscreen);

	engine.gameLoop();

	return 0;
}

#ifdef _WIN32

#ifndef _MAX_PATH
#define _MAX_PATH 2048
#endif

/* Taken from http://www.flipcode.com/archives/WinMain_Command_Line_Parser.shtml */
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, char* command_line, int show_command)
{

    int    argc;
    char** argv;

    char*  arg;
    int    index;
    int    result;

    // count the arguments

    argc = 1;
    arg  = command_line;

    while (arg[0] != 0) {

        while (arg[0] != 0 && arg[0] == ' ') {
            arg++;
        }

        if (arg[0] != 0) {

            argc++;

            while (arg[0] != 0 && arg[0] != ' ') {
                arg++;
            }

        }

    }

    // tokenize the arguments

    argv = (char**)malloc(argc * sizeof(char*));

    arg = command_line;
    index = 1;

    while (arg[0] != 0) {

        while (arg[0] != 0 && arg[0] == ' ') {
            arg++;
        }

        if (arg[0] != 0) {

            argv[index] = arg;
            index++;

            while (arg[0] != 0 && arg[0] != ' ') {
                arg++;
            }

            if (arg[0] != 0) {
                arg[0] = 0;
                arg++;
            }

        }

    }

    // put the program name into argv[0]

    char filename[_MAX_PATH];

    GetModuleFileName(NULL, filename, _MAX_PATH);
    argv[0] = filename;

    // call the user specified main function

    result = main(argc, argv);

    free(argv);
    return result;

}
#endif
