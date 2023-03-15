make: processing.o
	gcc -std=gnu99 -g line_processor.o -o line_processor

clean:
	rm -f line_processor
	rm -f *.o


processing.o: line_processor.c
	gcc -std=gnu99 -g -c line_processor.c -o line_processor.o -pthread

valgrind:
	@echo "Running with valgrind"
	valgrind ./line_processor < input1.txt