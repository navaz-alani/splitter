CC = clang
CARGS = -fsanitize=address -O1 -fno-omit-frame-pointer -g
IN = main.c
OUT = splitter
TEST = ./tests/test.splitter

build: $(IN)
	$(CC) $(IN) -o $(OUT)

dev: $(IN)
	$(CC) $(CARGS) $(IN) -o $(OUT)

io: $(OUT)
	./$(OUT) $(TEST)

test: |build io

clean: $(OUT)
	rm $(OUT)
	rm -rf *.dSYM