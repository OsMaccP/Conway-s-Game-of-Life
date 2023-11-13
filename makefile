all: compile link

compile:
	g++ -c -std=c++17 -Wall main.cpp -I".\dependencies\include" -DSFML_STATIC

link:
	g++ main.o -o GameOfLife -L".\dependencies\lib" \
	-lsfml-graphics-s \
	-lsfml-window-s \
	-lsfml-system-s \
	-lsfml-audio-s \
	-lopengl32 \
	-lfreetype \
	-lwinmm \
	-lgdi32 \
	-mwindows \
	-lsfml-main