#ifndef WORKQUEUE_HPP
#define WORKQUEUE_HPP

#include <queue>
#include <mutex>

template <class T>
class WorkQueue
{
private:
public:
    WorkQueue() {};
    // It doesn't make sense to hold 0 elements, and
    // we use 0 capacity to be "unlimited capacity"
    WorkQueue(size_t size)
    {
        capacity = size;
        assert(size > 0);
    };

    // We define our WorkQueue as NOT being copyable or movable, as it has non-copyable
    // components in it, and it would be undefined behavior even to
    // move it with things waiting on the old condition variables.  To
    // actually make it moveable there would need to be an extra layer of
    // indirection to allow not having to actually move the queue and
    // condition variable.
    WorkQueue(const WorkQueue &) = delete;
    void operator=(const WorkQueue &) = delete;

    void put(T &element)
    {
        // This convention is so that we don't cause a
        // "wake up and lock again" immediately on the other
        // thread.  Instead we see if we will need to
        // notify and if so, notify AFTER we release the lock.
        bool wasempty = false;
        {
            std::unique_lock l(lock);
            // If there are already more elements than capacity
            // we wait.  Note that capacity 0 is special so...
            while (capacity != 0 &&
                   data.size() >= capacity)
            {
                notify_put.wait(l);
            }
            wasempty = data.empty();
            data.push(element);
        }
        if (wasempty)
            notify_get.notify_one();
    }

    T get()
    {
        bool wasfull = false;
        std::unique_lock l(lock);
        while (data.empty())
        {
            notify_get.wait(l);
        }
        if (data.size() >= capacity)
        {
            wasfull = true;
        }
        auto ret = data.front();
        data.pop();
        // Doing an explicit unlock rather than RAII unlock because
        // of scoping issues with ret.
        l.unlock();
        if (wasfull)
        {
            notify_put.notify_one();
        }
        return ret;
    }

private:
    std::queue<T> data;
    std::mutex lock;
    std::condition_variable notify_get;
    std::condition_variable notify_put;
    size_t capacity = 0;
};

#endif