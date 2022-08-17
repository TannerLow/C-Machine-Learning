output_file := 
delete := 
tests := 
CFLAGS = -Wall
matrix_lib := $(wildcard matrix/*.c)
matrix_tests := $(wildcard matrix/tests/*.c)
model_lib := $(wildcard model/*.c)
model_tests := $(wildcard model/tests/*.c)
learning_lib := $(wildcard learning/*.c)
learning_tests := $(wildcard learning/tests/*.c)
mnist_lib := $(wildcard mnist/*.c)
formats_lib := $(wildcard formats/*/*.c)
test_adds := $(learning_tests) $(model_tests) $(matrix_tests) -DRUN_TESTS -DRUN_MODEL_TESTS

test_base_cmd = $(MAKE) -s CFLAGS="$(test_adds)"
base_cmd = gcc $(wildcard *.c) $(matrix_lib) $(model_lib) $(learning_lib) $(formats_lib) $(mnist_lib) $(CFLAGS) -o $(output_file)
debug_cmd  = $(base_cmd) -DDEBUG
release_cmd = $(base_cmd) -DNDEBUG

ifeq ($(OS), Windows_NT)
	delete := del /f
	output_file := main.exe
else
	delete := rm -f
	output_file := main.out
endif

debug:
	@echo $(debug_cmd)
	@$(debug_cmd)

release:
	@echo $(release_cmd)
	@$(release_cmd)

test_debug: 
	@$(test_base_cmd) debug

test_release: 
	@$(test_base_cmd) release

clean:
	$(delete) main.exe