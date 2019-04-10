// Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_MEMORY_TAGGED_POINTER_HEADER
#define OBI_MEMORY_TAGGED_POINTER_HEADER

#include <obi/memory/align.hpp>
#include <memory>
#include <cassert>

namespace obi { namespace memory {

template <typename T, std::uintptr_t Alignment = std::alignment_of_v<T>>
class tagged_pointer {
    std::uintptr_t _pointer;

    static T* get_pointer(std::uintptr_t pointer) {
        return reinterpret_cast<T*>(pointer &  ~mask);
    };

    static std::uintptr_t get_tag(std::uintptr_t pointer) {
        return pointer & mask;
    };

    static std::uintptr_t create(T* p, std::uintptr_t tag) {
        assert(tag <= mask);
        assert(obi::memory::is_alignment(Alignment));
        std::uintptr_t ret = reinterpret_cast<std::uintptr_t>(p);
        return ret | (tag & mask);
    };

public:
    static constexpr std::uintptr_t alignment = Alignment;
    static constexpr std::uintptr_t mask = Alignment - 1;

    tagged_pointer(tagged_pointer const&) noexcept = default;
    tagged_pointer(tagged_pointer&&) noexcept = default;
    tagged_pointer& operator=(tagged_pointer const&) noexcept = default;
    tagged_pointer& operator=(tagged_pointer&&) noexcept = default;

    tagged_pointer() noexcept
        : _pointer(create(nullptr, 0)) {};

    tagged_pointer(T* pointer, std::uintptr_t tag) noexcept
        : _pointer(create(pointer, tag)) {};

    T* get() const {
        return get_pointer(_pointer);
    }

    uintptr_t tag() const {
        return get_tag(_pointer);
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

}}
#endif //OBI_MEMORY_TAGGED_POINTER_HPP
