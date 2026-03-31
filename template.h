// Memory manager implementation for Problem 069
#pragma once

#include <cstddef>
#include <cstdint>

namespace sjtu {

constexpr size_t MEMORY_SIZE = 1024 * 1024; // 1 MB

struct MemoryBlock{
    int address;
    int size;
    bool is_free;
    MemoryBlock* next = nullptr;
};

inline MemoryBlock* head = nullptr; // 内存块链表头指针

inline MemoryBlock* splitBlock(MemoryBlock* block, size_t size) {
    if (block->size < static_cast<int>(size + sizeof(MemoryBlock))) {
        return nullptr; // 无法拆分
    }
    MemoryBlock* new_block = new MemoryBlock;
    new_block->address = block->address + static_cast<int>(sizeof(MemoryBlock)) + static_cast<int>(size);
    new_block->size = block->size - static_cast<int>(size) - static_cast<int>(sizeof(MemoryBlock));
    new_block->is_free = true;
    new_block->next = block->next;
    block->size = static_cast<int>(size);
    block->next = new_block;
    return new_block;
}

inline void mergeBlock(MemoryBlock* block){
    MemoryBlock* next = block->next;
    if (next == nullptr) {
        return;
    }
    block->size += static_cast<int>(sizeof(MemoryBlock)) + next->size;
    block->next = next->next;
    delete next;
}

inline MemoryBlock* allocate(int size){
    if (size <= 0) return nullptr;
    MemoryBlock* cur = head;
    while (cur) {
        if (cur->is_free && cur->size >= size) {
            // Try to split if there is enough space for a new header + remaining data
            // If split is not possible or not meaningful, allocate the whole block
            // Use helper function as required
            if (cur->size > size) {
                MemoryBlock* res = splitBlock(cur, static_cast<size_t>(size));
                // If split failed (remaining too small), just use whole block
                (void)res; // res may be nullptr; either way we allocate cur
            }
            cur->is_free = false;
            return cur;
        }
        cur = cur->next;
    }
    return nullptr; // no suitable block
}

inline void deallocate(MemoryBlock* ptr){
    if (ptr == nullptr) return;
    ptr->is_free = true;
    // Only merge with next if it is free (backward merge not required)
    if (ptr->next && ptr->next->is_free) {
        mergeBlock(ptr);
    }
}

inline void resetMemory(){
    MemoryBlock* temp = head;
    while (temp) {
        MemoryBlock* next = temp->next;
        delete temp;
        temp = next;
    }
    head = new MemoryBlock;
    head->address = 0;
    head->size = static_cast<int>(MEMORY_SIZE - sizeof(MemoryBlock));
    head->is_free = true;
    head->next = nullptr;
}

} // namespace sjtu

