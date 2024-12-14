#include "data_file.h"

#include <fstream>
#include <iterator>

namespace lyn {

data_file::data_file(const char* fileName)
	: data_file(std::string(fileName)) {}

data_file::data_file(const std::string& fileName)
	: data_file(std::string(fileName)) {}

data_file::data_file(std::string&& fileName)
	: mFileName(std::move(fileName)) {
	std::ifstream input;

	input.open(mFileName, std::ios::in | std::ios::binary);

	if (!input.is_open())
		throw std::runtime_error(std::string("Couldn't open file for read: ").append(fileName)); // TODO: better error

	input.seekg(0, std::ios::end);
	resize(input.tellg());

	input.seekg(0);
	input.read(reinterpret_cast<std::ifstream::char_type*>(data()), size());
}

void data_file::error(const char* format, ...) const {
	std::va_list args;

	va_start(args, format);

	unsigned stringSize; {
		std::va_list argsCpy;
		va_copy(argsCpy, args);

		stringSize = std::vsnprintf(nullptr, 0, format, argsCpy);
		va_end(argsCpy);
	}

	std::vector<char> buf(stringSize + 1);
	std::vsnprintf(buf.data(), buf.size(), format, args);

	va_end(args);

	throw std::runtime_error(std::string(buf.begin(), buf.end())); // TODO: better error
}

void data_file::ensure_aligned(unsigned align) {
	if (unsigned off = (size() % align))
		resize(size() + (align - off));
}

} // namespace lyn
