#pragma once

#include "../control/control.h"

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = std::remove_extent_t<T>;
    using pointer = element_type*;
    using weak_type = WeakPtr<T>;

    constexpr SharedPtr() noexcept = default;
    ~SharedPtr();

    template <typename Y>
    explicit SharedPtr(Y* p) : ptr_(p), control_(new ControlBlock<pointer>(p)) {}

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept : ptr_(p), control_(new ControlBlock<pointer, Deleter>(p, deleter)) {}

    SharedPtr(const SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;

    SharedPtr& operator=(const SharedPtr& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept;

    SharedPtr& operator=(SharedPtr&& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept;

    // Modifiers
    void reset() noexcept;

    template <typename Y>
    void reset(Y* p) noexcept;

    template <typename Y, typename Deleter>
    void reset(Y* p, Deleter deleter) noexcept;

    void swap(SharedPtr& other) noexcept;

    // Observers
    T* get() const noexcept { return ptr_; }
    std::size_t useCount() const noexcept;
    T& operator*() const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    element_type& operator[](std::ptrdiff_t idx) const { return *(ptr_ + idx); }
    explicit operator bool() const noexcept;

    template <typename U>
    friend class WeakPtr;

private:
    pointer ptr_ = nullptr;
    SharedWeakCount* control_ = nullptr;
};


// MakeShared
template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
// MakeShared

// SharedPtr
template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (control_ != nullptr) {
        control_->releaseShared();
    }
    ptr_ = nullptr;
    control_ = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept :
    ptr_(other.ptr_),
    control_(other.control_)
{
    control_->retainShared();
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept :
    ptr_(other.ptr_),
    control_(other.control_)
{
    other.ptr_ = nullptr;
    other.control_ = nullptr;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& r) noexcept {
    SharedPtr<T>(r).swap(*this);
    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& r) noexcept {
    SharedPtr<T>(std::forward<SharedPtr>(r)).swap(*this);
    return *this;
}

template<typename T>
void SharedPtr<T>::reset() noexcept {
    ptr_ = nullptr;
    if (control_ != nullptr) {
        control_->releaseShared();
    }
}

template<typename T>
template<typename Y>
void SharedPtr<T>::reset(Y* p) noexcept {
    SharedPtr<T>(p).swap(*this);
}

template<typename T>
template<typename Y, typename Deleter>
void SharedPtr<T>::reset(Y* p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).swap(*this);
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr &other) noexcept {
    std::swap(control_, other.control_);
    std::swap(ptr_, other.ptr_);
}

template<typename T>
std::size_t SharedPtr<T>::useCount() const noexcept {
    return control_ == nullptr ? 0 : control_->useCount();
}

template<typename T>
SharedPtr<T>::operator bool() const noexcept {
    return useCount() > 0 && ptr_ != nullptr;
}
// SharedPtr

// WeakPtr
template <typename T>
class WeakPtr {

public:
    using element_type = std::remove_extent_t<T>;
    using pointer = T*;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;
    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other);
    WeakPtr(const WeakPtr& other) noexcept;
    WeakPtr(WeakPtr&& other) noexcept;
    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other);
    WeakPtr& operator=(const WeakPtr& other) noexcept;
    WeakPtr& operator=(WeakPtr&& other) noexcept;

    ~WeakPtr();

    // Modifiers
    void reset() noexcept;
    void swap(WeakPtr<T>& other) noexcept;

    // Observers
    bool expired() noexcept;
    SharedPtr<T> lock() const noexcept;

    template <typename U>
    friend class SharedPtr;

private:
    pointer ptr_ = nullptr;
    SharedWeakCount* control_ = nullptr;
};

// WeakPtr

template<typename T>
template<typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y>& other) :
    ptr_(other.ptr_),
    control_(other.control_)
{
    if (control_ != nullptr)
        control_->retainWeak();
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept :
    ptr_(other.ptr_),
    control_(other.control_)
{
    if (control_ != nullptr)
        control_->retainWeak();
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept :
    ptr_(other.ptr_),
    control_(other.control_)
{
    other.ptr_ = nullptr;
    other.control_ = nullptr;
}

template<typename T>
template<typename Y>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<Y>& other) {
    reset();
    control_ = other.control_;
    ptr_ = other.ptr_;
    if (control_ != nullptr) {
        control_->retainWeak();
    }
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    if (this == other) {
        return *this;
    }
    reset();
    ptr_ = other.ptr_;
    control_ = other.control_;
    if (control_ != nullptr) {
        control_->retainWeak();
    }
    return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    reset();
    ptr_ = other.ptr_;
    control_ = other.control_;
    other.ptr_ = nullptr;
    other.control_ = nullptr;
    return *this;
}

template<typename T>
WeakPtr<T>::~WeakPtr() {
    if (control_ != nullptr) {
        control_->releaseWeak();
    }
}

template<typename T>
void WeakPtr<T>::reset() noexcept {
    if (control_ != nullptr && ptr_ != nullptr) {
        ptr_ = nullptr;
        control_->releaseWeak();
        control_ = nullptr;
    }
}

template<typename T>
void WeakPtr<T>::swap(WeakPtr<T>& other) noexcept {
    std::swap(ptr_, other.ptr_);
    std::swap(control_, other.control_);
}

template<typename T>
bool WeakPtr<T>::expired() noexcept {
    return control_ == nullptr || control_->useCount() <= 0l;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept {
    SharedPtr<T> ptr;
    ptr.control_ = control_ != nullptr ? control_->lock() : control_;
    if (ptr.control_ != nullptr) {
        ptr.ptr_ = this->ptr_;
    }
    return ptr;
}

// WeakPtr