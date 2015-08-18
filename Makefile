all: cli ser

clean:
	rm cli ser

cli: lib/cache.c lib/sound.c lib/utils.c client/client.c client/main.c
	g++ lib/cache.c lib/sound.c lib/utils.c client/client.c client/main.c -I ./ -lasound -pthread -o cli

ser: lib/cache.c lib/sound.c lib/utils.c server/server.c server/main.c
	g++ lib/cache.c lib/sound.c lib/utils.c server/server.c server/main.c -I ./ -lasound -pthread -o ser
