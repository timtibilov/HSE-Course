#pragma once

#include <list>
#include <memory>
#include <type_traits>

namespace task {

template <typename T, typename Allocator = std::allocator<T>>
class list {
public:
    class iterator;
private:
    class Node;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using const_iterator = const iterator;
    using size_type = std::size_t;

public:
    class iterator {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        explicit iterator(Node* ptr): _ptr(ptr) {}

        iterator& operator++() {
            _ptr = _ptr->getNext();
            return *this;
        }

        iterator operator++(int) {
            iterator copy(*this);
            ++(*this);
            return copy;
        }

        iterator& operator--() {
            _ptr = _ptr->getPrev();
            return *this;
        }

        iterator operator--(int) {
            iterator copy(*this);
            --(*this);
            return copy;
        }

        bool operator==(const iterator& rhs) { return _ptr == rhs._ptr; }
        bool operator!=(const iterator& rhs) { return _ptr != rhs._ptr; }

        reference operator*() { return _ptr->getValueRef(); }
        pointer operator->() { return &_ptr->getValueRef(); }

    private:
        Node* _ptr;
    };

private:
    class Node {
    public:
        Node() : value(), next(nullptr), prev(nullptr) {}

        explicit Node(value_type value, Node* next = nullptr, Node* prev = nullptr) {
            this->value = value;
            this->next = next;
            this->prev = prev;
        }

        value_type getValue() const { return value; }
        reference getValueRef() { return value; }
        const_reference getValueRef() const { return value;}

        Node* getNext() { return next; }
        Node* getPrev() { return prev; }

        const Node* getNext() const { return next; }
        const Node* getPrev() const { return prev; }

        void setNext(Node* newNext) { next = newNext; }
        void setPrev(Node* newPrev) { prev = newPrev; }

        ~Node() {
            this->next = nullptr;
            this->prev = nullptr;
        }

    private:
        value_type value;
        Node* prev;
        Node* next;
    };

public:

    list();

    list(const list& other);
    list(const list& other, const Allocator& alloc);

    list(list&& other);
    list(list&& other, const Allocator& alloc);

    ~list();

    list& operator=(const list& other);

    list& operator=(list&& other) noexcept;

    // Element access
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // Iterators
    iterator begin() noexcept { return iterator(NIL->getNext()); }
    const_iterator begin() const noexcept { return iterator(NIL->getNext()); }

    iterator end() noexcept { return iterator(NIL); }
    const_iterator end() const noexcept { return iterator(NIL); }

    // Capacity
    bool empty() const noexcept { return _size == 0; }

    size_type size() const noexcept { return _size; }
    size_type maxSize() const noexcept { return _m_alloc.max_size(); }

    // Modifiers
    void clear();
    void swap(list& other);

    void pushBack(const T& value);
    void pushBack(T&& value);
    
    template <typename... Args>
    void emplaceBack(Args&&... args);
    void popBack();
    void pushFront(const T& value);
    void pushFront(T&& value);
    template <typename... Args>
    void emplaceFront(Args&&... args);
    void popFront();

    void resize(size_type count);

    // Operations
    void remove(const T& value);
    void unique();
    void sort();

    allocator_type getAllocator() const noexcept { return allocator_type(_m_alloc); }

private:
    using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using node_alloc_traits = typename std::allocator_traits<node_allocator>;

    void remove(Node* node);
    static list merge(const list& left, const list& right);

    node_allocator _m_alloc;
    Node* NIL;
    size_type _size;
};

}  // namespace task

template<typename T, typename Allocator>
task::list<T, Allocator>::list() : NIL(_m_alloc.allocate(1)) {
    _size = 0;
    _m_alloc.construct(NIL);
    NIL->setNext(NIL);
    NIL->setPrev(NIL);
}

template<typename T, typename Allocator>
task::list<T, Allocator>::list(const task::list<T, Allocator>& other) :
    _m_alloc(node_alloc_traits::select_on_container_copy_construction(other._m_alloc)),
    NIL(_m_alloc.allocate(1))
{
    _size = 0;
    _m_alloc.construct(NIL);
    NIL->setNext(NIL);
    NIL->setPrev(NIL);

    for (auto it = other.begin(); it != other.end(); ++it)
        pushBack(*it);
}

