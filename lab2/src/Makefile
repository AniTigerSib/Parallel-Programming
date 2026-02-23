CC = clang
CFLAGS = -Wall -Werror -Wextra -std=c11 -g -fopenmp
LDFLAGS = -fopenmp

LABSOURCES := lab_work.c
HWSOURCES := home_work.c
BUILDDIR := ./build

LABOBJECTIVES := $(BUILDDIR)/$(LABSOURCES:.c=.o)
HWOBJECTIVES := $(BUILDDIR)/$(HWSOURCES:.c=.o)

LABTARGET = lab_program
HWTARGET = hw_program

all: clear $(LABTARGET) $(HWTARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(LABTARGET): $(LABOBJECTIVES)
	$(CC) $(LDFLAGS) $< -o $@

$(LABOBJECTIVES): $(LABSOURCES) | $(BUILDDIR)
	$(CC) $(CFLAGS) -O2 -c $< -o $@

$(HWTARGET): $(HWOBJECTIVES)
	$(CC) $(LDFLAGS) $< -o $@

$(HWOBJECTIVES): $(HWSOURCES) | $(BUILDDIR)
	$(CC) $(CFLAGS) -O2 -c $< -o $@

clear:
	rm -rf $(BUILDDIR)
	rm -f $(LABTARGET)
	rm -f $(HWTARGET)