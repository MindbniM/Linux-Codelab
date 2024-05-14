all:shm_c shm_t
shm_c:shm_c.cpp
	g++ $^ -o $@
shm_t:shm_t.cpp
	g++ $^ -o $@
.PHONY:
clean:
	rm -rf shm_c shm_t fifo