Readme.

Files included in the software product:
source.h 	, source.cpp
points.h 	,points.cpp
graph.h ,	 graph.cpp
globalvar.h ,	 globalvar.cpp
lines.h	,	 lines.cpp
openfile.h	, openfile.cpp
3dmaths.h	,3dmaths.cpp
faces.h		,faces.cpp
drawObject.h, 	drawobject.cpp
globalvar.h ,	 globalvar.cpp


To use the program
either run the project1.exe
or compile it using visual c++ by using the project1.sln
please dont forget to add the external library dependancy and add additional includes file
furthermore on the linker section under input please add the following into dependancy
glui32.lib;
freeglutd.lib;
opengl32.lib;
SOIL.lib;

Note that it requires glut32.dll in the file system to compile the program
that could be downloaded on
http://freeglut.sourceforge.net/
the glut32.dll is not included because it is unsafe to include systems file inside

