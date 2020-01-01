# Compilers & Debuggers
CC = gcc
LEX = lex
DBG = lldb

# Input, Output, Testing files
LEX_IN = main.l
OUT = splitter
TEST = tests/t1
BUILD_TRASH = lex.yy.c

# Build arguments
MAC_LIBS = -ll
LIN_LIBS = -lfl
MEM_ANALYSIS_BUILD = -fsanitize=address -fno-omit-frame-pointer
STEPPER_BUILD = -g

# Targets
build-mac: | build-lex mac-compiler-lex clean

build-linux: | build-lex lin-compile-lex clean

mac-compiler-lex: $(BUILD_TRASH)
	$(CC) $(MAC_LIBS) lex.yy.c -o $(OUT)

lin-compiler-lex: $(BUILD_TRASH)
	$(CC) $(LIN_LIBS) lex.yy.c -o $(OUT)

build-lex: $(LEX_IN)
	$(LEX) $(LEX_IN)

clean: 
	rm $(BUILD_TRASH)

apt-config:
	sudo apt install flex

# Need to configure tests into a c file for CI
test-io: $(TEST) $(OUT)
	cat $(TEST) | ./$(OUT)

stepper: $(DEBUGGER) $(OUT)
	$(DBG) ./$(OUT)

clean-debug:
	rm $(OUT)
	rm -rf $(OUT).dSYM
