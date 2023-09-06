CFLAGS=-Wextra -Wall

zfl: zfl.c
	$(CC) $(CFLAGS) zfl.c -o zfl

build_server:
	west build --build-dir="./zflserver/out" -b qemu_x86 -o="--jobs=8" "./zflserver" -- -G"Unix Makefiles" -DEXTRA_CONF_FILE=overlay-e1000.conf

build_client:
	west build --build-dir="./http_client/out" -b qemu_x86 -o="--jobs=8" "./http_client" -- -G"Unix Makefiles" -DEXTRA_CONF_FILE=overlay-e1000.conf

