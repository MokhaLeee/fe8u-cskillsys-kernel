#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H

#include <vector>
#include <cstdint>

namespace lyn {

struct data_chunk : public std::vector<std::uint8_t> {
	using std::vector<std::uint8_t>::vector;

	bool is_cstr_at(unsigned pos) const;
	const char* cstr_at(unsigned pos) const;

	template<typename IntType, unsigned ByteCount = sizeof(IntType)>
	IntType read(unsigned pos) const;

	template<typename IntType, unsigned ByteCount = sizeof(IntType)>
	void write(unsigned pos, IntType value);

	value_type read_byte(unsigned pos) const;
	void write_byte(unsigned pos, value_type value);
};

template<typename IntType, unsigned ByteCount>
IntType data_chunk::read(unsigned pos) const {
	IntType result = 0;

	for (unsigned i=0; i<ByteCount; ++i)
		result |= (read_byte(pos + i) << (i*8));

	return result;
}

template<typename IntType, unsigned ByteCount>
void data_chunk::write(unsigned pos, IntType value) {
	for (unsigned i=0; i<ByteCount; ++i)
		write_byte(pos + i, (value >> (i*8)) & 0xFF);
}

} // namespace lyn

#endif // DATA_CHUNK_H
