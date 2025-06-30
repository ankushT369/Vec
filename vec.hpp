#ifndef __VEC__
#define __VEC__

#include <cstdint>
#include <cstddef>
#include <array>
#include <iostream>

#include <sys/mman.h>

#define PAGE_LIST_SIZE (2 << 13)
#define PAGE_LEVEL_SIZE 13
#define PAGE_SIZE 4096

#define INIT_LEVEL_INDEX 0

enum class page_status{
        DIRTY,
        CLEAN,
};


//-------------------- Unit page-----------------------
class Page_t {
public:
        bool isFull;
        void* page;
        uint32_t offset;
};

/* Level allocation */
inline Page_t* allocate_level(size_t level_idx) {
        size_t npages = (1 << level_idx);
        void *page = mmap(NULL, npages * PAGE_SIZE,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        if (page == MAP_FAILED) {
                //perror("mmap failed");
                return NULL;
        }

        Page_t *_page = new Page_t();
        _page->isFull = false;
        _page->page   = page;
        _page->offset  = 0;
        return _page;
}

// ----------- Chunk System for paging ----------------
template<typename T>
class Chunk_t {
private:
        Page_t* level[PAGE_LEVEL_SIZE];
        bool full;
        page_status status;
        int counter;

public:
        void setLevel(size_t index, Page_t* page_block) {
                level[index]    = page_block;
                status          = page_status::CLEAN;
                full            = false;
        }

        Page_t* getLevel(size_t index) {
                return level[index]; 
        }
};

template<typename T>
Chunk_t<T>* allocate_chuck_t(Page_t* page_block, size_t index) {
        /* For now 'new' is used for the dynammic memory allocation */
        Chunk_t<T>* chunk = new Chunk_t<T>();
        chunk->setLevel(index, page_block);
        return chunk;
}

template<typename T>
class ChunkList_t {
private:
        Chunk_t<T>* chunks[PAGE_LIST_SIZE];
public:
        void setChunk(size_t index, Chunk_t<T>* chunk) {
                chunks[index] = chunk;
        }

        Chunk_t<T>* getChunk(size_t index) {
                return chunks[index];
        }
};

template<typename T>
ChunkList_t<T>* allocate_chucklist_t(Chunk_t<T>* chunk, size_t index) {
        /* For now 'new' is used for the dynammic memory allocation */
        ChunkList_t<T>* clist = new ChunkList_t<T>();
        clist->setChunk(index, chunk);
        return clist;
}

// ------------------- Vec Class ------------------------
template<typename T>
Chunk_t<T>* allocate_chunk_t(Page_t* page_block, size_t index) {
    return allocate_chuck_t<T>(page_block, index);
}

template<typename T>
ChunkList_t<T>* allocate_chunklist_t(Chunk_t<T>* chunk, size_t index) {
    return allocate_chucklist_t<T>(chunk, index);
}

template<typename T>
class Vec {
private:
        size_t list_idx;
        size_t level_idx;

        ChunkList_t<T>* list;

        size_t data_per_page;
        size_t limit;
        size_t _size;
        size_t _capacity;
        uint64_t total_nodes;
        Page_t* next_addr;
        size_t temp_size;
        size_t list_capacity;

public:
        Vec() {
                list_idx                = 0;
                level_idx               = 0;
                data_per_page           = PAGE_SIZE / sizeof(T);
                limit                   = (1 << INIT_LEVEL_INDEX) * data_per_page;
                _size                   = 0;
                _capacity               = data_per_page;
                total_nodes             = 0;
                temp_size               = 0;
                list_capacity           = ((1 << (PAGE_LEVEL_SIZE)) - 1) * data_per_page;

                /* For now 'new' is used for the dynammic memory allocation */
                Page_t* page_block      = allocate_level(INIT_LEVEL_INDEX);
                Chunk_t<T>* chunk       = allocate_chunk_t<T>(page_block, level_idx);
                list                    = allocate_chunklist_t<T>(chunk, list_idx);

                next_addr               = list->getChunk(list_idx)->getLevel(level_idx);
        }

        void push(const T& value) {
                if (temp_size < limit) {
                        void* raw_ptr = static_cast<char*>(next_addr->page) + next_addr->offset;
                        new (raw_ptr) T(value);
                        next_addr->offset += sizeof(T);
                        _size++;
                        temp_size++;
                        return;
                }

                if (__builtin_expect(level_idx >= PAGE_LEVEL_SIZE - 1, 0)) {
                        list_idx++;
                        level_idx = INIT_LEVEL_INDEX;
                        resetLimit();
                } else {
                        level_idx++;
                        limit = (1 << level_idx) * data_per_page;
                }

                temp_size = 0;

                Page_t* page_block      = allocate_level(level_idx);
                if(level_idx == INIT_LEVEL_INDEX) {
                        Chunk_t<T>* chunk       = allocate_chunk_t<T>(page_block, level_idx);
                        list->setChunk(list_idx, chunk);
                }

                list->getChunk(list_idx)->setLevel(level_idx, page_block);

                next_addr               = list->getChunk(list_idx)->getLevel(level_idx);

                void* raw_ptr           = static_cast<char*>(next_addr->page) + next_addr->offset;
                new (raw_ptr) T(value);
                next_addr->offset       += sizeof(T);
                _size++;
                temp_size++;
        }

        T& at(size_t index) {
                /*
                if (index >= _size) {
                        throw std::out_of_range("Index out of range in Vec::at()");
                }
                */

                size_t list_index       = index / list_capacity; 
                size_t inner_index      = index % list_capacity;

                int x                   = inner_index / data_per_page;
                int i                   = 31 - __builtin_clz(x + 1);  // Efficient log2

                int start               = data_per_page * ((1 << i) - 1);
                int offset              = inner_index - start;

                Page_t* page            = list->getChunk(list_index)->getLevel(i);

                char* raw_ptr           = static_cast<char*>(page->page) + (offset * sizeof(T));
                return *reinterpret_cast<T*>(raw_ptr);

                //throw std::out_of_range("unimplemented");
        }

        size_t size() const { return _size; }

        size_t capacity() const { return _capacity; }

        void resetLimit() {
                limit = (1 << INIT_LEVEL_INDEX) * data_per_page;
        }

};


#endif // __VEC__

