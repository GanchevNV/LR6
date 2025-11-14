#include <iostream>
#include <vector>
#include <execution>
#include <chrono>
#include <random>
#include <thread>

template<typename T, typename ParallelPolicy>
void removeElements(std::vector<T> &collection, const T &REMOVE_ELEM, ParallelPolicy policy,
                           const std::string &policyName) {
    // Время в начале выполнения алгоритма
    const auto start = std::chrono::high_resolution_clock::now();

    // ОСНОВНАЯ ЧАСТЬ КОДА
    // Собираем все значения в конце от iter и удаляем
    auto iter = std::remove(policy, collection.begin(), collection.end(), REMOVE_ELEM);
    collection.erase(iter, collection.end());

    // Время в конце выполнения алгоритма
    const auto end = std::chrono::high_resolution_clock::now();
    // Расчёт разницы между концом и началом
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); // ИСПРАВЛЕНО: microseconds вместо nanoseconds

    // Вывод в консоль
    std::cout << policyName << ": " << duration.count() << " microseconds" << std::endl;
}

std::vector<int> generateTestData(const size_t SIZE) {
    // Массив, в который будет записана генерация
    std::vector<int> data(SIZE);
    // Настройки генератора
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    // Цикл ввода случайных данных
    for (size_t i = 0; i < SIZE; ++i) {
        data[i] = dist(gen);
    }
    return data;
}

void testParallel() {
    // Размер создаваемого массива для проверки
    size_t data_size;
    std::cout << "Input a collection size: ";
    std::cin >> data_size;
    // Генерируем тестовые данные
    std::vector<int> testData = generateTestData(data_size);

    // Элемент для удаления
    int rm_element;
    std::cout << "Input value for remove: ";
    std::cin >> rm_element;
    // Проверка ввода
    while ((rm_element > 100) || (rm_element < 0)) {
        std::cout << "Value must be in [0; 100] range" << std::endl;
        std::cout << "Input value for remove: ";
        std::cin >> rm_element;
    }

    std::cout << std::endl;
    std::cout << "Collection size: " << data_size << " elements" << std::endl;
    std::cout << "Remove value: " << rm_element << std::endl;
    std::cout << "Available threads: " << std::thread::hardware_concurrency() << std::endl;

    // Тестируем различные политики выполнения

    // Базовая версия (без политики)
    std::cout << "Base version (without policies): ";
    auto data1 = testData;
    auto start = std::chrono::high_resolution_clock::now();
    auto iter1 = std::remove(data1.begin(), data1.end(), rm_element);
    data1.erase(iter1, data1.end());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " microseconds" << std::endl;

    // Последовательная политика
    auto data2 = testData;
    removeElements(data2, rm_element, std::execution::seq, "Sequenced policy (seq)");

    // Параллельная + векторная политика
    auto data5 = testData;
    removeElements(data5, rm_element, std::execution::unseq, "Unsequenced_policy (unseq)");

    // Параллельная политика
    auto data3 = testData;
    removeElements(data3, rm_element, std::execution::par, "Parallel_policy (par)");

    // Параллельная + векторная политика
    auto data4 = testData;
    removeElements(data4, rm_element, std::execution::par_unseq, "Parallel_unsequenced_policy (par_unseq)");
}

// Функция для демонстрации работы алгоритма
void demonstrateAlgorithm() {
    std::cout << "Demonstration algorithm work" << std::endl;

    std::vector<int> data = {1, 2, 3, 2, 4, 2, 5, 6, 2, 7};
    int elementToRemove = 2;

    std::cout << "Stock collection: ";
    for (auto i = data.begin(); i != data.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;

    auto iter = std::remove(data.begin(), data.end(), elementToRemove);
    data.erase(iter, data.end());

    std::cout << "After remove " << elementToRemove << ": ";
    for (auto i = data.begin(); i != data.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl << std::endl;
}

int main() {
    system("chcp 65001");
    demonstrateAlgorithm();
    testParallel();
    return 0;
}
