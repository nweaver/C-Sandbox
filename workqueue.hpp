#ifndef WORKQUEUE_HPP
#define WORKQUEUE_HPP

#include <queue>
#include <mutex>

template <class T> class WorkQueue {
    public:
    WorkQueue(){};

    void put(T &element){
        std::unique_lock l(lock);
        auto wasempty = data.empty();
        data.push(element);
        if (wasempty) notify.notify_one();
    }

    T get(){
        std::unique_lock l(lock);
        while (data.empty()){
            notify.wait(l);
        }
        auto ret = data.front();
        data.pop();
        return ret;
    }

    private:
    std::queue<T> data;
    std::mutex lock;
    std::condition_variable notify;
};

#endif