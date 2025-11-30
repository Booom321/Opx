#include <OpxPCH.hpp>

OPX_NAMESPACE_BEGIN

namespace Memory {
    void* Allocate(SizeT size) {
        void* pointer = malloc(size);
        OPX_ASSERT(pointer);
        return pointer;
    }

    void Free(void* pointer) {
        free(pointer);
    }
}  // namespace Memory

OPX_NAMESPACE_END