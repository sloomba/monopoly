all:	
	g++ -o monopoly main.cpp -lglut -lGLU -lGL -lIL -lglm -ldrawtext -lpng12 -ljpeg
	chmod +x run.sh
