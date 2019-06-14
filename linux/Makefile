PROGRAM_NAME=kxm

SOURCE_FILES=\
	src/main.c \
	src/config.c \
	src/crypto/tweetnacl.c \
	src/crypto/encryption.c \

INC=\
	-Isrc \
	-Isrc/crypto \

CC_FLAGS=\
	-O2 \
	-Wall \
	-Werror \

LINKER_FLAGS=\
	-lXtst \
	-lX11 \

all:
	mkdir -p build
	cp -f res/config.txt build/config.txt
	gcc -o build/$(PROGRAM_NAME) $(CC_FLAGS) $(LINKER_FLAGS) $(INC) $(SOURCE_FILES)

test:
	mkdir -p build
	gcc -o build/xlistener -O2 -Wall -Werror src/input_hook/keyboardhook.c src/input_hook/mousehook.c

clean:
	rm -rf build/*

