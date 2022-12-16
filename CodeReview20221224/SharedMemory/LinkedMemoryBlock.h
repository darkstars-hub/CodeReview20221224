#ifndef MEMORY_MEMORY_DEFINE_H
#define MEMORY_MEMORY_DEFINE_H


namespace memory {
template<size_t ByteSizeMax>
class LinkedMemoryBlock {
public:
    std::byte memory[ByteSizeMax];
    LinkedMemoryBlock* next = nullptr;
    void Reset(void) {
        memset(memory, 0, sizeof(std::byte) * ByteSizeMax);
    }
    static size_t Size(void) {
        return ByteSizeMax;
    }
};
}
#endif // !MEMORY_MEMORY_DEFINE_H