#include <iostream>
#include "template.h"

using namespace sjtu;

static bool testNormalAlloc(){
    resetMemory();
    MemoryBlock* block1 = allocate(256);
    if (!block1 || block1->size < 256 || !block1->is_free == false) return false;
    return true;
}

static bool testSmallAlloc(){
    resetMemory();
    MemoryBlock* block1 = allocate(1);
    if (!block1 || block1->size < 1 || !block1->is_free == false) return false;
    return true;
}

static bool testBigAlloc(){
    resetMemory();
    const int MAX_SIZE = static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock));
    MemoryBlock* block1 = allocate(MAX_SIZE);
    if (!block1 || block1->size < MAX_SIZE || !block1->is_free == false) return false;
    return true;
}

static bool testOverAlloc(){
    resetMemory();
    const int MAX_SIZE = static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock));
    MemoryBlock* block1 = allocate(MAX_SIZE + 1);
    if (block1 != nullptr) return false;
    return true;
}

static bool testTwoAlloc(){
    resetMemory();
    MemoryBlock* block1 = allocate(256);
    if(!block1 || block1->size < 256 || !block1->is_free == false) return false;

    MemoryBlock* block2 = allocate(128);
    if(!block2 || block2->size < 128 || !block2->is_free == false) return false;

    if(block1->next != block2) return false;

    return true;
}

static bool testMultiAlloc(){
    resetMemory();
    const int ALLOC_SIZES[] = {64, 128, 256, 512, 1024, 2048, 4096};

    MemoryBlock* prev_block = nullptr;
    for(auto size: ALLOC_SIZES){
        MemoryBlock* block = allocate(size);
        if(!block || block->size < size || !block->is_free == false) return false;
        if(prev_block && prev_block->next != block) return false;
        prev_block = block;
    }

    return true;
}

static bool testMultiAllocUntilFull(){
    resetMemory();
    const int ALLOC_SIZE = 1024;
    const int MAX_BLOCKS = static_cast<int>(MEMORY_SIZE / (ALLOC_SIZE + sizeof(MemoryBlock)));

    MemoryBlock* prev_block = nullptr;
    for(int i = 0; i < MAX_BLOCKS; ++i){
        MemoryBlock* block = allocate(ALLOC_SIZE);
        if(!block || block->size < ALLOC_SIZE || !block->is_free == false) return false;
        if(prev_block && prev_block->next != block) return false;
        prev_block = block;
    }

    MemoryBlock* block = allocate(ALLOC_SIZE);
    if(block != nullptr) return false;

    return true;
}

static bool testAllocAndDealloc(){
    resetMemory();
    MemoryBlock* block1 = allocate(256);
    if(!block1 || block1->size < 256 || !block1->is_free == false) return false;
    
    deallocate(block1);
    if(block1->is_free == false) return false;

    MemoryBlock* block2 = allocate(128);
    if(!block2 || block2->size < 128 || !block2->is_free == false) return false;

    if(block1 != block2) return false;
    return true;
}

static bool testMultiAllocAndDealloc(){
    resetMemory();
    MemoryBlock* blocks[5];
    for(int i = 0; i < 5; ++i){
        blocks[i] = allocate(256);
        if(!blocks[i] || blocks[i]->size < 256 || !blocks[i]->is_free == false) return false;
    }
    deallocate(blocks[2]);
    if(blocks[2]->is_free == false) return false;
    MemoryBlock* block_small = allocate(128);
    if(!block_small || block_small->size < 128 || !block_small->is_free == false) return false;
    if(block_small != blocks[2]) return false;
    return true;
}

static bool testAllAllocAndDealloc(){
    resetMemory();
    const int ALLOC_SIZE = 1024;
    const int MAX_BLOCKS = static_cast<int>(MEMORY_SIZE / (ALLOC_SIZE + sizeof(MemoryBlock)));

    MemoryBlock* blocks[MAX_BLOCKS];
    for(int i = 0; i < MAX_BLOCKS; ++i){
        blocks[i] = allocate(ALLOC_SIZE);
        if(!blocks[i] || blocks[i]->size < ALLOC_SIZE || !blocks[i]->is_free == false) return false;
    }

    for(int i = MAX_BLOCKS-1; i >= 0; --i){
        deallocate(blocks[i]);
        if(blocks[i]->is_free == false) return false;
    }

    MemoryBlock* big_block = allocate(static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock) - 1));
    if(!big_block || big_block->size < static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock) - 1) || !big_block->is_free == false) return false;
    return true;
}

int main(){
    std::cout << "Test Normal Alloc: " << (testNormalAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Small Alloc: " << (testSmallAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Big Alloc: " << (testBigAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Over Alloc: " << (testOverAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Two Alloc: " << (testTwoAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Multi Alloc: " << (testMultiAlloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Multi Alloc Until Full: " << (testMultiAllocUntilFull() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Alloc And Dealloc: " << (testAllocAndDealloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test Multi Alloc And Dealloc: " << (testMultiAllocAndDealloc() ? "Passed" : "Failed") << '\n';
    std::cout << "Test All Alloc And Dealloc: " << (testAllAllocAndDealloc() ? "Passed" : "Failed") << '\n';
    return 0;
}

