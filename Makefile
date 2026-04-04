CC     = gcc
CFLAGS = -Wall -O2 -I. $(shell pkg-config --cflags raylib)
LIBS   = $(shell pkg-config --libs raylib) -lm -ldl -lpthread

TARGET = animasi_selebrasi

SRCS = main.c \
       coords.c \
       src/algo/dda.c \
       src/algo/bresenham.c \
       src/algo/midcircle.c \
       src/ui/primitives.c \
       src/ui/back_button.c \
       src/ui/replay_button.c \
       src/ui/cartesian.c \
       src/screens/kneeslide.c \
       src/screens/lompat.c \
       src/screens/takel.c \
       src/screens/menu_animasi.c \
       src/screens/pola2d.c \
       src/screens/about.c \
       src/screens/menu.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	LIBGL_ALWAYS_SOFTWARE=1 ./$(TARGET)
