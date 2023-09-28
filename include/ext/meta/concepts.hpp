// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_META_CONCEPTS_HEADER
#define EXT_META_CONCEPTS_HEADER
#include <concepts>

namespace ext::meta {

template <typename Container>
concept Associative = requires(Container cont) {
                          typename Container::key_type;
                          typename Container::mapped_type;
                          { cont.begin() } -> std::same_as<typename Container::iterator>;
                          { cont.end() } -> std::same_as<typename Container::iterator>;
                      };

} // namespace ext::meta
#endif // EXT_META_CONCEPTS_HEADER
