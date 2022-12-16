#include "SharedMemory.h"
#include "SharedMemoryManager.h"


#pragma comment(lib, "winmm.lib")
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "LinkedMemory.h"


constexpr int kBlockCount = 10;
constexpr int kByteSize = 40;

class _4Byte {
    std::byte data[4];
};
class _8Byte {
    std::byte data[8];
};
class _10Byte {
    std::byte data[10];
};
class _20Byte {
    std::byte data[20];
};
class _40Byte {
    std::byte data[40];
};

int main(int, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    memory::LinkedStackMemory<kBlockCount, kByteSize > memory;

    std::pmr::vector<_4Byte > v4(&memory);
    std::pmr::vector<_8Byte > v8(&memory);
    std::pmr::vector<_10Byte > v10(&memory);
    std::pmr::vector<_20Byte > v20(&memory);
    std::pmr::vector<_40Byte > v40(&memory);



    // ブレークポイントをおいてみるとreserveしなかった場合のSTLが確保するメモリがよくわかる。

    // 4 byte
    {
        for(int i = 0; i < 10 - 1; i++) {
            v4.push_back({});
        } // for
    }
    // 8 byte
    {
        for(int i = 0; i < 5 - 1; i++) {
            v8.push_back({});
        } // for
    }
    // 10 byte
    {
        for(int i = 0; i < 4; i++) {
            v10.push_back({});
        } // for
    }
    // 20 byte
    {
        for(int i = 0; i < 2; i++) {
            v20.push_back({});
        } // for
    }
    // 40 byte
    {
        v40.push_back({});
    }

    _ASSERT_EXPR(!_CrtDumpMemoryLeaks(), L"memory leak");
    return 0;
}