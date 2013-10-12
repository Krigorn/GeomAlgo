CFLAGS = -c -g -Wall
GL_INCLUDE    = -I/usr/include/GL
GL_LIBDIR    = -L. -L/usr/X11R6/lib
GL_LIBRARIES = -lglut -lGL -lm  -lGLU -lXmu -lXext -lXi -lX11
CC=gcc
OS = "unknown"

# Add objectiveC library for MacOSX.
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep -i "Apple")),)
 GL_INCLUDE    = -I/usr/X11R6/include
 GL_LIBRARIES += -lobjc
 BROWSER = dillo
 OS = "osx"
endif
# Nothing to be done for Linux :o)
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep -i "Linux")),)
 OS = "linux"
 BROWSER = mozilla
endif
#
job: polygone dox
	polygone && $(BROWSER) DOX/html/index.html &
polygone: polygone.o
	$(CC) $(GL_LIBDIR) polygone.o $(GL_LIBRARIES) -o $@
polygone.o : polygone.c
	$(CC) $(GL_INCLUDE) $(CFLAGS) $<



clean:
	@echo "operating system = $(OS)"
	rm -rf *.o polygone DOX

################################################################################
# Generate doxygen documentation of file two.c using command file two.dox.
dox: polygone.c polygone.h
	rm -rf DOX
	doxygen polygone.dox
#

