CC := gcc
LDFLAGS := -lm -lraylib
CFLAGS := -Wall -g

SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=%.o)
EXECUTABLE = out

.PHONY: all clean web

all: $(EXECUTABLE)

web: CC:=emcc
web: LDFLAGS := -0 -lm ./dependencies/raylib/lib/libraylib.a -s USE_GLFW=3 --shell-file ./dependencies/raylib/src/minshell.html
web: CFLAGS := -DPLATFORM_WEB
web: $(OBJS)
	@mkdir -p html
	$(CC) $(CFLAGS) -o html/index.html $(addprefix $(BUILD_DIR)/,$(notdir $^)) $(LDFLAGS)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $(addprefix $(BUILD_DIR)/,$(notdir $^)) $(LDFLAGS)

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_DIR)/,$(notdir $@)) -I$(INCLUDE_DIR)

run:
	./out

debug:
	gdb ./out

leak-check:
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leak.txt ./build/app
	valgrind --leak-check=full --log-file=leak.txt ./out

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) html/

raylib:
	emcc -o build/rcore.o -c ./dependencies/raylib/src/rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	emcc -o build/rshapes.o -c ./dependencies/raylib/src/rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	emcc -o build/rtextures.o -c ./dependencies/raylib/src/rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	emcc -o build/rtext.o -c ./dependencies/raylib/src/rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	emcc -o build/rmodels.o -c ./dependencies/raylib/src/rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
	emcc -o build/utils.o -c ./dependencies/raylib/src/utils.c -Os -Wall -DPLATFORM_WEB
	emcc -o build/raudio.o -c ./dependencies/raylib/src/raudio.c -Os -Wall -DPLATFORM_WEB

	emar rcs dependencies/raylib/lib/libraylib.a build/rcore.o build/rshapes.o build/rtextures.o build/rtext.o build/rmodels.o build/utils.o build/raudio.o

emsdk:
	./dependencies/emsdk/emsdk activate latest
	source ./dependencies/emsdk/emsdk_env.sh
