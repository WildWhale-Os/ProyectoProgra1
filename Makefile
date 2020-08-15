build:
	gcc -Wfatal-errors \
	-std=c99 \
	./*.c \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-o tetris.exe