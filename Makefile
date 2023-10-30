INC := inc/pstreams

all: server client sizeof

server: sock_obj.cpp server.cpp tcp_server.cpp log.h tcp_server.h
	g++ -o server  -I${INC} sock_obj.cpp server.cpp tcp_server.cpp

client: sock_obj.cpp client.cpp tcp_client.cpp log.h tcp_client.h 
	g++ -o client -I${INC} sock_obj.cpp client.cpp tcp_client.cpp -lreadline

sizeof: sizeof.cpp
	g++ -o sizeof sizeof.cpp

clear:
	rm ./server ./client
