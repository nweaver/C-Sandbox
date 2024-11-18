#ifndef WORKQUEUE_HPP
#define WORKQUEUE_HPP

#include <queue>
#include <mutex>

template <class T>
class WorkQueue
{
private:

public:
    WorkQueue()  {};
    WorkQueue(const WorkQueue&) = delete;



    void operator=(const WorkQueue&) = delete;

    void put(T &element)
    {
        // Doing it this way means we release the lock
        // before we notify, eliminating the accidental
        // "wakup and wait on lock" problem that might
        // otherwise occur.
        bool wasempty = false;
        {
            std::unique_lock l(lock);
            wasempty = data.empty();
            data.push(element);
        }
        if (wasempty)
            notify.notify_all();
    }

    T get()
    {
        std::unique_lock l(lock);
        while (data.empty())
        {
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