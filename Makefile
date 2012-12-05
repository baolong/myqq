a:windows.c user.c user.h net.c net.h
	gcc -g -o a windows.c user.h user.c net.h net.c -lcursesw 
c:
	rm a
