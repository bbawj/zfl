CFLAGS=-Wextra -Wall

zfl: zfl.c
	$(CC) $(CFLAGS) zfl.c -o zfl

server:
	$(CC) $(CFLAGS) "./zflserver/src/main.c" -lraylib -o "./zflserver/server" -lm -g

client:
	west build --build-dir="./zflclient/out" -b qemu_x86 -o="--jobs=8" "./zflclient" -- -G"Unix Makefiles" -DEXTRA_CONF_FILE=overlay-e1000.conf

test: test.c
	$(CC) $(CFLAGS) "test.c" -o "test" -lm -g
