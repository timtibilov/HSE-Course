#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

    struct NullOpt {
        NullOpt() noexcept = default;
    };

    constexpr NullOpt kNullOpt = {};

    struct InPlace {
        InPlace() noexcept = default;
    };

    constexpr InPlace kInPlace = {};

    template <typename T>
    class Optional {
    public:
        using value_type = T;
        using pointer = typename std::pointer_traits<value_type*>::pointer;
        using const_pointer = const pointer;

        constexpr Optional() noexcept : ptr_(nullptr) {}

        template <typename U = value_type>
        constexpr explicit Optional(U&& value) : ptr_(new value_type(std::forward<U>(value))) {}

        constexpr explicit Optional(NullOpt) noexcept : ptr_(nullptr) {}

        template <typename... Args>
        constexpr explicit Optional(InPlace, Args&&... args) : ptr_(new value_type(std::forward<Args>(args)...)) {}

        Optional& operator=(NullOpt) noexcept;

        template <typename U = T>
        Optional& operator=(U&& value);

        void Reset() noexcept;

        template <typename U>
        constexpr T ValueOr(U&& default_value) const& { return HasValue() ? *ptr_ : default_value; }

        template <typename U>
        constexpr T ValueOr(U&& default_value) && { return HasValue() ? *ptr_ : default_value; }

        constexpr bool HasValue() const noexcept { return ptr_ != nullptr; }

        constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

        constexpr const_pointer operator->() const { return ptr_; }

        constexpr pointer operator->() { return ptr_; }

        constexpr const value_type& operator*() const& { return *ptr_; }

        constexpr value_type& operator*() & { return *ptr_; }

        constexpr const value_type&& operator*() const&& { return *ptr_; }

        constexpr value_type&& operator*() && { return *ptr_; }

    private:
        using is_triv_dest = std::is_trivially_destructible<T>;
        pointer ptr_;

    };
}  // namespace task

template<typename T>
task::Optional<T>& task::Optional<T>::operator=(task::NullOpt) noexcept {
    Reset();
    return *this;
}

template<typename T>
template<typename U>
task::Optional<T>& task::Optional<T>::operator=(U&& value) {
    Reset();
    ptr_(new task::Optional<T>::value_type(std::forward<U>(value)));
    return *this;
}

template<typename T>
void task::Optional<T>::Reset() noexcept {
    delete ptr_;
    ptr_ = nullptr;
}
