# Nombre del ejecutable final
TARGET = bin/kernel_memory

# Compilador y banderas
CC = gcc
# -I../utils/src le dice dónde buscar los headers compartidos
CFLAGS = -Wall -Wextra -g -I../utils/src

# Linkeo de bibliotecas
LDFLAGS = -L../utils -lutils -lcommons -lpthread

# Archivos fuente del Kernel Memory
SRCS = src/main.c src/servidor_memory.c
OBJS = $(SRCS:.c=.o)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkeando $(TARGET)..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "¡Kernel Memory compilado con éxito!"

%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p bin

clean:
	@echo "Limpiando Kernel Memory..."
	rm -rf bin *.o

.PHONY: all clean directories