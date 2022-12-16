#ifndef MEMORY_MONO_STACK_MEMORY_H
#define MEMORY_MONO_STACK_MEMORY_H


#include <memory_resource>
#include <array>

#include "LinkedMemoryBlock.h"


namespace memory {
//! 生存期間同じでバイト数が近いインスタンスの生成先。
//! スタック領域に確保されるバイト数 = (MemoryBlockByteSize * MemoryBlockCount) + std::pmr::memory_resource + MonoStackMemory::_free_pointer
template<size_t MemoryBlockCount, size_t MemoryBlockByteSize>
class MonoStackMemory : public std::pmr::memory_resource {
    using MemoryBlock = memory::LinkedMemoryBlock<MemoryBlockByteSize>;
private:
    //! 
    std::array<MemoryBlock, MemoryBlockCount> _memory_block;
    MemoryBlock* _free_pointer;
public:
    virtual void* do_allocate(size_t byte, size_t alignment) override {
        if(byte > MemoryBlock::Size()) {
            throw std::bad_alloc();
        } // if
        if(_free_pointer == nullptr) {
            throw std::bad_alloc();
        } // if

        auto allocated = reinterpret_cast<void*> (_free_pointer);
        _free_pointer = _free_pointer->next;
        return allocated;
    }
    virtual void do_deallocate(void* used_pointer, size_t bytes, size_t alignment) override {
        if(used_pointer == nullptr) {
            throw std::runtime_error("do_deallocate");
        } // if

        auto freed_pointer = reinterpret_cast<MemoryBlock*>(used_pointer);
        freed_pointer->next = this->_free_pointer;
        this->_free_pointer = freed_pointer;
    }
    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }
public:
    MonoStackMemory() :
        _memory_block(),
        _free_pointer(_memory_block.data()) {
        this->Reset();
    }
    ~MonoStackMemory() {
    }
    void Reset(void) {
        for(size_t i = 0; i < _memory_block.size() - 1; i++) {
            _memory_block[i].next = &_memory_block[i + 1];
        } // for
        _memory_block[_memory_block.size() - 1].next = nullptr;
    }

    template<typename Type>
    auto Construct(void) {
        auto place = this->do_allocate(sizeof(Type), MemoryBlock::Size());
        return new (place) Type;
    }
    template<typename Type, typename... Args>
    auto Construct(Args... args) {
        auto place = this->do_allocate(sizeof(Type), MemoryBlock::Size());
        return new (place) Type(args...);
    }

    template<typename Type>
    void Destruct(Type* instance_pointer) {
        instance_pointer->~Type();
        this->deallocate(instance_pointer, sizeof(Type));
    }
};
}
#endif // !MEMORY_MONO_STACK_MEMORY_H