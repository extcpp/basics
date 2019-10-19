// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_BENCHMARK_HEADER
#define EXT_UTIL_BENCHMARK_HEADER

#include <atomic>
#include <ext/macros/compiler.hpp>

namespace ext { namespace util {

inline void do_not_optimize(void* pointer) {
#ifdef EXT_COMPILER_GNU
    // Ihttps://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
    asm volatile(""             // template
                 :              // output
                 : "g"(pointer) // input - "g" any general register
                 : "memory");   // clobber - Note that this clobber does not
                                // prevent the processor from doing speculative
                                // reads past the asm statement. To prevent that
                                // you need processor-specific fence instructions.
#else
    // https://docs.microsoft.com/en-us/cpp/intrinsics/readwritebarrier?view=vs-2019
    // _ReadWriteBarrier
    std::atomic_thread_fence(std::memory_order_seq_cst);
#endif // EXT_COMPILER_GNU
}

inline void clobber_memory() {
#ifdef EXT_COMPILER_GNU
    asm volatile("" // template
                 :  // output
                 :
                 : "memory"); // clobber - Note that this clobber does not
                              // prevent the processor from doing speculative
                              // reads past the asm statement. To prevent that
                              // you need processor-specific fence instructions.
#else
    std::atomic_thread_fence(std::memory_order_seq_cst);
#endif // EXT_COMPILER_GNU
}

}}     // namespace ext::util
#endif // EXT_UTIL_BENCHMARK_HEADER
