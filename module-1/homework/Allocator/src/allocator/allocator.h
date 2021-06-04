// Implemented by Taimuraz Tibilov, based on Linear Allocator principle

#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using pointer_difference = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propogate_on_container_swap = std::false_type;
    using is_always_equal = std::false_type;

    CustomAllocator();
    CustomAllocator(const CustomAllocator& other) noexcept;
    ~CustomAllocator();

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    pointer allocate(size_type n);
    void deallocate(T* p, size_t n);
    template <typename... Args>
    void construct(pointer p, Args&&... args);
    void destroy(pointer p) noexcept;
    size_type max_size();

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;
    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    void* _arena;
    int* _offset;
    int* _ref_count;
    int _arena_size = 65536; // 2 ^ 16
};

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs._arena == rhs._arena;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}

template<typename T>
CustomAllocator<T>::CustomAllocator() {
    _arena = ::operator new(_arena_size * sizeof(T));
    _offset = new int(0);
    _ref_count = new int(1);
}

template<typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept :
    _arena(other._arena),
    _offset(other._offset),
    _ref_count(other._ref_count)
{
    (*_ref_count)++;
}

template<typename T>
template<typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept :
    _arena(other._arena),
    _offset(other._offset),
    _ref_count(other._ref_count)
{
    (*_ref_count)++;
}

template<typename T>
CustomAllocator<T>::~CustomAllocator() {
    if (--(*_ref_count) == 0) {
        ::operator delete(_arena);
        delete _offset;
        delete _ref_count;
    }
}

template<typename T>
T* CustomAllocator<T>::allocate(std::size_t n) {
    if (n > max_size())
        throw (std::bad_alloc());
    int offset = *_offset;
    *_offset += n;
    return static_cast<pointer>(_arena) + offset;
}

template<typename T>
void CustomAllocator<T>::deallocate(T* p, std::size_t n) {}

template<typename T>
template<typename... Args>
void CustomAllocator<T>::construct(CustomAllocator::pointer p, Args&& ... args) {
    ::new((void *)p) T(std::forward<Args>(args)...);
}

template<typename T>
void CustomAllocator<T>::destroy(CustomAllocator::pointer p) noexcept {
    p->~T();
}

template<typename T>
std::size_t CustomAllocator<T>::max_size() {
    return _arena_size - *_offset;
}