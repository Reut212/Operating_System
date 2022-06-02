using namespace std;

template<typename T> class singleton {
private:
    static bool initialized;
    static singleton *instance;
    T data;
    singleton(T temp);

public:
    static singleton *Instance(T temp);
    void Destroy();
};

template<typename T>bool singleton<T>::initialized = false;

template<typename T> singleton<T> *singleton<T>::Instance(T data2) {
    if (!initialized) { // no available instance
        instance = new singleton(data2);
        initialized= true;
    }
    return instance;
}

// constructor
template<typename T> singleton<T>::singleton(T data2) {
    data = data2;
}

template<typename T> void singleton<T>::Destroy() {
    initialized= false;
}
