# Compilers & Debuggers
CC = clang
LEX = lex
DBG = lldb

# Input, Output, Testing files
LEX_IN = main.l
OUT = splitter
TEST = tests/t1

# Build arguments
MAC_LIBS = -ll
NORMAL_BUILD = -O1
MEM_ANALYSIS_BUILD = -fsanitize=address -fno-omit-frame-pointer
STEPPER_BUILD = -g

# Targets
build-mac: $(LEX_IN)
	$(LEX) $(LEX_IN)
	$(CC) $(MAC_LIBS) $(MEM_ANALYSIS_BUILD) lex.yy.c -o $(OUT)
	rm lex.yy.c

# Need to configure tests into a c file for CI
test-io: $(TEST) $(OUT)
	cat $(TEST) | ./$(OUT)

stepper: $(DEBUGGER) $(OUT)
	$(DBG) ./$(OUT)

clean-debug:
	rm $(OUT)
	rm -rf $(OUT).dSYM
