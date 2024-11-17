#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include "array_ptr.h"

struct ReserveProxyObject {
    size_t capacity_to_reserve;
    explicit ReserveProxyObject(size_t capacity) : capacity_to_reserve(capacity) {}
};

ReserveProxyObject Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObject(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public: //  Types
    using Iterator = Type*;
    using ConstIterator = const Type*;

public: // Constructor & assigment operator
    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : 
    items_(size), size_(size),capacity_(size) {
        std::fill(begin(), end(), Type{});
    }

    SimpleVector(size_t size, const Type& value): 
    items_(size), size_(size),capacity_(size) {
        std::fill(begin(), end(), value);
    }

    SimpleVector(std::initializer_list<Type> init) : 
    items_(init.size()), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), begin());
    }   

    SimpleVector(const SimpleVector& other) {
        SimpleVector<Type> tmp(other.size_);
        std::copy(other.begin(), other.end(), tmp.begin());
        swap(tmp);
    }

    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            SimpleVector tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    SimpleVector(SimpleVector&& other) noexcept
    {
        MoveFrom(std::move(other));
    }

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            MoveFrom(std::move(other));
        }
        return *this;
    }

    explicit SimpleVector(ReserveProxyObject wrapper) {
        capacity_ = wrapper.capacity_to_reserve;
        Reserve(capacity_);

    }

public: // Methods
    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return items_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return items_[index];
        }

    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index is out of range");
        }
        return items_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index is out of range");
        }   
        return items_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            ReallocateAndMoveData(new_size);
    }

        if (new_size > size_) {
            std::generate(begin() + size_, begin() + new_size,
                []() {return Type{};});
        }
        size_ = new_size;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ReallocateAndMoveData(new_capacity);
        }
    }
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void PopBack() noexcept {
        assert(size_ > 0);
        --size_;    
    }

    Iterator Erase (ConstIterator pos) {
        assert(pos >= begin() && pos < end());
        Iterator non_const_pos = const_cast<Iterator>(pos);
        std::move(non_const_pos + 1, end(), non_const_pos);
        --size_;
        return non_const_pos;

    }

    void PushBack(const Type& value) {
        size_t new_size = size_ + 1u;
        if (new_size > capacity_) {
            size_t new_capacity = (capacity_ != 0) ? std::max(new_size, 2u * capacity_) : 1;
            ReallocateAndMoveData(new_capacity);
        }
        items_[size_] = value;
        size_ = new_size;
    }

    void PushBack(Type&& value) {
        size_t new_size = size_ + 1u;
        if (new_size > capacity_) {
            size_t new_capacity = (capacity_ != 0) ? std::max(new_size, 2u * capacity_) : 1;
            ReallocateAndMoveData(new_capacity);
        }
        items_[size_] = std::move(value);
        size_ = new_size;
    }
    
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        size_t new_size = size_ + 1;
        size_t position_offset = static_cast<size_t>(pos - begin());
        Iterator element_position = items_.Get() + position_offset;   // Получаем индекс для вставки

        if (pos == end()) {
            PushBack(value);
            return end() - 1;
        }   

        if (size_ < capacity_) {
            std::move_backward(element_position, end(), end() + 1u);
            items_[position_offset] = value;
        }   
        else {
            size_t new_capacity = (capacity_ != 0) ? std::max(new_size, 2u * capacity_) : 1;
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), begin() + position_offset, new_items.Get());
            new_items[position_offset] = value;
            std::move(element_position, end(), new_items.Get() + position_offset + 1u);
            
            items_.swap(new_items);
    
            capacity_ = new_capacity;    
        }
        size_ = new_size;
        return element_position;
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        size_t new_size = size_ + 1;
        size_t position_offset = static_cast<size_t>(pos - begin());
        Iterator element_position = items_.Get() + position_offset;   // Получаем индекс для вставки

        if (pos == end()) {
            PushBack(std::move(value));
            return end() - 1;
        }

        if (size_ < capacity_) {
            std::move_backward(element_position, end(), end() + 1u);
            items_[position_offset] = std::move(value);
        }   
        else {
            size_t new_capacity = (capacity_ != 0) ? std::max(new_size, 2u * capacity_) : 1;
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), begin() + position_offset, new_items.Get());
            new_items[position_offset] = std::move(value);
            std::move(element_position, end(), new_items.Get() + position_offset + 1u);

            items_.swap(new_items);

            capacity_ = new_capacity;    
        }
        size_ = new_size;
        return element_position;
    }

    Iterator begin() noexcept {
        return items_.Get();
        }

    Iterator end() noexcept {
        return items_.Get() + size_;
    }   

    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    ConstIterator cbegin() const noexcept {
        return items_.Get();
        }

    ConstIterator cend() const noexcept {
        return items_.Get() + size_;
        }
private:
    void ReallocateAndMoveData(size_t new_capacity) {
        ArrayPtr<Type> new_items(new_capacity);
        size_t count_elements = std::min(size_, new_capacity);
        std::move(items_.Get(), items_.Get() + count_elements, new_items.Get());
        items_.swap(new_items);

        capacity_ = new_capacity;
        
    }

    void MoveFrom(SimpleVector&& other) noexcept {
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
        items_ = std::move(other.items_);

    }
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

    template <typename Type>
    void swap(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        lhs.swap(rhs);
    }

    template <typename Type>
    bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename Type>
    bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return rhs < lhs;
    }

    template <typename Type>
    bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return !(lhs > rhs);
    }

    template <typename Type>
    bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return !(lhs < rhs);
    }

    template <typename Type>
    bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return !((lhs < rhs) || (lhs > rhs));
    }

    template <typename Type>
    bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
        return !(lhs == rhs);
    }