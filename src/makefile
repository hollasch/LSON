# Visual Studio `nmake` Makefile for LSON
#
# A quick and dirty start to lson code.

buildDir = ..\build

all: lson

fresh: clobber all

clobber:
	-rmdir 2>nul /s /q $(buildDir)

$(buildDir):
	-mkdir $(buildDir)

lson: $(buildDir)\lson.exe

$(buildDir)\lson.exe: $(buildDir) lson.cpp
	cl -nologo -O2 -Ox -W4 -GF -GS \
		-Fe: $(buildDir)\lson.exe -Fo: $(buildDir)\lson.obj lson.cpp