template<typename T, typename Allocator>
task::list<T, Allocator>::list(const task::list<T, Allocator>& other, const Allocator& alloc) :
    _m_alloc(alloc),
    NIL(_m_alloc.allocate(1))
{
    _m_alloc.construct(NIL);
    NIL->setPrev(NIL);
    NIL->setNext(NIL);
    _size = 0;

    for (auto it = other.begin(); it != other.end(); ++it)
        pushBack(*it);
}

template<typename T, typename Allocator>
task::list<T, Allocator>::list(task::list<T, Allocator>&& other, const Allocator& alloc) :
    _m_alloc(alloc),
    NIL(std::move(other.NIL)),
    _size(std::move(other._size))
{
    other.NIL = nullptr;
}

template<typename T, typename Allocator>
task::list<T, Allocator>::list(task::list<T, Allocator>&& other) :
    _m_alloc(std::move(other._m_alloc)),
    NIL(std::move(other.NIL)),
    _size(std::move(other._size))
{
    other.NIL = nullptr;
}

template<typename T, typename Allocator>
task::list<T, Allocator>::~list() {
    if (NIL == nullptr)
        return;

    Node* curr = NIL->getNext();
    while (curr && curr->getNext() != NIL) {
        Node* to_free = curr;
        curr = curr->getNext();
        _m_alloc.destroy(to_free);
        _m_alloc.deallocate(to_free, 1);
    }
    _m_alloc.destroy(NIL);
    _m_alloc.deallocate(NIL, 1);
}

template<typename T, typename Allocator>
task::list<T, Allocator>& task::list<T, Allocator>::operator=(const task::list<T, Allocator>& other) {
    if (this != &other) {
        if (node_alloc_traits::propagate_on_container_copy_assignment::value) {
            if (_m_alloc != other._m_alloc)
                clear();
            _m_alloc = other._m_alloc;
        }
        clear();
        Node* curr = other.NIL->getNext();
        for (int i = 0; i < other.size(); ++i) {
            pushBack(curr->getValue());
            curr = curr->getNext();
        }
        _size = other.size();
    }
    return *this;
}

