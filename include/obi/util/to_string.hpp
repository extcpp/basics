#pragma once
#include <filesystem>

inline std::string to_type_string(::std::filesystem::recursive_directory_iterator const& iter){
    using namespace ::std::literals;
    if(iter->is_symlink())        { return "symlink"s; }
    if(iter->is_regular_file())   { return "file"s; }
    if(iter->is_directory())      { return "directory"s; }
    if(iter->is_block_file())     { return "block file"s; }
    if(iter->is_character_file()) { return "character file"s; }
    if(iter->is_fifo())           { return "fifo"s; }
    if(iter->is_other())          { return "other"s; }
    return "unknown"s;
}

