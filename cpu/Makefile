TARGET = bin/cpu
CC = gcc
CFLAGS = -Wall -Wextra -g -I../utils/src
LDFLAGS = -L../utils -lutils -lcommons -lpthread

SRCS = src/main.c
OBJS = $(SRCS:.c=.o)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkeando $(TARGET)..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "¡CPU compilada con éxito!"

%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p bin

clean:
	@echo "Limpiando CPU..."
	rm -rf bin *.o

.PHONY: all clean directories