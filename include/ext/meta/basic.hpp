// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_META_BASIC_HEADER
#define EXT_META_BASIC_HEADER
#include <type_traits>

namespace ext::meta {

template<typename T>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;

}
#endif // EXT_META_BASIC_HEADER
