include settings.mk

################################################################################

outname = bin/$(1)

define compile_out
	$(CC) $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(RUNDIRS:%=-Wl,-rpath,%) $(LIBS:%=-l%)
endef

define compile_objs
	$(CC) $(CFLAGS) -c -o "$@" $< $(IDIRS:%=-I%)
endef

################################################################################

NAME=$(shell pwd | xargs -I{} basename "{}")
CC=gcc
AR=ar
ARFLAGS=rcs
MAKE=make --no-print-directory

SRCS_C += $(shell find src -iname "*.c")
SRCS_H += $(shell find src -iname "*.h")

# --- CORRECCIÓN DE RUTAS DE LIBRERÍAS ---
# Quitamos el /lib/ de las librerías estáticas para que busque en ../utils/libutils.a
DEPS = $(foreach SHL,$(SHARED_LIBPATHS),$(SHL:%=%/lib/lib$(notdir $(SHL)).so)) \
	$(foreach STL,$(STATIC_LIBPATHS),$(STL:%=%/lib$(notdir $(STL)).a))

# --- CORRECCIÓN DE HEADERS ---
# Agregamos ../utils/src/utils para que encuentre sockets.h sin problemas
IDIRS += $(addsuffix /src,$(SHARED_LIBPATHS) $(STATIC_LIBPATHS) .) \
         $(addsuffix /src/utils,$(STATIC_LIBPATHS)) /usr/local/include

# --- CORRECCIÓN DE DIRECTORIOS DE LIBRERÍAS ---
LIBDIRS = $(SHARED_LIBPATHS:%=%/lib) $(STATIC_LIBPATHS) /usr/local/lib

RUNDIRS = $(SHARED_LIBPATHS:%=$(shell pwd)/%/lib)
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS_C))
OUT = $(call outname,$(NAME))

.PHONY: all
all: debug

.PHONY: debug
debug: CFLAGS = $(CDEBUG)
debug: $(OUT)

.PHONY: clean
clean:
	-rm -rfv bin/ obj/
	-for dir in $(STATIC_LIBPATHS); do $(MAKE) -C $$dir clean; done

$(OUT): $(OBJS) | $(dir $(OUT))
	$(call compile_out)

obj/%.o: src/%.c $(SRCS_H) $(DEPS) | $(dir $(OBJS))
	$(call compile_objs)

# Regla para compilar las dependencias (utils) si no existen
.SECONDEXPANSION:
$(DEPS):
	$(MAKE) -C $(dir $@)

$(sort $(dir $(OUT) $(OBJS))):
	mkdir -pv $@