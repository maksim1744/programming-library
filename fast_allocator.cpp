const int MEMSIZE = 1e9;  // in bytes
char memory[MEMSIZE];
int memorypos;
inline void * operator new(size_t n){
    if (memorypos + n >= MEMSIZE) {
        memorypos = MEMSIZE / 3;
    }
    char * ret = memory + memorypos;
    memorypos += n;
    return (void *)ret;
}
inline void operator delete(void *){}
inline void operator delete(void *, size_t){}