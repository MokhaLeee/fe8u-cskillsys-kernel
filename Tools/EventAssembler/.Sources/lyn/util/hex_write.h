#ifndef HEX_RW_H
#define HEX_RW_H

#include <cstdint>
#include <cmath>
#include <cstring>

#include <type_traits>
#include <iterator>

namespace util {

template<typename IntegerType = unsigned>
char hex_digit(const IntegerType& value) {
	if ((value & 0xF) < 10)
		return '0' + (value & 0xF);
	return 'A' + (value & 0xF) - 10;
}

template<typename IntegerType = unsigned>
unsigned hex_digit_count(typename std::remove_reference<IntegerType>::type value) {
	unsigned result = 0;

	while (value) {
		value >>= 4;
		result++;
	}

	return std::max<unsigned>(result, 1);
}

template<typename OutputIteratorType, typename IntegerType = unsigned>
OutputIteratorType hex_write(OutputIteratorType out, IntegerType value) {
	const unsigned digits = hex_digit_count<IntegerType>(value);

	for (unsigned i = 0; i < digits; ++i)
		*out++ = hex_digit<IntegerType>((value >> ((digits - i - 1)*4)) & 0xF);

	return out;
}

template<typename ContainerType, typename IntegerType = unsigned>
ContainerType& append_hex(ContainerType& container, IntegerType value) {
	hex_write(std::back_inserter(container), value);
	return container;
}

template<typename IntegerType = unsigned>
std::string make_hex_string(const char* prefix, IntegerType value) {
	std::string result;

	result.reserve(std::strlen(prefix) + hex_digit_count<IntegerType>(value));

	result.append(prefix);
	append_hex<std::string, IntegerType>(result, value);

	return result;
}

} // namespace util

#endif // HEX_RW_H
