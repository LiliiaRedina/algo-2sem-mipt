#include <iostream>
#include <vector>

template<typename T>
class Deque{
private:
    std::vector<T*> buffer;
    size_t left = 0;
    size_t right = 0;

    void swap(Deque& other) {
        std::swap(buffer, other.buffer);
        std::swap(left, other.left);
        std::swap(right, other.right);
    }

    void reallocate() {
        std::vector<T*> new_buffer;

        for (size_t i = 0; i < size() / block_size + 1; ++i) {
            new_buffer.push_back(reinterpret_cast<T*>(new int8_t[block_size * sizeof(T)]));
        }

        for (size_t i = 0; i < buffer.size(); ++i) {
            new_buffer.push_back(buffer[i]);
        }

        for (size_t i = 0; i < size() / block_size + 1; ++i) {
            new_buffer.push_back(reinterpret_cast<T*>(new int8_t[block_size * sizeof(T)]));
        }

        std::swap(buffer, new_buffer);

        size_t old_left = left;

        left = ((right - left) / block_size + 1) * block_size + left;
        right = ((right - old_left) / block_size + 1) * block_size + right;
    }

public:
    static const size_t block_size = 8;

    Deque() {
        buffer.push_back(reinterpret_cast<T*>(new int8_t[block_size * sizeof(T)]));

        left = 0;
        right = 0;
    }

    Deque(size_t size, const T& value = T()) {
        for (size_t i = 0; i < size / block_size + 1; ++i) {
            buffer.push_back(reinterpret_cast<T*>(new int8_t[block_size * sizeof(T)]));
        }

        for (size_t i = 0; i < size / block_size + 1; ++i) {
            for (size_t j = 0; j < block_size; ++j) {
                try {
                    if (i * block_size + j < size) {
                        new(buffer[i] + j) T(value);
                    }
                } catch (...) {
                    for (size_t alr = 0; alr < j; ++alr) {
                        if (i * block_size + alr < size) {
                            buffer[i][alr].~T();
                        }
                    }

                    for (size_t alr_buffer = 0; alr_buffer < i; ++alr_buffer) {
                        for (size_t alr_block = 0; alr_block < block_size; ++alr_block) {
                            buffer[alr_buffer][alr_block].~T();
                        }
                    }

                    for (size_t alr = 0; alr < size / block_size + 1; ++alr) {
                        delete[] reinterpret_cast<int8_t*>(buffer[alr]);
                    }

                    throw;
                }
            }
        }

        left = 0;
        right = size;
    }

    Deque(const Deque& other) {
        for (size_t i = 0; i < other.buffer.size(); ++i) {
            buffer.push_back(reinterpret_cast<T*>(new int8_t[block_size * sizeof(T)]));
        }

        for (size_t i = 0; i < other.buffer.size(); ++i) {
            for (size_t j = 0; j < block_size; ++j) {
                try {
                    if (i * block_size + j >= other.left && i * block_size + j < other.right) {
                        new(buffer[i] + j) T(other.buffer[i][j]);
                    }
                } catch (...) {
                    for (size_t alr = 0; alr < j; ++alr) {
                        if (i * block_size + alr >= other.left && i * block_size + alr < other.right) {
                            buffer[i][alr].~T();
                        }
                    }

                    for (size_t alr_buffer = 0; alr_buffer < i; ++alr_buffer) {
                        for (size_t alr_block = 0; alr_block < block_size; ++alr_block) {
                            if (alr_buffer * block_size + alr_block >= other.left &&
                                alr_buffer * block_size + alr_block < other.right) {
                                buffer[alr_buffer][alr_block].~T();
                            }
                        }
                    }

                    for (size_t alr = 0; alr < other.buffer.size(); ++alr) {
                        delete[] reinterpret_cast<int8_t*>(buffer[alr]);
                    }

                    throw;
                }
            }
        }

        left = other.left;
        right = other.right;
    }

    ~Deque() {
        for (size_t full_buffer = 0; full_buffer < buffer.size(); ++full_buffer) {
            for (size_t full_block = 0; full_block < block_size; ++full_block) {
                if (full_buffer * block_size + full_block >= left &&
                    full_buffer * block_size + full_block < right) {
                    buffer[full_buffer][full_block].~T();
                }
            }
        }

        for (size_t i = 0; i < buffer.size(); ++i) {
            delete[] reinterpret_cast<int8_t*>(buffer[i]);
        }
    }

    size_t size() const {
        return right - left;
    }

    Deque& operator=(const Deque& other) {
        Deque copy = other;
        swap(copy);

        return *this;
    }

    T& operator[](size_t position) {
        return buffer[(left + position) / block_size][(left + position) % block_size];
    }

    const T& operator[](size_t position) const {
        return buffer[(left + position) / block_size][(left + position) % block_size];
    }

    T& at(size_t position) {
        if (position < right - left) {
            return (*this)[position];
        } else {
            throw(std::out_of_range("out_of_range"));
        }
    }

    const T& at(size_t position) const {
        if (position < right - left) {
            return (*this)[position];
        } else {
            throw(std::out_of_range("out_of_range"));
        }
    }

