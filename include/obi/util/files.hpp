#pragma once

#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace obi::util {

bool files_equal(std::filesystem::path const& file1
                ,std::filesystem::path const& file2
				){
    // memory mapped with recent gcc
    std::ifstream stream1(file1);
    std::ifstream stream2(file2);

    std::istreambuf_iterator iter1(stream1);
    std::istreambuf_iterator iter2(stream2);
    decltype(iter1) end;

    return std::equal(iter1, end, iter2, end);
}

}
