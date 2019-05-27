// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_BENCHMARK_HEADER
#define OBI_UTIL_BENCHMARK_HEADER

#include <obi/macros/compiler.hpp>
#include <atomic>

namespace obi { namespace util {

inline void do_not_optimize(void* pointer) {
#ifdef OBI_COMPILER_GNU
    // Ihttps://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
    asm volatile(""             // template
                :               // output
                : "g"(pointer)  // input - "g" any general register
                : "memory");    // clobber - Note that this clobber does not
                                // prevent the processor from doing speculative
                                // reads past the asm statement. To prevent that
                                // you need processor-specific fence instructions.
#else
    // https://docs.microsoft.com/en-us/cpp/intrinsics/readwritebarrier?view=vs-2019
    // _ReadWriteBarrier
    std::atomic_thread_fence(std::memory_order_seq_cst);
#endif // OBI_COMPILER_GNU
}

inline void clobber_memory() {
#ifdef OBI_COMPILER_GNU
    asm volatile(""             // template
                :               // output
                :
                : "memory");    // clobber - Note that this clobber does not
                                // prevent the processor from doing speculative
                                // reads past the asm statement. To prevent that
                                // you need processor-specific fence instructions.
#else
    std::atomic_thread_fence(std::memory_order_seq_cst);
#endif // OBI_COMPILER_GNU
}

}}
#endif // OBI_UTIL_BENCHMARK_HEADER
