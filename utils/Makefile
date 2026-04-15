#Nombre del empaquetado final (Biblioteca estática)
TARGET = libutils.a

#Compilador y banderas (flags)
CC = gcc
CFLAGS = -Wall -Wextra -g -I./src

#Directorios de trabajo
SRC_DIR = src/utils
OBJ_DIR = obj

#Buscar automáticamente todos los .c en src y definir sus .o correspondientes
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

#Regla por defecto que se ejecuta al escribir "make"
all: $(TARGET)

#Regla para empaquetar la biblioteca estática usando "ar"
$(TARGET): $(OBJS)
	@echo "Empaquetando la biblioteca $(TARGET)..."
	ar rcs $@ $^
	@echo "¡utils compilado con éxito!"

#Regla para compilar cada .c en un .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | directories
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

#Regla para crear la carpeta "obj" si no existe
directories:
	@mkdir -p $(OBJ_DIR)

#Regla para limpiar el proyecto al escribir "make clean"
clean:
	@echo "Limpiando archivos de utils..."
	rm -rf $(OBJ_DIR) $(TARGET)

#Evita conflictos si por casualidad creas un archivo llamado "all" o "clean"
.PHONY: all clean directories