output_file := 
delete := 
tests := $(wildcard tests/*.c)

ifeq ($(OS), Windows_NT)
	delete := del /f
	output_file := main.exe
else
	delete := rm -f
	output_file := main.out
endif

debug:
	gcc $(wildcard *.c) $(tests) -o $(output_file)

release:
	gcc $(wildcard *.c) $(tests) -o $(output_file) -DNDEBUG

clean:
	$(delete) main.exe