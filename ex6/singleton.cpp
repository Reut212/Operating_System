#include <mutex>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <ftw.h>

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

int main() {
    int fd = open("example.txt", O_CREAT | O_RDWR | O_SYNC, 0666);
    struct stat st;
    fstat(fd, &st);
    uint8_t *byte_ptr = (uint8_t *) mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    singleton<uint8_t *> *singleton1 = singleton<uint8_t *>::Instance(byte_ptr);
    singleton<uint8_t *> *singleton2 = singleton<uint8_t *>::Instance(byte_ptr);
    if (singleton1==singleton2){
        printf("pass!");
    }
    singleton1->Destroy();
    singleton2->Destroy();
    close(fd);
}