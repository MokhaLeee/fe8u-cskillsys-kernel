#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <iostream>
#include <cstdarg>

#include "data_chunk.h"

namespace lyn {

/*!
 * \brief class representing a file loaded into memory
 *
 * subclass of lyn::data_chunk specifically representing a data file loaded in memory
 * lyn::data_file provides an interface suitable for use with elfcpp::Elf_file.
 *
 */
struct data_file : public data_chunk {
	struct Location {
		Location(size_type off, size_type len)
			: file_offset(off), data_size(len) {}

		size_type file_offset, data_size;
	};

	struct View : public Location {
		View(const data_file* file, size_type off, size_type len)
			: Location(off, len), pFile(file) {}

		const unsigned char* data() const {
			return pFile->data() + file_offset;
		}

	private:
		const data_file* const pFile;
	};

	using location_type = Location;
	using view_type     = View;

	data_file(const char* fileName);
	data_file(const std::string& fileName);
	data_file(std::string&& fileName);

	using data_chunk::data_chunk;

	view_type view(size_t offset, size_t size) const {
		if ((offset + size) > this->size())
			throw std::runtime_error("Tried to load data out of file boundaries"); // TODO: better error

		return View(this, offset, size);
	}

	view_type view(const location_type& location) const {
		if ((location.file_offset + location.data_size) > this->size())
			throw std::runtime_error("Tried to load data out of file boundaries"); // TODO: better error

		return View(this, location.file_offset, location.data_size);
	}

	void error(const char* format, ...) const;

	void ensure_aligned(unsigned align);

private:
	std::string mFileName;
};

} // namespace lyn

#endif // DATA_FILE_H
