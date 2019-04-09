// get gcc macros
// gcc -dM -E - < /dev/null
#pragma once
#ifndef OBI_CONFIG_HPP
#define OBI_CONFIG_HPP

#ifdef __linux
// define fallback for linux if cmak is not able to figure out
// FIXME - add architecture check
#ifndef OBI_KNOWN_L1_CACHE_LINE_SIZE
#define OBI_KNOWN_L1_CACHE_LINE_SIZE 64
#endif
#endif

#endif // OBI_CONFIG_HPP
