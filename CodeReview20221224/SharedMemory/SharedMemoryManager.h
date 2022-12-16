//#ifndef MEMORY_SHARED_MEMORY_MANAGER_H
//#define MEMORY_SHARED_MEMORY_MANAGER_H
//
//
//#include <cstddef>
//#include <vector>
//
//#include "SharedMemory.h"
//
//
//namespace memory {
//template<size_t ArenaSize>
//class SharedMemoryManager : public std::pmr::memory_resource {
//    struct MemoryInfo {
//        void* arena_position;
//        size_t size;
//      
//        ISharedMemory* shared_memory;
//        IMemoryBlock* memory_block;
//        //int* i;
//        //void* p;
//        //SharedMemoryManager* m;
//    };
//private:
//    //! 保持しているメモリ
//    std::byte _arena[ArenaSize];
//    //! 生成先ポインタ
//    std::byte* _position = _arena;
//    //! 確保したメモリの情報。要素数はMakeSharedを呼び出した分だけある。
//    std::vector<MemoryInfo> _memory_info;
//    //! コンパクションする際の退避先(newしてもいい)
//    std::byte _working[ArenaSize];
//public:
//    SharedMemoryManager() :
//        _arena(),
//        _position(_arena),
//        _memory_info(),
//        _working() {
//    }
//public:
//    virtual void* do_allocate(size_t byte, size_t alignment) override {
//        auto ret = _position;
//
//        if(ret + byte - 1 >= _arena + ArenaSize) {
//            throw std::bad_alloc();
//        } // if
//
//        _position += byte;
//        return ret;
//    }
//    virtual void do_deallocate(void* used_pointer, size_t bytes, size_t alignment) override {
//        auto& v = _memory_info;
//        auto info = MemoryInfo();
//        info.size = bytes;
//        info.arena_position = used_pointer;
//
//        v.erase(std::remove_if(v.begin(), v.end(), [used_pointer](MemoryInfo& info) {
//            return info.arena_position == used_pointer;
//                }), v.end());
//
//        std::memset(used_pointer, 0, bytes);
//    }
//    virtual bool do_is_equal(const memory_resource& other) const noexcept override {
//        return this == &other;
//    }
//
//    //template<typename T, typename... Args>
//    template<typename T>
//    SharedMemory<T, SharedMemoryManager> MakeShared(void) {
//        //SharedMemory<T> ret = SharedMemory<T>(*this, args...);
//        memory::SharedMemory<T, SharedMemoryManager> ret = SharedMemory<T, SharedMemoryManager>(*this);
//
//        //! アリーナにSharedMemoryが持つMemoryBlockが生成される。
//        //! MemoryBlockの先頭でSharedMemoryが持つポインタの型が宣言されため、
//        //! SharedMemoryが持つポインタの参照先は割り当てた際のアリーナの位置になる。
//        auto info = MemoryInfo();
//        info.size = ret.MemorySize();
//        info.arena_position = ret.Get();
//        //info.i = ret.GetCount();
//        //info.m = ret.GetResource();
//        //info.p = ret.Get();
//        info.memory_block = ret.GetMemoryBlock();
//        info.shared_memory = &ret;
//
//        _memory_info.push_back(info);
//        return ret;
//    }
//
//    void MemoryCompaction(void) {
//        size_t size_sum = 0;
//
//        auto working_current_position = _working;
//
//
//        for(auto& memory_info : _memory_info) {
//            size_t size = memory_info.size;
//
//            // ワーキングメモリへ現在割り当てているメモリデータを順番にコピーする。
//            std::memcpy(working_current_position, memory_info.arena_position, size);
//
//            size_sum += size;
//            working_current_position += size;
//        } // for
//
//        // ワーキングメモリにコピーしたアリーナへ上書きする。
//        std::memset(_arena, 0, ArenaSize);
//        std::memcpy(_arena, _working, size_sum);
//
//        _position = _arena;
//        for(auto& memory_info : _memory_info) {
//            //! アリーナに配置されたSharedMemoryが持つMemoryBlockの配置位置に、
//            //! SharedMemoryが持つポインタの参照先を更新する。
//            memory_info.arena_position = _position;
//
//            memory_info.shared_memory->SetMemoryBlock(memory_info.memory_block);
//
//
//            size_t size = memory_info.size;
//            _position += size;
//        } // for
//
//
//        std::memset(_working, 0, ArenaSize);
//    }
//};
//}
//#endif // !MEMORY_SHARED_MEMORY_MANAGER_H