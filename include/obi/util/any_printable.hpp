// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#pragma once
#ifndef OBI_UTIL_ANY_PRINTABLE_HEADER
#define OBI_UTIL_ANY_PRINTABLE_HEADER

#include <memory>
#include <iostream>
#include <sstream>

namespace obi { namespace util {
class any_printable {
public:
    template <typename T>
    any_printable(T value) : data_ptr(new storage<T>(std::move(value))) {}

    std::ostream& write_to(std::ostream& os) const {
        return data_ptr->write_to(os);
    }

    std::string str() const {
        std::stringstream ss;
        data_ptr->write_to(ss);
        return ss.str();
    }
private:
    // we want to store a pointer to a base class that
    // does not require a template argument
    struct storage_base {
        virtual ~storage_base() = default;
        virtual std::ostream& write_to(std::ostream&) const = 0;
    };

    template <typename T>
    struct storage : storage_base {
        storage(T value) : data(std::move(value)) {}
        std::ostream& write_to(std::ostream& os) const override { return os << data; }
        T data;
    };

    template <typename T>
    T& get(){
        return dynamic_cast<storage<T>&>(*data_ptr).data;
    }

    std::shared_ptr<storage_base const> data_ptr;
};
}} // obi::util

inline std::ostream& operator<<(std::ostream& os, obi::util::any_printable const& any) {
    return any.write_to(os);
}
#endif // OBI_UTIL_ANY_PRINTABLE_HEADER
