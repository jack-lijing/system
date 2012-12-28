all:
	gcc -g -o echoserverp echoserverp.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o echoservers echoservers.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o echoservert echoservert.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o echoservert_pre echo_cnt.c echoservert_pre.c sbuf.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o norace norace.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o select select.c ../src/csapp.c -I ../include/ -lpthread
	gcc -g -o sharing sharing.c ../src/csapp.c -I ../include/ -lpthread