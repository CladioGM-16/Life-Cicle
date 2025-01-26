#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>

std::mutex io_mutex;

void calculator(const char* name) {
    printf("%s Process ID: %d\n", name, getpid());
    printf("%s Thread ID: %ld\n", name, std::this_thread::get_id());
    while (true) {
        int num1, num2;
        {
            // Bloque sincronizado para proteger entrada y salida
            std::lock_guard<std::mutex> lock(io_mutex);
            std::cout <<"Enter first number: ";
            std::cin >> num1;
            std::cout<<"Enter second number: ";
            std::cin >> num2;
            std::cout<<"Result: " << num1 + num2 << std::endl<< std::endl;
        }

        // Pausa para evitar que los hilos se ejecuten muy rápido
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    printf("Main Process ID: %d\n", getpid());
    printf("Main Thread ID: %ld\n", std::this_thread::get_id());

    // Crear los hilos
    std::thread thread1(calculator, "C1");
    std::thread thread2(calculator, "C2");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    thread1.join();
    thread2.join();
    return 0;
}
