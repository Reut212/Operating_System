#include <mutex>
#include <iostream>

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

class mapped_file_params;

class mapped_file_params;

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

//
//void *write_to_file(void *arg){
//    FILE* out = *((FILE **) arg);
//    singleton<FILE *>* a = singleton<FILE *>::Instance(out);
//    char* str = "hello ";
//    fwrite(&str, sizeof(str), 1, reinterpret_cast<FILE *>(a));
//}
void *map(void *arg) {
}

int main() {
//    FILE *out;
//    out = fopen("output.txt","w");
    int a = 5;
    pthread_t t1, t2;
    pthread_create(&t1, NULL, map, &a);
    pthread_create(&t2, NULL, map, &a);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
//    fclose(out);
}