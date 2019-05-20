## Process this file with automake to produce Makefile.in

## Created by Anjuta

INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g

AM_CPPFLAGS = \
	 -Wall\
	 -g

bin_PROGRAMS = tuxfootball

tuxfootball_SOURCES = \
	gamestates/extratimefirsthalfstate.cpp \
	gamestates/extratimehalftimestate.cpp \
	gamestates/extratimesecondhalfstate.cpp \
	gamestates/extratimestate.cpp \
	gamestates/firsthalfstate.cpp \
	gamestates/fulltimestate.cpp \
	gamestates/halftimestate.cpp \
	gamestates/ingamestatebase.cpp \
	gamestates/mainmenustate.cpp \
	gamestates/matchabortedstate.cpp \
	gamestates/matchfinishedstate.cpp \
	gamestates/matchstartstate.cpp \
	gamestates/menustatebase.cpp \
	gamestates/nationalanthemstate.cpp \
	gamestates/penaltyshootoutstate.cpp \
	gamestates/redefinekeysstate.cpp \
	gamestates/secondhalfstate.cpp \
	gamestates/videomenustate.cpp \
	gamestates/statebase.cpp \
	gamestates/extratimefirsthalfstate.h \
	gamestates/extratimehalftimestate.h \
	gamestates/extratimesecondhalfstate.h \
	gamestates/extratimestate.h \
	gamestates/firsthalfstate.h \
	gamestates/fulltimestate.h \
	gamestates/halftimestate.h \
	gamestates/ingamestatebase.h \
	gamestates/mainmenustate.h \
	gamestates/matchabortedstate.h \
	gamestates/matchfinishedstate.h \
	gamestates/matchstartstate.h \
	gamestates/menustatebase.h \
	gamestates/nationalanthemstate.h \
	gamestates/penaltyshootoutstate.h \
	gamestates/redefinekeysstate.h \
	gamestates/secondhalfstate.h \
	gamestates/statebase.h \
	gamestates/videomenustate.h \
	logger/logger.h \
	logger/logger.cpp \
	menu/elements/menubutton.cpp \
	menu/elements/menuitem.cpp \
	menu/elements/menukeyselect.cpp \
	menu/elements/menulabel.cpp \
	menu/elements/menuoptionlist.cpp \
	menu/elements/menubutton.h \
	menu/elements/menuitem.h \
	menu/elements/menukeyselect.h \
	menu/elements/menulabel.h \
	menu/elements/menuoptionlist.h \
	menu/mainmenu.cpp \
	menu/menu.cpp \
	menu/redefinekeysmenu.cpp \
	menu/videosettingsmenu.cpp \
	menu/mainmenu.h \
	menu/menu.h \
	menu/redefinekeysmenu.h \
	menu/videosettingsmenu.h \
	resources/fontmanager.cpp \
	resources/fontmanager.h \
	resources/soundmanager.cpp \
	resources/soundmanager.h \
	resources/surfacemanager.cpp \
	resources/surfacemanager.h \
	resources/resourcemanager.cpp \
	resources/resourcemanager.h \
	ball.cpp \
	body.cpp \
	camera.cpp \
	controller.cpp \
	gameengine.cpp \
	graphics.cpp \
	main.cpp \
	matrix.cpp \
	pitch.cpp \
	player.cpp \
	point3d.cpp \
	rect.cpp \
	segment.cpp \
	spriteobject.cpp \
	spritesequence.cpp \
	team.cpp \
	tilemap.cpp \
	SFont.c \
	ball.h \
	body.h \
	camera.h \
	const.h \
	controller.h \
	Font.hpp \
	gameengine.h \
	graphics.h \
	matrix.h \
	pitch.h \
	player.h \
	point3d.h \
	rect.h \
	segment.h \
	SFont.h \
	spriteobject.h \
	spritesequence.h \
	team.h \
	tilemap.h \
	gettext.h

tuxfootball_LDFLAGS = \
	$(SDL_LIBS) \
	$(LTLIBINTL)

tuxfootball_CFLAGS =    \
	$(SDL_CFLAGS)

tuxfootball_CXXFLAGS =      \
	$(SDL_CFLAGS)

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
