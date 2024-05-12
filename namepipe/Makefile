all:test1 test2
test1:test1.cpp
	g++ -o $@ $^ 
test2:test2.cpp
	g++ -o $@ $^ 
.PHONY:clean
clean:
	rm -rf test1 test2 fifo