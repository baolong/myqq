main = test_net.c

a:$(main) net.c net.h user.c user.h
	gcc -g -o a $(main) net.c net.h user.c user.h
c:
	rm a date
