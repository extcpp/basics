#pragma once

#include <type_traits>
#include <iostream>
#include <iomanip>

namespace obi { namespace util {

//// HEX
template<typename T>
std::enable_if_t<std::is_integral_v<T>,std::string>
hexify(T i){
    std::stringbuf buf;
    std::ostream os(&buf);

    os //<< "0x"
       << std::setfill('0')
       << std::setw(sizeof(T) * 2)
       << std::hex << i;

    return buf.str().c_str();
}

inline std::string hexify(char const* in, std::size_t len){
    static char const* hex_values = "0123456789abcdef";

	std::string rv;
	rv.reserve(len * 2);

	char const* itr = in;
	char const* end = itr + len;
	while (itr < end) {
		auto c = static_cast<unsigned char>(*(itr++));
		rv.push_back(hex_values[c >> 4]);
		rv.push_back(hex_values[c % 16]);
	}

	return rv;
}

inline std::string hexify(std::string const& in){
	return hexify(in.data(),in.size());
}

inline std::string hexify(std::string_view const& in){
	return hexify(in.data(),in.size());
}

inline std::string dehexify(char const* in, size_t len) {
    std::string rv;

	if (!len) { return rv; }; // no input

	if (len & 0b1) {
		throw std::logic_error("input length must be even");
	}

	rv.reserve(len / 2);

	unsigned char const* start = reinterpret_cast<unsigned char const*>(in);
	unsigned char const* end = start + len;
	unsigned char insert_char = 0;
    int offset = 0;
	for(unsigned const char* itr = start; itr < end; itr++) {
        bool even = !(offset & 0b1);
        if (even) { insert_char = 0; }

		unsigned char current_char = *itr ;
		if (current_char >= '0' && current_char <= '9') {
		    insert_char += (current_char - '0');
		} else if (current_char >= 'a' && current_char <= 'f') {
		    insert_char += (current_char - 'a' + 10);
		} else if (current_char >= 'A' && current_char <= 'F') {
		    insert_char += (current_char - 'A' + 10);
		} else {
		    throw std::logic_error("invalid input");
		}

        if (even){
            insert_char <<= 4;
        } else {
            rv.push_back(insert_char);
        }

        offset++;
    }
	return rv;
}

inline std::string dehexify(std::string const& in) {
    return dehexify(in.data(), in.size());
}

inline std::string dehexify(std::string_view const& in) {
    return dehexify(in.data(), in.size());
}
//// HEX - end

}} // namespace obi::util
