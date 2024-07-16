all:server client
server:server.cpp
	g++ $^ -o $@ -lpthread -ljsoncpp
client:client.cpp
	g++ $^ -o $@ -ljsoncpp
.PHONY:
clean:
	rm -rf server client