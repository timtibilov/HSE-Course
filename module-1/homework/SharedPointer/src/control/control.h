#pragma once

#include <atomic>

template<typename T>
struct default_deleter {
    void operator()(T* ptr) {
        delete ptr;
    }
};

template<typename T>
struct default_deleter<T[]> {
    template <class U>
    void operator()(U* ptr) {
        delete[] ptr;
    }
};

class SharedCount {
public:
    SharedCount(std::size_t count = 0) noexcept : strong_ref_count(count) {}
    SharedCount(const SharedCount&) = delete;
    SharedCount(SharedCount&&) = delete;

    void retainShared() noexcept {
        strong_ref_count.fetch_add(1);
    }

    bool releaseShared() noexcept {
        if (strong_ref_count.fetch_sub(1) == 0) {
            deletePtr();
            return true;
        }
        return false;
    }

    std::size_t useCount() const noexcept {
        return strong_ref_count + 1;
    }

    virtual void deletePtr() = 0;

protected:
    std::atomic<std::size_t> strong_ref_count;

    virtual ~SharedCount() {};
};

class SharedWeakCount : public SharedCount {
public:
    SharedWeakCount(std::size_t weak = 0, std::size_t strong = 0) noexcept : weak_ref_count(weak), SharedCount(strong) {}
    SharedWeakCount(const SharedWeakCount&) = delete;
    SharedWeakCount(SharedWeakCount&&) = delete;

    void retainWeak() noexcept {
        weak_ref_count.fetch_add(1);
    }

    void releaseWeak() noexcept {
        weak_ref_count.fetch_sub(1);
    }

    void releaseShared() noexcept {
        if (SharedCount::releaseShared()) {
            releaseWeak();
        }
    }

    virtual void deletePtr() noexcept = 0;

    virtual SharedWeakCount* lock() = 0;

protected:
    std::atomic<std::size_t> weak_ref_count;

    virtual ~SharedWeakCount() {};
};

template <typename T, typename Deleter = default_deleter<std::remove_pointer_t<T>>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock(T ptr) : ptr_(ptr) {
        if (std::is_trivially_constructible_v<Deleter>)
            deleter_ = Deleter();
        else
            deleter_ = default_deleter<std::remove_pointer_t<T>>();
    }
    ControlBlock(T ptr, Deleter deleter) : ptr_(ptr), deleter_(deleter) {}

    ControlBlock(const ControlBlock<T, Deleter>&) = delete;
    ControlBlock(ControlBlock<T, Deleter>&&) = delete;

    void deletePtr() noexcept override {
        deleter_(ptr_);
    }

    SharedWeakCount* lock() override {
        if (useCount() > 0) {
            ControlBlock* block = new ControlBlock(ptr_, deleter_);
            block->retainShared();
            return block;
        }
        return nullptr;
    }

private:
    T ptr_;
    Deleter deleter_;
};