template<typename T, typename Allocator>
task::list<T, Allocator>& task::list<T, Allocator>::operator=(task::list<T, Allocator>&& other) noexcept {
    clear();
    if (node_alloc_traits::propagate_on_container_move_assignment::value) {
        _m_alloc = std::move(other._m_alloc);
        _size = std::move(other._size);
        other._size = std::move(0);
        NIL = std::move(other.NIL);
        other.NIL = new Node();
        other.NIL->setNext(other.NIL);
        other.NIL->setPrev(other.NIL);
    } else if (node_alloc_traits::is_always_equal::value || _m_alloc == other._m_alloc) {
        _size = std::move(other._size);
        other._size = std::move(0);
        NIL = std::move(other.NIL);
        other.NIL = new Node();
        other.NIL->setNext(other.NIL);
        other.NIL->setPrev(other.NIL);
    } else  {
        for (iterator it = other.begin(); it != other.end(); ++it)
            pushBack(std::move(*it));
    }
    return *this;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::clear() {
    while (size()) {
        popBack();
    }
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::pushBack(const T& value) {
    Node* last = NIL->getPrev();
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, value, NIL, last);
    NIL->setPrev(newNode);
    last->setNext(newNode);
    ++_size;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::pushBack(T&& value) {
    Node* last = NIL->getPrev();
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, std::forward<T>(value),  NIL, last);
    NIL->setPrev(newNode);
    last->setNext(newNode);
    ++_size;
}

template<typename T, typename Allocator>
template<typename... Args>
void task::list<T, Allocator>::emplaceBack(Args&& ... args) {
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, std::forward<Args>(args)...);

    Node* last = NIL->getPrev();
    NIL->setPrev(newNode);
    newNode->setNext(NIL);
    newNode->setPrev(last);
    last->setNext(newNode);
    ++_size;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::popBack() {
    Node* last = NIL->getPrev();
    if (last == NIL)
        throw std::logic_error("Cannot pop from empty list");
    remove(last);
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::pushFront(const T& value) {
    Node* first = NIL->getNext();
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, value, first, NIL);
    first->setPrev(newNode);
    NIL->setNext(newNode);
    ++_size;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::pushFront(T&& value) {
    Node* first = NIL->getNext();
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, std::forward<T>(value), first, NIL);
    first->setPrev(newNode);
    NIL->setNext(newNode);
    ++_size;
}

template<typename T, typename Allocator>
template<typename... Args>
void task::list<T, Allocator>::emplaceFront(Args&& ... args) {
    Node* newNode = static_cast<Node*>(_m_alloc.allocate(1));
    _m_alloc.construct(newNode, std::forward<Args>(args)...);

    Node* first = NIL->getNext();
    NIL->setNext(newNode);
    newNode->setPrev(NIL);
    newNode->setNext(first);
    first->setPrev(newNode);
    ++_size;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::popFront() {
    Node* first = NIL->getNext();
    if (first == NIL)
        throw std::logic_error("Cannot pop from empty list");
    remove(first);
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::resize(size_t count) {
    while (_size > count)
        popBack();
    while (_size < count)
        pushBack({});
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::remove(const T& value) {
    Node *tmp = NIL->getNext();
    while (tmp != NIL) {
        tmp = tmp->getNext();
        if (tmp->getPrev()->getValue() == value)
            remove(tmp->getPrev());
    }
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::remove(Node* node) {
    Node* prev = node->getPrev();
    Node* next = node->getNext();
    prev->setNext(next);
    next->setPrev(prev);
    _m_alloc.destroy(node);
    _m_alloc.deallocate(node, 1);
    --_size;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::unique() {
    Node *tmp = NIL->getNext()->getNext();

    while (tmp->getPrev() != NIL) {
        Node *prev = tmp->getPrev();
        if (prev->getValue() == tmp->getValue())
            remove(prev);
        tmp = tmp->getNext();
    }
}

template<typename T, typename Allocator>
task::list<T, Allocator> task::list<T, Allocator>::merge(const task::list<T, Allocator>& first, const task::list<T, Allocator>& second) {
    task::list<T, Allocator> result;
    Node* fHead = first.NIL->getNext();
    Node* sHead = second.NIL->getNext();
    while (fHead != first.NIL || sHead != second.NIL) {
        if (sHead == second.NIL) {
            result.pushBack(std::move(fHead->getValue()));
            fHead = fHead->getNext();
        } else if (fHead == first.NIL) {
            result.pushBack(std::move(sHead->getValue()));
            sHead = sHead->getNext();
        } else if (sHead->getValue() > fHead->getValue()) {
            result.pushBack(std::move(fHead->getValue()));
            fHead = fHead->getNext();
        } else {
            result.pushBack(std::move(sHead->getValue()));
            sHead = sHead->getNext();
        }
    }
    return result;
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::sort() {
    if (_size < 2)
        return;
    task::list<T, Allocator> left;
    task::list<T, Allocator> right(*this);
    Node* tmp = NIL->getNext();
    for (size_t i = 0; i < _size / 2; i++) {
        left.pushBack(std::move(tmp->getValue()));
        tmp = tmp->getNext();
        right.popFront();
    }
    left.sort();
    right.sort();
    auto res = merge(left, right);
    *this = std::move(res);
}

template<typename T, typename Allocator>
void task::list<T, Allocator>::swap(task::list<T, Allocator>& other) {
    if (node_alloc_traits::propagate_on_container_swap::value)
        std::swap(_m_alloc, other._m_alloc);
    else if (_m_alloc == other._m_alloc)
        throw std::runtime_error("Swap with different allocators");
    std::swap(NIL, other.NIL);
    std::swap(_size, other._size);
}

template<typename T, typename Allocator>
typename task::list<T, Allocator>::reference task::list<T, Allocator>::front() {
    if (!size())
        throw std::runtime_error("Cannot return front of empty list");

    return NIL->getNext()->getValueRef();
}

template<typename T, typename Allocator>
typename task::list<T, Allocator>::const_reference task::list<T, Allocator>::front() const {
    if (!size())
        throw std::runtime_error("Cannot return front of empty list");

    return NIL->getNext()->getValueRef();
}

template<typename T, typename Allocator>
typename task::list<T, Allocator>::reference task::list<T, Allocator>::back() {
    if (!size())
        throw std::runtime_error("Cannot return back of empty list");

    return NIL->getPrev()->getValueRef();
}

template<typename T, typename Allocator>
typename task::list<T, Allocator>::const_reference task::list<T, Allocator>::back() const {
    if (!size())
        throw std::runtime_error("Cannot return back of empty list");

    return NIL->getPrev()->getValueRef();
}
