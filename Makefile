# Copyright PCLP Team, 2025

# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99 -g

# Define targets, e.g., ninel, codeinvim, vectsecv, nomogram.
TARGETS=search_index

SOURCES=main.c utils.c operations.c

# Manually define all targets.
build: $(TARGETS)

search_index: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGETS) -lm

# Pack the solution into a zip file.
pack:
	zip -FSr 314CA_StefanAlexandruVladut_Tema2.zip README Makefile *.c *.h

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: build pack clean