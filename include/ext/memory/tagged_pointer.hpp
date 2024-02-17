// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_MEMORY_TAGGED_POINTER_HEADER
#define EXT_MEMORY_TAGGED_POINTER_HEADER

#include <ext/memory/align.hpp>

#include <cassert>
#include <cstdint>
#include <memory>

namespace ext { namespace memory {

template <typename T, std::uintptr_t Alignment = std::alignment_of_v<T>>
class tagged_pointer {
    std::uintptr_t _pointer;

    static T* get_pointer(std::uintptr_t pointer) {
        return reinterpret_cast<T*>(pointer & ~mask);
    };

    static std::uintptr_t get_tag(std::uintptr_t pointer) {
        return pointer & mask;
    };

    static std::uintptr_t create(T* p, std::uintptr_t tag) {
        assert(tag <= mask);
        assert(ext::memory::is_alignment(Alignment));
        std::uintptr_t ret = reinterpret_cast<std::uintptr_t>(p);
        return ret | (tag & mask);
    };

public:
    static constexpr std::uintptr_t alignment = Alignment;
    static constexpr std::uintptr_t mask = Alignment - 1;

    // A default constructed tagged_pointer is as *real* pointer uninitialized.
    // Note: Becauase of asserts (in debug mode) you need to set the tag first
    // if you are not setting both at the same time.
    tagged_pointer() noexcept = default;
    tagged_pointer(tagged_pointer const&) noexcept = default;
    tagged_pointer(tagged_pointer&&) noexcept = default;
    tagged_pointer& operator=(tagged_pointer const&) noexcept = default;
    tagged_pointer& operator=(tagged_pointer&&) noexcept = default;

    tagged_pointer(T* pointer, std::uintptr_t tag) noexcept : _pointer(create(pointer, tag)){};

    T* get() const {
        return get_pointer(_pointer);
    }

    std::uintptr_t tag() const {
        return get_tag(_pointer);
    }

    std::uintptr_t tag_next() const {
        return (get_tag(_pointer) + 1) & mask;
    }

    void set(T* pointer, std::uintptr_t tag) {
        _pointer = create_tag(pointer, tag);
    }

    void set(T* pointer) {
        auto tag = get_tag(_pointer);
        _pointer = create(pointer, tag);
    }

    void set(uintptr_t tag) {
        auto pointer = get_pointer(_pointer);
        _pointer = create(pointer, tag);
    }

    T& operator*() const {
        return *get_pointer();
    }

    T* operator->() const {
        return get_pointer();
    }

    operator bool() const {
        return get_pointer() != nullptr;
    }

    bool operator==(tagged_pointer const& pointer) const {
        return _pointer == pointer._pointer;
    }

    bool operator==(T const* pointer) const {
        return get_pointer(_pointer) == pointer;
    }

    bool operator!=(tagged_pointer const& pointer) const {
        return !operator==(pointer);
    }

    bool operator!=(T const* pointer) const {
        return !operator==(pointer);
    }
};
}}     // namespace ext::memory
#endif // EXT_MEMORY_TAGGED_POINTER_HEADER
