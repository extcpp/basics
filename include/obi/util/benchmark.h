// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_BENCHMARK_HEADER
#define OBI_UTIL_BENCHMARK_HEADER

namespace obi { namespace util {

inline void escape(void* pointer) {
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm
    asm volatile(""             // template
                :               // output
                : "g"(pointer)  // input - "g" any general register
                : "memory");    // clobber - Note that this clobber does not
                                // prevent the processor from doing speculative
                                // reads past the asm statement. To prevent that
                                // you need processor-specific fence instructions.
}

}}
#endif // OBI_UTIL_BENCHMARK_HEADER
