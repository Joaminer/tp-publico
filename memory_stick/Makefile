# Nombre del ejecutable final
TARGET = bin/memory_stick

# Compilador y banderas
CC = gcc
# -I../utils/src le dice al compilador dónde buscar estructura.h, sockets.h, etc.
CFLAGS = -Wall -Wextra -g -I../utils/src

# Linkeo de bibliotecas (orden importante)
# -L../utils le dice dónde buscar el archivo libutils.a
# -lutils linkea biblioteca, -lcommons la de la cátedra, -lpthread para los hilos
LDFLAGS = -L../utils -lutils -lcommons -lpthread

# Archivos fuente (por ahora solo  main.c)
SRCS = src/main.c
OBJS = $(SRCS:.c=.o)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkeando $(TARGET)..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "¡Memory Stick compilado con éxito!"

%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p bin

clean:
	@echo "Limpiando Memory Stick..."
	rm -rf bin *.o

.PHONY: all clean directories