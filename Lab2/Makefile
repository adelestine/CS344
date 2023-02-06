compile:
	@echo "Compileing to main.c"
	gcc --std=gnu99 -g -o output main.c
clean:
	@echo "Cleaning up..."
	rm -f output
	rm -r delestic.movies.*
valgrind:
	@echo "Running with valgrind"
	valgrind ./movies movies_sample_1.csv
zip:
	@echo "zipping :] good luck on your grade :]"
	zip delestic_program1 Makefile main.c header.h readme.txt