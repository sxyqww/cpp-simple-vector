#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <utility>
template <typename Type>
class ArrayPtr {
public: // Constructors & assigment operatos
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size) : raw_ptr_(size > 0? new Type[size] : nullptr) {}
    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept : raw_ptr_(raw_ptr) {}

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(const ArrayPtr&) = delete;


    explicit ArrayPtr(ArrayPtr&& other) noexcept : 
    raw_ptr_(std::exchange(other.raw_ptr_, nullptr)) {}

    ArrayPtr& operator=(ArrayPtr&& other) noexcept {
        if (this != &other) {
            delete[] raw_ptr_;
            raw_ptr_ = std::exchange(other.raw_ptr_, nullptr);
        }
        return *this;
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }
public: // Methods
    [[nodiscard]] Type* Release() noexcept {
        Type* old_raw = raw_ptr_;
        raw_ptr_ = nullptr;
        return old_raw;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        assert(raw_ptr_ != nullptr);
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(raw_ptr_ != nullptr);
        return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return raw_ptr_ != nullptr;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};