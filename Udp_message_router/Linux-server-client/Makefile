all: server client
server:server.cpp
	g++ $^ -o $@
client:client.cpp
	g++ $^ -o $@ -lpthread
.PHONY:
clean:
	rm -rf server client