SUBDIRS = engines .

bin_PROGRAMS = remotee

remotee_SOURCES = \
	cache.cc \
	command_line.cc \
	curl.cc \
	get_remote_ids.cc \
	engine_factory.cc \
	main.cc \
	remotes.cc \
	status_printer.cc \
	txt_printer.cc

AM_CXXFLAGS = ${REMOTEE_CXXFLAGS}

remotee_LDADD = \
	engines/libengines.a \
	${REMOTEE_LDFLAGS}

CLEANFILES = *~
