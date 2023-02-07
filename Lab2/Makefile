compile:
	@echo "Compileing to main.c"
	gcc --std=gnu99 -g -o movies_by_year main.c
clean:
	@echo "Cleaning up..."
	rm -f movies_by_year
	rm -r delestic.movies.*
valgrind:
	@echo "Running with valgrind"
	valgrind ./movies_by_year
all:
	@echo "Compiling and running with valgrind"
	gcc --std=gnu99 -g -o movies_by_year main.c
	valgrind ./output
zip:
	@echo "zipping :] good luck on your grade :]"
	zip delestic_program2 Makefile main.c header.h readme.txt