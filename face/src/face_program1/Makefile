TARGET=face_program1
OBJS=image.o main.o
CC=gcc
CFLAGS=-Wall -O0
GFlAG=-g
IMAGEDIR=/Users/home2/s132042/zikken3/face/image/bmp_image/

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $<
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

debug: $(OBJS)
	$(CC) $(CFLAGS) $(GFLAG) -o $(TARGET)

test_display:
	./$(TARGET) $(IMAGEDIR)face1.bmp test.bmp
