bug = b
main = client.c

$(bug) a:$(main) net.c net.h user.c user.h
	gcc -g -o $(bug) $(main) net.c net.h user.c user.h
	gcc -g -o a server.c net.c net.h user.c user.h -lpthread
c:
	rm a b
