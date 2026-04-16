# Nombre del ejecutable final
TARGET = bin/kernel_scheduler

# Compilador y banderas
CC = gcc
# -I../utils/src le dice dónde buscar los headers compartidos (por si Eliseo los empieza a usar)
CFLAGS = -Wall -Wextra -g -I../utils/src

# Linkeo de bibliotecas

LDFLAGS = -L../utils -lutils -lcommons -lpthread

# Archivo fuente 
SRCS = src/main.c
OBJS = $(SRCS:.c=.o)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkeando $(TARGET)..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "¡Kernel Scheduler compilado con éxito!"

%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p bin

clean:
	@echo "Limpiando Kernel Scheduler..."
	rm -rf bin *.o

.PHONY: all clean directories