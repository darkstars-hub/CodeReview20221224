#ifndef MEMORY_LINKED_MEMORY_INTERFACE_H
#define MEMORY_LINKED_MEMORY_INTERFACE_H


#include <memory_resource>


namespace memory {
class LinkedMemoryInterface : public std::pmr::memory_resource {
protected:
public:
    virtual void* do_allocate(size_t bytes, [[maybe_unused]] size_t alignment) override {
        return nullptr;
    }
    virtual void do_deallocate(void* used_pointer, size_t bytes, [[maybe_unused]] size_t alignment) override {
    }
    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }
    virtual size_t MemoryAlignment(void) const noexcept = 0;
public:
    LinkedMemoryInterface() {
    }
    ~LinkedMemoryInterface() {
    }

    template<typename Type>
    auto Construct(void) {
        //auto place = this->do_allocate(sizeof(Type), MemoryBlock::Size());
        auto place = this->do_allocate(sizeof(Type), this->MemoryAlignment());
        return new (place) Type;
    }
    template<typename Type, typename... Args>
    auto Construct(Args&&... args) {
        auto place = this->do_allocate(sizeof(Type), this->MemoryAlignment());
        return new (place) Type(args...);
    }

    template<typename Type>
    void Destruct(Type*& instance_pointer) {
        instance_pointer->~Type();
        this->deallocate(instance_pointer, sizeof(Type));
        instance_pointer = nullptr;
    }
};
}
#endif // !MEMORY_LINKED_MEMORY_INTERFACE_H