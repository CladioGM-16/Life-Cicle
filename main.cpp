#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>
std::mutex cout_mutex;

void say_hello() {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Hello Thread PID: " << getpid() << std::endl;
        std::cout << "Hello Thread Thread ID: " << std::this_thread::get_id() << std::endl;
    }
    while (true) {
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Hello" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void say_goodbye() {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[Goodbye Thread] PID: " << getpid() << std::endl;
        std::cout << "[Goodbye Thread] Thread ID: " << std::this_thread::get_id() << std::endl;
    }

    while (true) {
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Goodbye" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
int main() {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[Main Thread] Process ID: " << getpid() << std::endl;
        std::cout << "[Main Thread] Thread ID: " << std::this_thread::get_id() << std::endl;
    }
    std::thread thread_hello(say_hello);
    std::thread thread_goodbye(say_goodbye);
    std::cout << "Greet" << std::endl;
    thread_hello.join();
    thread_goodbye.join();
    return 0;
}
