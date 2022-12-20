#pragma once

#include <atomic>
#include <memory>
#include <iostream>

namespace mipha {

class atomiclock {
public:
    atomiclock() : flag(ATOMIC_FLAG_INIT) {}

    void lock () {
        while (this->flag.test_and_set(std::memory_order_acquire));
    }

    bool try_lock() {
        return !this->flag.test_and_set(std::memory_order_acquire);
    }

    void unlock() {
        this->flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag flag;
};

class atomiclockguard {
public:
    atomiclockguard(atomiclock& lock) : lockguard(lock) {
        lockguard.lock();
    }
    ~atomiclockguard() {
        lockguard.unlock();
    }
private:
    atomiclock& lockguard;
};

template<typename T, size_t buffer_size>
class lockfreequeue final {
public:
    lockfreequeue() : m_head(0), m_tail(0), m_queue_buffer_size(buffer_size), m_queue_size(0) {
        create_buffer();
    }

    lockfreequeue(const lockfreequeue<T, buffer_size>&&) = delete;
    lockfreequeue(const lockfreequeue<T, buffer_size>&) = delete;
    ~lockfreequeue() = default;

    void enque(const T& val) {
        atomiclockguard(std::ref(m_lock));
        if (m_tail == m_head && m_queue_size) {
            realloc_buffer();
        }
        m_buffer[m_tail++] = val;
        ++m_queue_size;
        if (m_tail > m_queue_buffer_size)
            m_tail = 0;
    }

    void enque(const T&& val) {
        atomiclockguard(std::ref(m_lock));
        if (m_tail == m_head && m_queue_size) {
            realloc_buffer();
        }
        m_buffer[m_tail++] = std::move(val);
        ++m_queue_size;
        if (m_tail > m_queue_buffer_size)
            m_tail = 0;
    }

    // without realloc buffer
    bool try_enque(const T& val) {
        atomiclockguard(std::ref(m_lock));
        if (m_tail == m_head && m_queue_size) {
            return false;
        }
        m_buffer[m_tail++] = val;
        ++m_queue_size;
        if (m_tail > m_queue_buffer_size)
            m_tail = 0;
        return true;
    }

    // without realloc buffer
    bool try_enque(const T&& val) {
        atomiclockguard(std::ref(m_lock));
        if (m_tail == m_head && m_queue_size) {
            return false;
        }
        m_buffer[m_tail++] = std::move(val);
        ++m_queue_size;
        if (m_tail > m_queue_buffer_size)
            m_tail = 0;
        return true;
    }
    T deque() {
        atomiclockguard(std::ref(m_lock));
        if (m_tail == m_head && !m_queue_size)
            return T();
        --m_queue_size;
        if (m_head > m_queue_buffer_size)
            m_head = 0;
        return m_buffer[m_head++];
    }

    size_t queue_size() {
        atomiclockguard(std::ref(m_lock));
        return m_queue_size;
    }

    size_t queue_buffer_size() {
        atomiclockguard(std::ref(m_lock));
        return m_queue_buffer_size;
    }
private:
    void create_buffer() {
        m_queue_buffer_size = m_queue_buffer_size < 256 ? 256 : m_queue_buffer_size; // too small need realloc buffer frequently
        atomiclockguard(std::ref(m_lock));
        m_buffer.reset(new T[m_queue_buffer_size]);
    }

    void realloc_buffer() {
        // TODO: here need fix
    }

    std::unique_ptr<T[]> m_buffer;
    size_t m_head, m_tail, m_queue_buffer_size, m_queue_size;
    atomiclock m_lock;
};
}