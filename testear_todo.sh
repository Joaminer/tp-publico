#!/bin/bash

echo "========================================"
echo "🔨 COMPILANDO MÓDULOS..."
echo "========================================"

# 1. Compilamos Utils primero sí o sí, y luego el resto
for modulo in utils kernel_memory kernel_scheduler memory_stick swap cpu io; do
    echo "--> Limpiando y compilando $modulo..."
    # Hacemos make clean y make. El > /dev/null oculta los mensajes para que no te ensucie la pantalla
    make -C $modulo clean > /dev/null
    make -C $modulo > /dev/null
done

echo "✅ ¡Todo compilado con éxito!"
echo "========================================"
echo "🚀 LEVANTANDO EL SISTEMA..."
echo "========================================"

# 2. Levantar los servidores (Se abren en terminales nuevas de GNOME)
# El 'exec bash' final evita que la ventana se cierre de golpe si el programa tira un Segmentation Fault

gnome-terminal --title="Kernel Memory" -- bash -c "cd kernel_memory && ./bin/kernel_memory config/kernel_memory.config; exec bash"
gnome-terminal --title="Kernel Scheduler" -- bash -c "cd kernel_scheduler && ./bin/kernel_scheduler config/kernel_scheduler.config; exec bash"

echo "⏳ Esperando 2 segundos para que los Kernels escuchen..."
sleep 2

# 3. Levantar los módulos de almacenamiento
gnome-terminal --title="Memory Stick" -- bash -c "cd memory_stick && ./bin/memory_stick config/memory_stick.config 1024; exec bash"
gnome-terminal --title="Swap" -- bash -c "cd swap && ./bin/swap config/swap.config; exec bash"

echo "⏳ Esperando 2 segundos para que la Memoria atienda..."
sleep 2

# 4. Levantar los clientes de ejecución
gnome-terminal --title="CPU" -- bash -c "cd cpu && ./bin/cpu config/cpu.config 1; exec bash"
gnome-terminal --title="IO" -- bash -c "cd io && ./bin/io config/io.config; exec bash"

echo "🎉 ¡Sistema completo levantado y ejecutándose!"