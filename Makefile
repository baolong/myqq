a:test_user.c net.c net.h user.c user.h
	gcc -g -o a test_user.c net.c net.h user.c user.h
clean:
	rm a date
