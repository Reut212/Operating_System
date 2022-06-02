#include <mutex>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

template<typename T>
class singleton {
private:
    static bool initialized;
    static singleton *instance;
    T data;
    static pthread_mutex_t lock;

    singleton(T temp);

public:
    static singleton *Instance(T temp);

    void Destroy();
};

template<typename T> bool singleton<T>::initialized = false;
template<typename T> singleton<T> *singleton<T>::instance = NULL;
template<typename T> pthread_mutex_t singleton<T>::lock = PTHREAD_MUTEX_INITIALIZER;

template<typename T>
singleton<T> *singleton<T>::Instance(T data2) {
    if (!initialized) { // no available instance
        pthread_mutex_lock(&lock);
        instance = new singleton(data2);
        initialized = true;
    }
    pthread_mutex_unlock(&lock);
    return instance;
}

// constructor
template<typename T>
singleton<T>::singleton(T data2) {
    data = data2;
}

template<typename T>
void singleton<T>::Destroy() {
    initialized = false;
}

void *map(void *arg) {
    int fd = *((int *) arg);
    char* message = "example file";
    int fsize = strlen(message);
    void* addr = mmap(NULL, fsize, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ftruncate(fd, fsize);
    memcpy(addr, message, strlen(message));
    msync(addr, fsize, MS_SYNC);
    munmap(addr, fsize);
}

int main() {
    int fd = open("example.txt", O_CREAT | O_RDWR | O_TRUNC, 0664);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, map, &fd);
    pthread_create(&t2, NULL, map, &fd);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
//    fclose(out);
}