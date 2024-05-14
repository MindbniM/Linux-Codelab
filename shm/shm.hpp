#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
template <class T, size_t N>
class shm
{
public:
    shm()
    {
    }
    bool shm_creat(const std::string &path, key_t keyid = 13141)
    {
        _path = path;
        _key = ftok(path.c_str(), keyid);
        if (_key == -1)
        {
            std::cout << "key error:" << strerror(errno) << std::endl;
            return false;
        }
        _shm_id = shmget(_key, N, IPC_CREAT | IPC_EXCL|0666);
        if (_shm_id == -1)
        {
            std::cout << "shm error:" << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }
    bool shm_get(const std::string &path, key_t keyid = 13141)
    {
        _path = path;
        _key = ftok(path.c_str(), keyid);
        if (_key == -1)
        {
            std::cout << "key error:" << strerror(errno) << std::endl;
            return false;
        }
        _shm_id = shmget(_key, N, 0);
        if (_shm_id == -1)
        {
            std::cout << "shm error:" << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }
    bool shm_at()
    {
        _ptr = (T*)shmat(_shm_id, nullptr, 0);
        if ((long long)_ptr == -1)
        {
            std::cout << "shmat error:" << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }
    ~shm()
    {
        shm_dt();
        shmctl(_shm_id,IPC_RMID,nullptr);
    }
    T& operator*()
    {
        return *_ptr;
    }
    T* operator->()
    {
        return &_ptr;
    }
    T* operator+(int n)
    {
        return _ptr+n;
    }
    T& operator[](int n)
    {
        return *(_ptr+n);
    }
    void debug()
    {
        std::cout<<_shm_id<<" "<<_ptr<<" "<<_key<<std::endl;
    }
private:
    int _shm_id;
    std::string _path;
    key_t _key;
    T *_ptr=nullptr;
    void shm_dt()
    {
        int n = shmdt(_ptr);
        if (n == -1)
        {
            std::cout << "shmdt error:" << strerror(errno) << std::endl;
        }
    }
};