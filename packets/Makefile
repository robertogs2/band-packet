SRC_DIR = src/
LIB_DIR = lib/

all:
	rm -R bin
	mkdir bin && make -C $(LIB_DIR) && make -C $(SRC_DIR)
	./bin/main
