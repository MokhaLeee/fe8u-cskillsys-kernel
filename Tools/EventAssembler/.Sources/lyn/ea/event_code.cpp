#include "event_code.h"

#include <sstream>

namespace lyn {

std::vector<event_code::event_code_type> event_code::msCodeTypeLibrary = {
//  { name,    name,     size, align },
	{ "BYTE",  "BYTE",   1,    1     }, // CODE_BYTE  = 0
	{ "SHORT", "SHORT2", 2,    2     }, // CODE_SHORT = 1
	{ "WORD",  "WORD2",  4,    4     }, // CODE_WORD  = 2
	{ "POIN",  "POIN2",  4,    4     }, // CODE_POIN  = 3
};

event_code::event_code(code_type_enum type, const std::string& argument, bool allowCombine)
	: mAllowsCombine(allowCombine), mCodeType(type), mArguments({ argument }) {}

event_code::event_code(code_type_enum type, const std::initializer_list<std::string>& arguments, bool allowCombine)
	: mAllowsCombine(allowCombine), mCodeType(type), mArguments(arguments) {}

std::string event_code::get_code_string() const {
	unsigned reserve = code_name().size() + 2;

	for (auto& arg : mArguments)
		reserve += arg.size() + 2;

	std::string result;
	result.reserve(reserve);

	std::ostringstream sstream(result);
	write_to_stream(sstream);

	return result;
}

std::string event_code::get_code_string_misaligned() const {
	unsigned reserve = code_name_misaligned().size() + 2;

	for (auto& arg : mArguments)
		reserve += arg.size() + 2;

	std::string result;
	result.reserve(reserve);

	std::ostringstream sstream(result);
	write_to_stream(sstream);

	return result;
}

void event_code::write_to_stream(std::ostream& output) const {
	auto& codeType = msCodeTypeLibrary[mCodeType];

	output << codeType.name;

	for (auto& arg : mArguments)
		output << " " << arg;
}

void event_code::write_to_stream_misaligned(std::ostream& output) const {
	auto& codeType = msCodeTypeLibrary[mCodeType];

	output << codeType.nameMisaligned;

	for (auto& arg : mArguments)
		output << " " << arg;
}

const std::string& event_code::code_name() const {
	return msCodeTypeLibrary[mCodeType].name;
}

const std::string& event_code::code_name_misaligned() const {
	return msCodeTypeLibrary[mCodeType].nameMisaligned;
}

unsigned int event_code::code_size() const {
	return msCodeTypeLibrary[mCodeType].size * mArguments.size();
}

unsigned int event_code::code_align() const {
	return msCodeTypeLibrary[mCodeType].align;
}

bool event_code::can_combine_with(const event_code& other) const {
	return (mCodeType == other.mCodeType) && (mAllowsCombine) && (other.mAllowsCombine);
}

void event_code::combine_with(event_code&& other) {
	if (!can_combine_with(other))
		throw std::runtime_error(std::string("EVENT CODE: tried combining the following codes:\n\t")
								 .append(get_code_string()).append("\n\t")
								 .append(other.get_code_string()).append("\n"));

	mArguments.reserve(mArguments.size() + other.mArguments.size());

	std::copy(
		std::make_move_iterator(other.mArguments.begin()),
		std::make_move_iterator(other.mArguments.end()),
		std::back_inserter(mArguments)
	);
}

} // namespace lyn

std::ostream& operator << (std::ostream& stream, const lyn::event_code& eventCode) {
	eventCode.write_to_stream(stream);
	return stream;
}
