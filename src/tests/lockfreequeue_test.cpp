#include "../eventloop.h"

#include <thread>
#include <random>

void enque_element(mipha::lockfreequeue<int, 2>& q) {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(-20000,20000); // 左闭右闭区间
    e.seed(time(0));

    for (int i = 0; i < 32768; ++i) {
        q.enque(u(e));
    }
}

int deque_element(mipha::lockfreequeue<int, 2>& q) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "here will be 32768 nums: ";
    size_t nums = 0;
    while (q.queue_size()) {
        q.deque();
        ++nums;
//        std::cout << q.deque() << " ";
    }
    std::cout << nums << std::endl;
    assert(nums == 32768 * 3);
}

int main() {
    mipha::lockfreequeue<int, 2> lockfreequeue_test;
    std::thread t1(enque_element, std::ref(lockfreequeue_test));
    std::thread t2(enque_element, std::ref(lockfreequeue_test));
    std::thread t3(deque_element, std::ref(lockfreequeue_test));

    t1.join();
    t2.join();
    t3.join();
    return 0;
}