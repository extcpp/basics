// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_FILES_HEADER
#define OBI_UTIL_FILES_HEADER

#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>

#include <obi/util/scoped_timer.hpp>

namespace obi::util {

bool files_equal(std::filesystem::path const& file1
                ,std::filesystem::path const& file2
                ,bool verbose = false
				){
    scoped_timer timer("files_equal");
    if (verbose) {
        std::cerr << std::endl  << "compare: "
                  << std::endl << file1
                  << std::endl << file2
                  << std::endl;
    } else {
        timer.dismiss();
    }

    // memory mapped with recent gcc when optimized
    std::ifstream stream1(file1);
    std::ifstream stream2(file2);

    std::istreambuf_iterator iter1(stream1);
    std::istreambuf_iterator iter2(stream2);
    decltype(iter1) end;

    bool rv = std::equal(iter1, end, iter2, end);

    // more output here?
    return rv;
}

}
#endif // OBI_UTIL_FILES_HEADER
