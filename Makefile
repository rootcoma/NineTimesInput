PROGRAM_NAME = kxm

SOURCE_FILES = \
	src/linux/gui.c \
	src/common/main.c \
	src/common/config/config.c \
	src/common/crypto/tweetnacl.c \
	src/common/crypto/encryption.c \
	src/common/inputhandler/inputhandler.c \

INC = \
	-Isrc/linux \
	-Isrc/common/crypto \
	-Isrc/common/config \
	-Isrc/common/inputhandler \

CC_FLAGS = \
	-O2 \
	-Wall \
	-Werror \
	-pedantic \

LINKER_FLAGS = \
	-lxcb-xtest \
	-lxcb \
	-lpthread \

all:
	mkdir -p build
	cp -f res/common/config.txt build/config.txt
	cp -f res/common/config-client.txt build/config-client.txt
	gcc -o build/$(PROGRAM_NAME) $(CC_FLAGS) $(LINKER_FLAGS) $(INC) $(SOURCE_FILES)

clean:
	rm -rf build/*
