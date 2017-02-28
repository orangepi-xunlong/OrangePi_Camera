# ---------------------------------
# OrangePi V4L2 project
# Maintainer: Buddy <buddy.zhang@aliyun.com>
#
# Camera list:
# gc2035

CC=gcc
FLAGS= -I./include -ljpeg
TARGET= OrangePi_Capture

VPATH := src
V=

SharedLibrary= libOrangePi_SharedLib.so
OBJS= main.o

all: $(SharedLibrary) install $(TARGET)

$(TARGET): $(OBJS)
	$(V)$(CC) $< -ljpeg -lOrangePi_SharedLib -o $@
	$(V)rm *.o

$(SharedLibrary): OrangePi_SharedLib.c OrangePi_V4L2.c OrangePi_ImageLibrary.c
	$(V)$(CC) $^ $(FLAGS)  -shared -fPIC -o $@
	$(V)mv $@ lib/


# C source code
main.o: main.c
	$(V)$(CC) $< $(FLAGS) -c -o $@

.PHONY: intall
install:
	$(V)cp -rfa lib/libOrangePi_SharedLib.so /usr/lib/libOrangePi_SharedLib.so
	$(V)cp -rfa include/OrangePiV4L2 /usr/include

.PHONY: clean
clean:
	$(V)rm -rf *.o $(TARGET) lib/*.so

