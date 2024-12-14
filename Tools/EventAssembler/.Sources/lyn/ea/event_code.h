#ifndef EVENT_CODE_H
#define EVENT_CODE_H

#include <vector>
#include <string>

namespace lyn {

struct event_code {
	enum code_type_enum {
		CODE_BYTE,
		CODE_SHORT,
		CODE_WORD,
		CODE_POIN,
	};

public:
	event_code(code_type_enum type, const std::string& argument, bool allowCombine = true);
	event_code(code_type_enum type, const std::initializer_list<std::string>& arguments, bool allowCombine = true);

	std::string get_code_string() const;
	std::string get_code_string_misaligned() const;

	void write_to_stream_misaligned(std::ostream& output) const;
	void write_to_stream(std::ostream& output) const;

	const std::string& code_name() const;
	const std::string& code_name_misaligned() const;

	unsigned int code_size() const;
	unsigned int code_align() const;

	bool can_combine_with(const event_code& other) const;
	void combine_with(event_code&& other);

private:
	bool mAllowsCombine;
	code_type_enum mCodeType;
	std::vector<std::string> mArguments;

private:
	struct event_code_type {
		std::string name;
		std::string nameMisaligned;

		unsigned int size;
		unsigned int align;
	};

	static std::vector<event_code_type> msCodeTypeLibrary;
};

} // namespace lyn

std::ostream& operator << (std::ostream& stream, const lyn::event_code& eventCode);

#endif // EVENT_CODE_H
