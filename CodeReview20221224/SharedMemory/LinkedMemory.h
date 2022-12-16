#ifndef MEMORY_LINKED_STACK_MEMORY_H
#define MEMORY_LINKED_STACK_MEMORY_H


#include "LinkedMemoryInterface.h"

#include <memory_resource>
#include <array>

#include "LinkedMemoryBlock.h"


namespace memory {
template<size_t MemoryBlockCount, size_t MemoryBlockByteSize>
class LinkedStackMemory : public LinkedMemoryInterface {
    using MemoryBlock = memory::LinkedMemoryBlock<MemoryBlockByteSize>;
private:
    //! 確保したメモリ
    std::array<MemoryBlock, MemoryBlockCount> _memory_block;
    //! 次の生成ポインタ
    MemoryBlock* _free_pointer;
    //! 生成できなかった時の依頼先
    LinkedMemoryInterface* _next_stream;

    void Reset(void) {
        for(size_t i = 0; i < _memory_block.size() - 1; i++) {
            _memory_block[i].next = &_memory_block[i + 1];
        } // for
        _memory_block[_memory_block.size() - 1].next = nullptr;
    }
protected:
    virtual void* do_allocate(size_t bytes, size_t alignment) override {
        if(bytes > MemoryBlock::Size()) {
            return _next_stream->do_allocate(bytes, alignment);
        } // if

        if(_free_pointer == nullptr) {
            throw std::bad_alloc();
        } // if

        auto allocated = reinterpret_cast<void*> (_free_pointer);
        _free_pointer = _free_pointer->next;
        return allocated;
    }
    virtual void do_deallocate(void* used_pointer, size_t bytes, size_t alignment) override {
        if(bytes > MemoryBlock::Size()) {
            _next_stream->do_deallocate(used_pointer, bytes, alignment);
            return;
        } // if

        if(used_pointer == nullptr) {
            throw std::runtime_error("failed do_deallocate");
        } // if

        auto freed_pointer = reinterpret_cast<MemoryBlock*>(used_pointer);
        freed_pointer->Reset();

        freed_pointer->next = this->_free_pointer;
        this->_free_pointer = freed_pointer;
    }
    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }

    virtual size_t MemoryAlignment(void) const noexcept override {
        return MemoryBlock::Size();
    }
public:
    LinkedStackMemory() :
        _memory_block(),
        _free_pointer(_memory_block.data()),
        _next_stream(nullptr) {
        this->Reset();
    }
    LinkedStackMemory(LinkedMemoryInterface* next) :
        _memory_block(),
        _free_pointer(_memory_block.data()),
        _next_stream(next) {
        this->Reset();
    }
    ~LinkedStackMemory() {
    }
};
}
#endif // !MEMORY__LINKED_STACK_MEMORY_H