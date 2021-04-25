#include <iostream>
#include <vector>
#include <queue>

template <size_t chunkSize>
class FixedAllocator {
public:
    std::queue<int8_t*> buffer;
    std::queue<int8_t*> all_memory_buffer;
    const size_t size_for_realloc = 144;

    FixedAllocator() {}

    ~FixedAllocator() {
        clear();
    }

    static FixedAllocator& single_constr() {
        static auto alloc = FixedAllocator();
        return alloc;
    };

    void reallocate() {
        int8_t* pool = static_cast<int8_t*>(::operator new(chunkSize * size_for_realloc));

        for (size_t i = 0; i < size_for_realloc; ++i) {
            buffer.push(pool + i * chunkSize);
        }

        all_memory_buffer.push(pool);
    }

    void* allocate(size_t count = 1) {
        if (!buffer.size()) {
            reallocate();
        }

        int8_t* ret = buffer.front();

        for (size_t i = 0; i < count; ++i) {
            ret = buffer.front();
            buffer.pop();
        }

        return ret;
    }

    void deallocate(void* ptr, size_t) {
        buffer.push(static_cast<int8_t*>(ptr));
    }

    void clear() {
        while (all_memory_buffer.size()) {
            ::operator delete(all_memory_buffer.front());
            all_memory_buffer.pop();
        }
    }
};

template <typename T>
class FastAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    FastAllocator() {}

    template<typename U>
    FastAllocator(const FastAllocator<U>&) {}

    T* allocate(size_t count = 1) {
        if (sizeof(T) * count == 1) {
            return static_cast<T*>(FixedAllocator<1>::single_constr().allocate(count));
        } else if (sizeof(T) * count == 4) {
            return static_cast<T*>(FixedAllocator<4>::single_constr().allocate(count));
        } else if (sizeof(T) * count == 8) {
            return static_cast<T*>(FixedAllocator<8>::single_constr().allocate(count));
        } else if (sizeof(T) * count == 24) {
            return static_cast<T*>(FixedAllocator<24>::single_constr().allocate(count));
        } else {
            return static_cast<T*>(::operator new(count * sizeof(T)));
        }
    }

    void deallocate(T* ptr, size_t count) {
        if (sizeof(T) * count == 1) {
            FixedAllocator<1>::single_constr().deallocate(ptr, count);
        } else if (sizeof(T) * count == 4) {
            FixedAllocator<4>::single_constr().deallocate(ptr, count);
        } else if (sizeof(T) * count == 8) {
            FixedAllocator<8>::single_constr().deallocate(ptr, count);
        } else if (sizeof(T) * count == 24) {
            FixedAllocator<24>::single_constr().deallocate(ptr, count);
        } else {
            ::operator delete(ptr);
        }
    }

    template<class U>
    struct rebind {
        using other = FastAllocator<U>;
    };

    template<typename U>
    bool operator!=(const FastAllocator<U>&) {
        return false;
    }

    template<typename U>
    bool operator==(const FastAllocator<U>&) {
        return true;
    }
};

template <typename T, typename Allocator = std::allocator<T>>
class List {
public:
    struct Node {
    public:
        Node* prev = nullptr;
        Node* next = nullptr;
        T value;

        Node(): prev(nullptr), next(nullptr), value(T()) {}
        Node(const T& value): prev(nullptr), next(nullptr), value(value) {}
    };

    Node* fake_node = nullptr;
    size_t sz = 0;
    using NodeAllocator = typename Allocator::template rebind<Node>::other;
    NodeAllocator node_allocator;
    Allocator allocator;

    using AllocTraits = std::allocator_traits<NodeAllocator>;

    size_t size() const {
        return sz;
    }

    void node_connect(Node* node, Node* prev_node, Node* next_node) {
        node->prev = prev_node;
        node->next = next_node;
    }

    void node_and_node_connect(Node* first_node, Node* second_node) {
        first_node->next = second_node;
        second_node->prev = first_node;
    }

    explicit List(const Allocator& alloc = Allocator()) {
        allocator = alloc;
        node_allocator = typename Allocator::template rebind<Node>::other();

        fake_node = AllocTraits::allocate(node_allocator, 1);

        node_connect(fake_node, fake_node, fake_node);
    }

    List(size_t count, const Allocator& alloc = Allocator()) {
        allocator = alloc;
        node_allocator = typename Allocator::template rebind<Node>::other();

        fake_node = AllocTraits::allocate(node_allocator, 1);

        node_connect(fake_node, fake_node, fake_node);

        Node* prev_node = fake_node;
        Node* cur_node = fake_node;

        for (size_t i = 0; i < count; ++i) {
            cur_node = AllocTraits::allocate(node_allocator, 1);
            AllocTraits::construct(node_allocator, cur_node);

            node_and_node_connect(prev_node, cur_node);
            prev_node = cur_node;

            ++sz;
        }
        node_and_node_connect(cur_node, fake_node);
    }

