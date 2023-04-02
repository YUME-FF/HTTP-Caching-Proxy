#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <thread>

class MyThread {
public:
    template <typename Func, typename... Args>
    MyThread(Func&& func, Args&&... args) {
        _thread = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
        _thread.detach();
    }
    ~MyThread() = default;
private:
    std::thread _thread;
};

#endif // MYTHREAD_H