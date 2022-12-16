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
//    //! �ێ����Ă��郁����
//    std::byte _arena[ArenaSize];
//    //! ������|�C���^
//    std::byte* _position = _arena;
//    //! �m�ۂ����������̏��B�v�f����MakeShared���Ăяo��������������B
//    std::vector<MemoryInfo> _memory_info;
//    //! �R���p�N�V��������ۂ̑ޔ��(new���Ă�����)
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
//        //! �A���[�i��SharedMemory������MemoryBlock�����������B
//        //! MemoryBlock�̐擪��SharedMemory�����|�C���^�̌^���錾���ꂽ�߁A
//        //! SharedMemory�����|�C���^�̎Q�Ɛ�͊��蓖�Ă��ۂ̃A���[�i�̈ʒu�ɂȂ�B
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
//            // ���[�L���O�������֌��݊��蓖�ĂĂ��郁�����f�[�^�����ԂɃR�s�[����B
//            std::memcpy(working_current_position, memory_info.arena_position, size);
//
//            size_sum += size;
//            working_current_position += size;
//        } // for
//
//        // ���[�L���O�������ɃR�s�[�����A���[�i�֏㏑������B
//        std::memset(_arena, 0, ArenaSize);
//        std::memcpy(_arena, _working, size_sum);
//
//        _position = _arena;
//        for(auto& memory_info : _memory_info) {
//            //! �A���[�i�ɔz�u���ꂽSharedMemory������MemoryBlock�̔z�u�ʒu�ɁA
//            //! SharedMemory�����|�C���^�̎Q�Ɛ���X�V����B
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