    List(size_t count, const T& value, const Allocator& alloc = Allocator()) {
        allocator = alloc;
        node_allocator = typename Allocator::template rebind<Node>::other();

        fake_node = AllocTraits::allocate(node_allocator, 1);

        node_connect(fake_node, fake_node, fake_node);

        Node *prev_node = fake_node;
        Node *cur_node = fake_node;

        for (size_t i = 0; i < count; ++i) {
            cur_node = AllocTraits::allocate(node_allocator, 1);
            AllocTraits::construct(node_allocator, cur_node, value);

            node_and_node_connect(prev_node, cur_node);
            prev_node = cur_node;

            ++sz;
        }
        node_and_node_connect(cur_node, fake_node);
    }

    List(const List& other) {
        allocator = std::allocator_traits<Allocator>::select_on_container_copy_construction(other.allocator);
        node_allocator = AllocTraits::select_on_container_copy_construction(other.node_allocator);

        fake_node = AllocTraits::allocate(node_allocator, 1);
        node_connect(fake_node, fake_node, fake_node);

        Node* elem = other.fake_node->next;
        Node* next_elem = elem->next;

        for (size_t i = 0; i < other.size(); ++i) {
            push_back(elem->value);
            elem = next_elem;
            next_elem = elem->next;
        }
    }

    ~List() {
        size_t all_sz = sz;
        for (size_t i = 0; i < all_sz; ++i) {
            pop_back();
        }

        AllocTraits::deallocate(node_allocator, fake_node, 1);
    }

    List& operator=(const List& other) {
        List copy = other;
        if (AllocTraits::propagate_on_container_copy_assignment::value && allocator != other.allocator) {
            allocator = other.allocator;
            node_allocator = other.node_allocator;
        }

        std::swap(copy.sz, sz);
        std::swap(copy.fake_node, fake_node);

        return *this;
    }

    Allocator& get_allocator() {
        return allocator;
    }

    template<bool IsConst>
    class common_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;
        using const_reference = const T&;
        using difference_type = std::ptrdiff_t;

        Node* ptr_node;

        common_iterator(Node* ptr_node): ptr_node(ptr_node) {}

        reference operator*() const {
            return ptr_node->value;
        }

        pointer operator->() const {
            return &(ptr_node->value);
        }

        common_iterator<IsConst>& operator++() {
            ptr_node = ptr_node->next;

            return *this;
        }

        common_iterator operator++(int) {
            common_iterator<IsConst> copy_iterator = *this;

            ptr_node = ptr_node->next;

            return copy_iterator;
        }

        common_iterator& operator--() {
            ptr_node = ptr_node->prev;

            return *this;
        }

        common_iterator operator--(int) {
            common_iterator<IsConst> copy_iterator = *this;

            ptr_node = ptr_node->prev;

            return copy_iterator;
        }

        bool operator==(const common_iterator& other) const {
            return ptr_node == other.ptr_node;
        }

        bool operator!=(const common_iterator& other) const {
            return ptr_node != other.ptr_node;
        }

        operator common_iterator<true>() {
            return common_iterator<true>(ptr_node);
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(fake_node->next);
    }

    iterator end() {
        return iterator(fake_node);
    }

    const_iterator begin() const{
        return const_iterator(fake_node->next);
    }

    const_iterator end() const{
        return const_iterator(fake_node);
    }

    const_iterator cbegin() const{
        return const_iterator(fake_node->next);
    }

    const_iterator cend() const{
        return const_iterator(fake_node);
    }

    reverse_iterator rbegin() {
        return std::make_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const{
        return std::make_reverse_iterator(cend());
    }

    const_reverse_iterator rend() const{
        return std::make_reverse_iterator(cbegin());
    }

    const_reverse_iterator crbegin() const{
        return std::make_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const{
        return std::make_reverse_iterator(cbegin());
    }

    void push_back(const T& value) {
        insert(fake_node, value);
    }

    void push_front(const T& value) {
        insert(fake_node->next, value);
    }

    void pop_back() {
        erase(fake_node->prev);
    }

    void pop_front() {
        erase(fake_node->next);
    }

    void insert(const_iterator it, const T& value) {
        Node* new_node = AllocTraits::allocate(node_allocator, 1);
        AllocTraits::construct(node_allocator, new_node, value);

        node_connect(new_node, it.ptr_node->prev, it.ptr_node);

        it.ptr_node->prev->next = new_node;
        it.ptr_node->prev = new_node;

        ++sz;
    }

    void erase(const_iterator it) {
        Node* old_node = it.ptr_node->next->prev;

        old_node->prev->next = old_node->next;
        old_node->next->prev = old_node->prev;

        --sz;

        AllocTraits::destroy(node_allocator, old_node);
        AllocTraits::deallocate(node_allocator, old_node, 1);
    }
};