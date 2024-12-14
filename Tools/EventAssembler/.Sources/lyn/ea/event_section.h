#ifndef EVENT_SECTION_H
#define EVENT_SECTION_H

#include "ea/event_code.h"
#include "core/data_chunk.h"

#include <iostream>

namespace lyn {

class event_section {
public:
	event_section() = default;

	event_section(const event_section& other) { (*this) = other; }
	event_section(event_section&& other) { (*this) = std::move(other); }

	event_section& operator = (const event_section& other);
	event_section& operator = (event_section&& other);

	void write_to_stream(std::ostream& output, const data_chunk& base) const;

	void resize(unsigned size);

	void map_code(unsigned offset, const event_code& code);
	void map_code(unsigned offset, event_code&& code);

	void compress_codes();
	void optimize();

private:
	std::vector<int> mCodeMap;
	std::vector<event_code> mCodes;
};

} // namespace lyn

#endif // EVENT_SECTION_H
