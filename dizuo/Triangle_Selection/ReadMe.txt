A Sample Program 
by Jonathan Kreuzer

Stage #2: http://www.3dkingdoms.com/weekly/weekly.php?a=19
	Added triangle & marquee selection, updated viewport and mesh classes

Stage #1: http://www.3dkingdoms.com/weekly/weekly.php?a=16
	Created framework, demonstration of line-mesh intersection

***About***

This is just a little sample program.
Hopefully someone might find it useful.
I plan to develop more sophisticated samples using this as a base.
Possible this might eventually become a useful program.
I haven't had a chance to do much testing, but everything seems to work.

***What it does***

Loads a mesh made of vertices and triangles.
Displays the mesh in a viewport than can be dragged or rotated.
Allows selection of triangles by clicking, or dragging for marquee selection.

***Using***

Drag Right mouse button : rotate the view
Drag Center button      : pan the view
Drag Shift+Center button : zoom in or out

Left click - Selects the closest front facing triangle at that point of the screen
Drag + Left button - selects all triangles at least partially within the selection rectangle
Holding Ctrl - Adds to selection
Holding Alt  - Subtracts from selection

***Compiling***

I compiled with Microsoft Visual C++ 2005. I haven't had a chance to test other compilers yet. 
It uses OpenGL for rendering, so you'll need to add these libaries: opengl32.lib and glu32.lib

***Files needed in the project***

winmain.cpp      : Win32 stuff + Initialization + RenderFrame function
mesh.cpp         : Mesh Class ( load / select )
viewport.cpp     : Viewport Class + Selection Class
intersection.cpp : 3D Math helper functions for selection & collision detection
renderInterface.cpp : Used for all the rendering, currently uses OpenGL

linemeshtest.rc : Windows resource file