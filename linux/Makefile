PROGRAM_NAME=kxm

SOURCE_FILES=\
    src/main.c \
    src/tweetnacl.c \
    src/encryption.c \

CC_FLAGS=\
	-O2 \
	-Wall \
	-Werror \

LINKER_FLAGS=\
	-lXtst \
	-lX11 \

all:
	mkdir -p build
	gcc -o build/$(PROGRAM_NAME) $(CC_FLAGS) $(LINKER_FLAGS) $(SOURCE_FILES)

test:
	mkdir -p build
	gcc -o build/xlistener -O2 -Wall -Werror src/keyboardhook.c src/mousehook.c

clean:
	rm -rf build/*

