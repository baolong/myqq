c:server.c windows.c user.c user.h net.c net.h windows.h
	gcc -g -o a server.c windows.c windows.h user.h user.c net.h net.c -lcursesw -lpthread
cl:
	rm c
