c:client.c windows.c user.c user.h net.c net.h windows.h myqq.h
	gcc -g -o a server.c windows.c windows.h user.h user.c net.h net.c myqq.h -lcursesw -lpthread
	gcc -g -o b client.c windows.c windows.h user.h user.c net.h net.c -lcursesw -lpthread
cl:
	rm c
