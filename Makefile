CC = gcc
FLAGS = `pkg-config --cflags gtk+-2.0`
TARGET = main
SRCS = main.c ./taxcalculator/*.c
LINKS = `pkg-config --cflags --libs gtk+-2.0` -lm
MINGW =  x86_64-w64-mingw32-gcc
WINTARGET = main.exe

all:
	$(CC) $(FLAGS) -o $(TARGET) $(SRCS) $(LINKS)
	./$(TARGET)

windows:
	$(MINGW) $(FLAGS) -o $(WINTARGET) $(SRCS) $(LINKS)
