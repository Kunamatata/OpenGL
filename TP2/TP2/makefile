robotopengl: robotopengl.o main.o generateeps.o
	g++ robotopengl.o main.o generateeps.o -o robotopengl  -g -D_REENTRANT -L /usr/local/lib/ -lSDL2 -lGL -lGLU -lGLEW -lglut
main.o : main.c robotopengl.h
	g++ -I /usr/local/include/SDL2/  -c main.c
robotopengl.o : robotopengl.c robotopengl.h
	g++ -I /usr/local/include/SDL2/  -c robotopengl.c
generateeps.o : generateeps.c robotopengl.h
	g++ -I /usr/local/include/SDL2/  -c generateeps.c
all : tp03
tp03: tp03.c 
#	g++ tp03.c -o tp03 -g -I /usr/local/include/SDL2/ -D_REENTRANT -L /usr/local/lib/ -lSDL2 -lGL -lGLU -lGLEW -lassimp
