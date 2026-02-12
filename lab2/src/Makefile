CC = clang
CFLAGS = -Wall -Werror -Wextra -std=c11 -g -fopenmp
LDFLAGS = -fopenmp

SOURCES := array1.c
BUILDDIR := ./build
OBJECTIVES := $(BUILDDIR)/$(SOURCES:.c=.o)
TARGET = program


all: clear $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(TARGET): $(OBJECTIVES)
	$(CC) $(LDFLAGS) $< -o $@

$(OBJECTIVES): $(SOURCES) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)