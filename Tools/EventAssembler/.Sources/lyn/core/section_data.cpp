#include "section_data.h"

#include "../ea/event_section.h"
#include "util/hex_write.h"

#include <algorithm>

namespace lyn {

int section_data::mapping_type_at(unsigned int offset) const {
	for (auto mapping : mMappings)
		if (mapping.offset <= offset)
			return mapping.type;

	return mapping::Data;
}

void section_data::set_mapping(unsigned int offset, mapping::type_enum type) {
	for (auto& mapping : mMappings)
		if (mapping.offset == offset) {
			mapping.type = type;
			return;
		}
	mMappings.push_back({ type, offset });

	std::sort(mMappings.begin(), mMappings.end(), [] (const mapping& left, const mapping& right) {
		return left.offset < right.offset;
	});
}

} // namespace lyn
