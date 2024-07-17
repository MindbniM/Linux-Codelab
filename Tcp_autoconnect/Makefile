all:server client
server:server.cpp
	g++ $^ -o $@ -lpthread
client:client.cpp
	g++ $^ -o $@
.PHONY:clean
clean:
	rm -rf server client