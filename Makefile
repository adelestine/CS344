compile:
	@echo "Compileing to main.c"
	gcc --std=gnu99 -o movies main.c
clean:
	@echo "Cleaning up..."
valgrind:
	@echo "Running with valgrind"
	valgrind ./movies movies_sample_1.csv
zip:
	@echo "zipping :] good luck on your grade :]"
	zip delestic_program1 Makefile main.c header.h readme.txt