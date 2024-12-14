#include "event_section.h"

#include <stdexcept>
#include <algorithm>

#include "util/hex_write.h"

namespace lyn {

event_section& event_section::operator = (const event_section& other) {
	mCodeMap.resize(other.mCodeMap.size());
	mCodes.clear();

	for (unsigned currentOffset = 0; currentOffset < mCodeMap.size();) {
		if (other.mCodeMap[currentOffset] < 0) {
			mCodeMap[currentOffset] = -1;
			currentOffset++;
		} else {
			unsigned size = other.mCodes[other.mCodeMap[currentOffset]].code_size();

			map_code(currentOffset, other.mCodes[other.mCodeMap[currentOffset]]);
			currentOffset += size;
		}
	}

	return *this;
}

event_section& event_section::operator = (event_section&& other) {
	resize(other.mCodeMap.size());
	mCodes.clear();

	for (unsigned currentOffset = 0; currentOffset < mCodeMap.size();) {
		if (other.mCodeMap[currentOffset] < 0) {
			mCodeMap[currentOffset] = -1;
			currentOffset++;
		} else {
			unsigned size = other.mCodes[other.mCodeMap[currentOffset]].code_size();

			map_code(currentOffset, std::move(other.mCodes[other.mCodeMap[currentOffset]]));
			currentOffset += size;
		}
	}

	return *this;
}

void event_section::write_to_stream(std::ostream& output, const data_chunk& base) const {
	for (unsigned pos = 0; pos < mCodeMap.size();) {
		if (mCodeMap[pos] < 0) {
			auto now = std::next(mCodeMap.begin(), pos);

			auto it = std::find_if(
				now, mCodeMap.end(),
				[] (int value) -> bool { return value >= 0; }
			);

			unsigned distance = std::distance(now, it);

			if ((distance >= 4) && ((pos % 4) == 0)) {
				output << "WORD";

				while (distance >= 4) {
					output << util::make_hex_string(" $", base.read<uint32_t>(pos));

					pos      += 4;
					distance -= 4;
				}

				output << std::endl;
			} else if ((distance >= 2) && ((pos % 2) == 0)) {
				output << "SHORT"
					   << util::make_hex_string(" $", base.read<uint16_t>(pos))
					   << std::endl;

				pos      += 2;
				distance -= 2;
			} else if (distance >= 1) {
				output << "BYTE"
					   << util::make_hex_string(" $", base.read<uint8_t>(pos))
					   << std::endl;

				pos++;
				distance--;
			}
		} else {
			const event_code& code = mCodes[mCodeMap[pos]];

			if ((pos % code.code_align()) == 0)
				code.write_to_stream(output);
			else
				code.write_to_stream_misaligned(output);

			output << std::endl;
			pos += code.code_size();
		}
	}
}

void event_section::resize(unsigned size) {
	unsigned oldSize = mCodeMap.size();

	mCodeMap.resize(size);

	std::fill(
		std::next(mCodeMap.begin(), oldSize),
		mCodeMap.end(),

		-1
	);
}

void event_section::map_code(unsigned offset, const event_code& code) {
	unsigned size = code.code_size();

	if (offset + size > mCodeMap.size())
		resize(offset + size);

	int index = mCodes.size();
	mCodes.push_back(code);

	for (unsigned i = 0; i < size; ++i)
		mCodeMap[offset + i] = index;
}

void event_section::map_code(unsigned int offset, event_code&& code) {
	unsigned size = code.code_size();

	if (offset + size > mCodeMap.size())
		resize(offset + size);

	int index = mCodes.size();
	mCodes.push_back(std::move(code));

	for (unsigned i = 0; i < size; ++i)
		mCodeMap[offset + i] = index;
}

void event_section::compress_codes() {
	for (unsigned currentOffset = 0; currentOffset < mCodeMap.size();) {
		int index = mCodeMap[currentOffset];

		if (index < 0) {
			currentOffset++;
			continue;
		}

		auto& code = mCodes[index];

		if (currentOffset + code.code_size() >= mCodeMap.size())
			break;

		auto& nextCode = mCodes[mCodeMap[currentOffset + code.code_size()]];

		if (code.can_combine_with(nextCode)) {
			code.combine_with(std::move(nextCode));

			for (unsigned i=0; i<code.code_size(); ++i)
				mCodeMap[currentOffset + i] = index;
		} else {
			currentOffset += code.code_size();
		}
	}
}

void event_section::optimize() {
	event_section other(*this);

	mCodeMap = std::move(other.mCodeMap);
	mCodes   = std::move(other.mCodes);
}

} // namespace lyn
