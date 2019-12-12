CC = clang
CARGS = -fsanitize=address -O1 -fno-omit-frame-pointer -g
IN = main.c
OUT = splitter
TEST = ./tests/test.splitter

run: |build io

io: $(OUT)
	./$(OUT) $(TEST)

build: $(IN)
	$(CC) $(CARGS) $(IN) -o $(OUT)

exec: $(OUT)
	./$(OUT)

clean: $(OUT)
	rm $(OUT)
	rm -rf *.dSYM