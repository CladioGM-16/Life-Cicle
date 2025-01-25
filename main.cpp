#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

std::atomic<bool> chopping(true);
std::mutex sync_mutex;
int turn = 0; // Variable para alternar entre hilos (0: Olivia, 1: Barron)

void vegetable_chopper(const char* name, unsigned int& personal_count, int thread_id) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(sync_mutex);

            // Salir si chopping es falso y no es el turno del hilo
            if (!chopping.load() && turn != thread_id) break;

            // Esperar turno
            if (turn != thread_id) continue;

            // Incrementar el contador personal si chopping sigue activo
            if (chopping.load()) {
                personal_count++;
            }

            // Alternar turno
            turn = 1 - thread_id;
        }
        std::this_thread::yield(); // Ceder la CPU
    }
    printf("%s chopped %u vegetables.\n", name, personal_count);
}

int main() {
    unsigned int olivia_count = 0;
    unsigned int barron_count = 0;

    // Crear los hilos
    std::thread olivia(vegetable_chopper, "Olivia", std::ref(olivia_count), 0);
    std::thread barron(vegetable_chopper, "Barron", std::ref(barron_count), 1);

    printf("Barron and Olivia are chopping vegetables...\n");
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Esperar 2 segundos

    // Detener chopping
    chopping.store(false);

    // Esperar a que los hilos terminen
    barron.join();
    olivia.join();

}
