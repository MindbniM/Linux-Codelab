#pragma once
#include<pthread.h>
#include<semaphore.h>
#include<functional>


template<class T>
class ring_queue
{
public:
    ring_queue(int cap=5):_cap(cap),_v(cap)
    {
        pthread_mutex_init(&_consumer_mutex,nullptr);
        pthread_mutex_init(&_productor_mutex,nullptr);
        sem_init(&_room,0,cap);
        sem_init(&_data,0,0);
    }
    void push(T& data)
    {
        sem_wait(&_room);
        pthread_mutex_lock(&_consumer_mutex);
        _v[_consumer_index++]=data;
        _consumer_index%=_cap;
        data++;
        pthread_mutex_unlock(&_consumer_mutex);
        sem_post(&_data);
    }
    T pop()
    {
        sem_wait(&_data);
        pthread_mutex_lock(&_productor_mutex);
        
        const T& temp=_v[_productor_index++];
        _productor_index%=_cap;

        pthread_mutex_unlock(&_productor_mutex);
        sem_post(&_room);
        return temp;
    }
    ~ring_queue()
    {
        pthread_mutex_destroy(&_consumer_mutex);
        pthread_mutex_destroy(&_productor_mutex);
        sem_destroy(&_room);
        sem_destroy(&_data);
    }
private:
    std::vector<T> _v;
    pthread_mutex_t _consumer_mutex;
    pthread_mutex_t _productor_mutex;
    sem_t _room;
    sem_t _data;
    int _consumer_index=0;
    int _productor_index=0;
    int _cap;
};