    void push_back(const T& value) {
        if (right >= buffer.size() * block_size - 1) {
            reallocate();
        }

        new(buffer[right / block_size] + right % block_size) T(value);

        ++right;
    }

    void pop_back() {
        if (left < right) {
            --right;

            buffer[right / block_size][right % block_size].~T();
        }
    }

    void push_front(const T& value) {
        if (left == 0) {
            reallocate();
        }

        new(buffer[(left - 1) / block_size] + (left - 1) % block_size) T(value);

        --left;
    }

    void pop_front() {
        if (left < right) {
            buffer[left / block_size][left % block_size].~T();

            ++left;
        }
    }

    template<bool IsConst>
    class common_iterator {
    private:
        const std::vector<T*>* ptr_buffer;
        std::conditional_t<IsConst, const T*, T*> cell_in_block;
        size_t num_in_buffer = 0;
        size_t num_in_block = 0;

        int total_num() const {
            return num_in_buffer * block_size + num_in_block;
        }

    public:
        common_iterator<IsConst>(const std::vector<T*>* ptr_buffer,
                                 std::conditional_t<IsConst, const T*, T*> cell_in_block,
                                 size_t num_in_buffer, size_t num_in_block):
                ptr_buffer(ptr_buffer), cell_in_block(cell_in_block),
                num_in_buffer(num_in_buffer), num_in_block(num_in_block) {}

        std::conditional_t<IsConst, const T&, T&> operator*() {
            return *cell_in_block;
        }

        std::conditional_t<IsConst, const T*, T*> operator->() {
            return cell_in_block;
        }

        common_iterator<IsConst>& change_iterator(int delta) {
            num_in_buffer = (total_num() + delta) / block_size;
            num_in_block = (total_num() + delta) % block_size;
            cell_in_block = (*ptr_buffer)[num_in_buffer] + num_in_block;

            return *this;
        }

        common_iterator<IsConst>& operator++() {
            return change_iterator(1);
        }

        common_iterator<IsConst> operator++(int) {
            common_iterator<IsConst> copy_iterator = *this;

            change_iterator(1);

            return copy_iterator;
        }

        common_iterator<IsConst>& operator--() {
            return change_iterator(-1);
        }

        common_iterator<IsConst> operator--(int) {
            common_iterator<IsConst> copy_iterator = *this;

            change_iterator(-1);

            return copy_iterator;
        }

        common_iterator<IsConst>& operator+=(const int delta) {
            return change_iterator(delta);
        }

        common_iterator<IsConst>& operator-=(const int delta) {
            return change_iterator(-delta);
        }

        common_iterator<IsConst> operator+(const int delta) const {
            common_iterator<IsConst> copy_iterator = *this;
            copy_iterator += delta;

            return copy_iterator;
        }

        common_iterator<IsConst> operator-(const int delta) const {
            common_iterator<IsConst> copy_iterator = *this;
            copy_iterator -= delta;

            return copy_iterator;
        }

        int operator-(common_iterator<IsConst> other_iterator) const {
            return total_num() - other_iterator.total_num();
        }

        bool operator<(common_iterator<IsConst> other_iterator) const {
            return (total_num() < other_iterator.total_num());
        }

        bool operator>(common_iterator<IsConst> other_iterator) const {
            return (total_num() > other_iterator.total_num());
        }

        bool operator<=(common_iterator<IsConst> other_iterator) const {
            return (total_num() <= other_iterator.total_num());
        }

        bool operator>=(common_iterator<IsConst> other_iterator) const {
            return (total_num() >= other_iterator.total_num());
        }

        bool operator==(common_iterator<IsConst> other_iterator) const {
            return (total_num() == other_iterator.total_num());
        }
        bool operator!=(common_iterator<IsConst> other_iterator) const {
            return (total_num() != other_iterator.total_num());
        }

        operator common_iterator<true>() const {
            return common_iterator<true>(ptr_buffer, cell_in_block, num_in_buffer, num_in_block);
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(&buffer, buffer[left / block_size] + left % block_size,
                        left / block_size, left % block_size);
    }

    iterator end() {
        return iterator(&buffer, buffer[right / block_size] + right % block_size,
                        right / block_size, right % block_size);
    }

    const_iterator begin() const{
        return const_iterator(&buffer, buffer[left / block_size] + left % block_size,
                              left / block_size, left % block_size);
    }

    const_iterator end() const{
        return const_iterator(&buffer, buffer[right / block_size] + right % block_size,
                              right / block_size, right % block_size);
    }

    const_iterator cbegin() const{
        return const_iterator(&buffer, buffer[left / block_size] + left % block_size,
                              left / block_size, left % block_size);
    }

    const_iterator cend() const{
        return const_iterator(&buffer, buffer[right / block_size] + right % block_size,
                              right / block_size, right % block_size);
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

    void insert(const_iterator iter, const T& value) {
        push_back(value);

        size_t index = iter - begin();

        for (size_t i = size() - 1; i > index; --i) {
            std::swap((*this)[i], (*this)[i - 1]);
        }
    }

    void erase(const_iterator iter) {
        size_t index = iter - begin();
        for (size_t i = index; i < size() - 1; ++i) {
            std::swap((*this)[i], (*this)[i + 1]);
        }

        pop_back();
    }
};
