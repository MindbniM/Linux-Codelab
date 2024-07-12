all:server client
server:server.cpp
	g++ $^ -o $@ -lpthread -std=c++17
client:client.cpp
	g++ $^ -o $@
.PHONY:
clean:
	rm -rf server client