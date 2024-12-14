#include "data_chunk.h"

namespace lyn {

bool data_chunk::is_cstr_at(unsigned pos) const {
	do {
		if (!at(pos))
			return true;
	} while ((++pos) < size());

	return false;
}

const char* data_chunk::cstr_at(unsigned pos) const {
	return reinterpret_cast<const char*>(data() + pos);
}

data_chunk::value_type data_chunk::read_byte(unsigned pos) const {
	return at(pos);
}

void data_chunk::write_byte(unsigned pos, data_chunk::value_type value) {
	at(pos) = value;
}

} // namespace lyn
