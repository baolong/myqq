a:server.c net.c net.h user.c user.h
	gcc -o a server.c net.c net.h user.c user.h -lpthread
	gcc -o b client.c net.c net.h user.c user.h
c:
	rm a
