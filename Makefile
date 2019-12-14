CC = clang
CARGS_DEV = -fsanitize=address -O1 -fno-omit-frame-pointer -g
CARGS = -fsanitize=address -O1 -fno-omit-frame-pointer
IN = main.c
OUT = splitter
TEST = ./tests/test.splitter

build: $(IN)
	$(CC) $(IN) -o $(OUT)

dev: $(IN)
	$(CC) $(CARGS_DEV) $(IN) -o $(OUT)

io: $(OUT)
	./$(OUT) $(TEST)

test: |build io

clean: $(OUT)
	rm $(OUT)
	rm -rf *.dSYM