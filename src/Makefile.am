
SUBDIRS = console seer gui

INCLUDES = $(PARAGUI_CFLAGS) -I./console -I./seer -I./gui -DDISABLE_BASS

bin_PROGRAMS = pw2

pw2dir = $(prefix)

pw2_LDADD = \
	-lconsole -lseer -L./console -L./seer \
	./gui/libgui.la \
	$(STATIC_LIBS) $(EX_LIBS)
#	-lSDL_net -lSDL_image -lSDL_mixer $(EX_LIBS)

pw2_SOURCES = \
	bldnum.cpp \
	mapi.cpp \
	pac_bshot.cpp \
	pac_shot.cpp \
	smapman.cpp \
	client.cpp \
	mapman.cpp \
	pac_entity.cpp \
	pac_sound.cpp \
	sprite.cpp \
	const.cpp \
	messenger.cpp \
	pac_extra.cpp \
	prediction.cpp \
	stdconfig.cpp \
	cvar.cpp \
	mouse.cpp \
	pac_fray.cpp \
	pw2.cpp \
	vapi.cpp \
	game.cpp \
	net.cpp \
	pac_grenade.cpp \
	replication.cpp \
	gmath.cpp \
	oapi.cpp \
	pac_obj.cpp \
	sampleman.cpp \
	gvars.cpp \
	osdep.cpp \
	pac_player.cpp \
	scripting.cpp \
	map.cpp \
	pac_bomb.cpp \
	pac_rail.cpp \
	server.cpp \
	chat.cpp

EXTRA_DIST = \
	_bldnum.h \
	gmath.h \
	mouse.h \
	pac_entity.h \
	pac_shot.h \
	scripting.h \
	bldnum.h \
	net.h \
	pac_extra.h \
	pac_sound.h \
	client.h \
	gvars.h \
	oapi.h \
	pac_fray.h \
	prediction.h \
	server.h \
	const.h \
	map.h \
	osdep.h \
	pac_grenade.h \
	protocol.h \
	smapman.h \
	cvar.h \
	mapi.h \
	pac.h \
	pac_obj.h \
	pw2.h \
	sprite.h \
	game.h \
	mapman.h \
	pac_bomb.h \
	pac_player.h \
	replication.h \
	stdconfig.h \
	gamedefs.h \
	messenger.h \
	pac_bshot.h \
	pac_rail.h \
	sampleman.h \
	vapi.h \
	chat.h \
	osdep_beos.cpp


