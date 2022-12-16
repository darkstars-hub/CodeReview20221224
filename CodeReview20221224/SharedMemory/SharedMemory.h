//#ifndef MEMORY_SHARED_MEMORY_H
//#define MEMORY_SHARED_MEMORY_H
//
//
//#include <utility>
//#include <memory_resource>
//
//
//namespace memory {
//template <class MemoryResource>
//concept MemoryResourceConcept = requires (MemoryResource memory_resouce) {
//    std::is_base_of<std::pmr::memory_resource, MemoryResource>::value;
//};
//
//class IMemoryBlock {
//};
//class ISharedMemory {
//public:
//    virtual void Set(void* ptr) noexcept = 0;
//    virtual void SetResource(std::pmr::memory_resource* ptr) noexcept = 0;
//    virtual void SetCount(int* ptr) noexcept = 0;
//    virtual void SetMemoryBlock(IMemoryBlock* ptr) noexcept = 0;
//
//    virtual void* Get(void) noexcept = 0;
//    virtual std::pmr::memory_resource* GetResource(void) noexcept = 0;
//    virtual int* GetCount(void) noexcept = 0;
//    virtual IMemoryBlock* GetMemoryBlock(void) = 0;
//
//    virtual size_t MemorySize(void) const noexcept = 0;
//};
//template<typename T, typename MemoryResource = std::pmr::memory_resource>
//    requires MemoryResourceConcept<MemoryResource>
//class SharedMemory : public ISharedMemory {
//private:
//    using pointer = T*;
//    friend class SharedMemory<T, MemoryResource>;
//public:
//    template<typename T>
//    class MemoryBlock : public IMemoryBlock {
//    public:
//        T instance;
//        int use_count = 1;
//    };
//private:
//    //! 実ポインタ
//    T* _ptr = nullptr;
//    //! 共有する参照カウント
//    mutable int* _use_count = nullptr;
//    //! リソース
//    MemoryResource* _memory_resource;
//
//    IMemoryBlock* _memory_block;
//private:
//    template<typename T>
//    size_t MemoryAlignment(void) const noexcept {
//        size_t size = 1;
//        while(true) {
//            size = size << 1;
//            if(size > sizeof(T)) {
//                break;
//            } // if
//        } // while
//        return size;
//    }
//    template<class U>
//    void CopyConstructFrom(const SharedMemory<U, MemoryResource>& other) noexcept {
//        other.IncrementReference();
//
//        this->_ptr = other._ptr;
//        this->_use_count = other._use_count;
//        this->_memory_resource = other._memory_resource;
//    }
//    void IncrementReference(void) const noexcept {
//        (*_use_count)++;
//    }
//    void DecrementReference(void) const noexcept {
//        (*_use_count)--;
//    }
//    void SwapImpl(SharedMemory& right) noexcept {
//        std::swap(this->_ptr, right._ptr);
//        std::swap(this->_use_count, right._use_count);
//    }
//public:
//    void Update() {
//        //_ptr = this;
//    }
//    SharedMemory() {
//        _ptr = new T();
//        _use_count = new int(1);
//        _memory_resource = nullptr;
//    }
//    SharedMemory(MemoryResource& allocator) {
//        _memory_resource = &allocator;
//
//        size_t size = this->MemorySize();
//        auto place = _memory_resource->allocate(size, this->MemoryAlignment<MemoryBlock<T>>());
//        _ptr = new (place) T;
//        _use_count = &reinterpret_cast<MemoryBlock<T>*>(place)->use_count;
//        *_use_count = 1;
//
//        _memory_block = reinterpret_cast<IMemoryBlock*>(place);
//    }
//
//    void SetPtrFromBlock(void* p) {
//        auto block = reinterpret_cast<MemoryBlock<T>>(p);
//        _ptr = &block->instance;
//        _use_count = &block->use_count;
//    }
//    ~SharedMemory() {
//        this->DecrementReference();
//
//        // ほかのSharedMemoryも参照しなくなったらすべて削除する
//        if(*_use_count == 0) {
//
//            if(_memory_resource != nullptr) {
//                _ptr->~T();
//
//                using Block = MemoryBlock<T>;
//                _memory_resource->deallocate(
//                    _ptr,
//                    sizeof(Block),
//                    this->MemoryAlignment<Block>()
//                );
//            } // if
//            else {
//                delete _ptr;
//                delete _use_count;
//            } // else
//        } // if
//
//
//        _ptr = nullptr;
//        _use_count = nullptr;
//    }
//    SharedMemory(const SharedMemory& other) noexcept {
//        // 他と同じリソースを所有するオブジェクトを構築する
//        this->CopyConstructFrom(other);
//    }
//    [[nodiscard]]
//    pointer operator->(void) const noexcept {
//        return this->_ptr;
//    }
//    [[nodiscard]]
//    explicit operator bool(void) const noexcept {
//        return static_cast<bool>(this->_ptr);
//    }
//    virtual void Set(void* ptr) noexcept override {
//        this->_ptr = static_cast<T*>(ptr);
//    }
//
//    virtual void SetResource(std::pmr::memory_resource* ptr) noexcept override {
//        this->_memory_resource = static_cast<MemoryResource*> (ptr);
//    }
//    virtual void SetCount(int* ptr) noexcept override {
//        this->_use_count = (ptr);
//    }
//    virtual void SetMemoryBlock(IMemoryBlock* ptr) noexcept override {
//        this->_memory_block = (ptr);
//    }
//
//    virtual void* Get(void) noexcept override {
//        return reinterpret_cast<void*> (this->_ptr);
//    }
//    virtual std::pmr::memory_resource* GetResource(void) noexcept override {
//        return this->_memory_resource;
//    }
//    virtual int* GetCount(void) noexcept override {
//        return this->_use_count;
//    }
//    virtual IMemoryBlock* GetMemoryBlock(void) override {
//        return this->_memory_block;
//    }
//
//
//
//    virtual size_t MemorySize(void) const noexcept override {
//        return sizeof(MemoryBlock<T>);
//    }
//    [[nodiscard]]
//    pointer Get(void) const noexcept {
//        return this->_ptr;
//    }
//    void Swap(SharedMemory& other) noexcept {
//        this->SwapImpl(other);
//    }
//};
//}
//#endif // !MEMORY_SHARED_MEMORY_MANAGER_H