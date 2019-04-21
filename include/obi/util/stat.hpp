#pragma once
#ifndef OBI_UTIL_STAT_HEADER
#define OBI_UTIL_STAT_HEADER

#include <obi/util/result.hpp>
#include <filesystem>

extern "C" {
    #include <sys/types.h>
    #include <sys/stat.h>
}

namespace obi::util {

namespace {
    struct ::stat buf;
}

struct stat {
    using inode_type = decltype(buf.st_ino);
    using uid_type = decltype(buf.st_uid);
    using gid_type = decltype(buf.st_gid);
    using size_type = decltype(buf.st_size);
    using nlink_type = decltype(buf.st_nlink);

    inode_type inode;
    uid_type uid;
    gid_type gid;
    size_type size;
    nlink_type nlink;
};

inline typed_result<stat> get_stat(std::filesystem::path const& path) {
    typed_result<stat> rv;

    struct ::stat statbuff;
    auto stat_result = ::stat(path.c_str(), &statbuff);
    if (stat_result != 0) {
        rv.reset(OBI_FAIL, "failed to get stat");
    }

    rv.value.inode = statbuff.st_ino;
    rv.value.gid = statbuff.st_gid;
    rv.value.uid = statbuff.st_uid;
    rv.value.size = statbuff.st_size;
    rv.value.nlink = statbuff.st_nlink;

    return rv;
}

} // namespace obi::util - end
#endif // OBI_UTIL_STAT_HEADER