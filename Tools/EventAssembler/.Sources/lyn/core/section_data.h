#ifndef SECTION_DATA_H
#define SECTION_DATA_H

#include <string>

#include "data_chunk.h"

namespace lyn {

struct section_data : public data_chunk {
	struct mapping {
		enum type_enum {
			Data,
			Thumb,
			ARM
		};

		type_enum type;
		unsigned offset;
	};

	struct symbol {
		std::string name;
		unsigned int offset;
		bool isLocal;
	};

	struct relocation {
		std::string symbolName;
		int addend;

		unsigned type;
		unsigned offset;
	};

public:
	void set_name(const std::string& name) { mName = name; }
	const std::string& name() const { return mName; }

	const std::vector<relocation>& relocations() const { return mRelocations; }
	std::vector<relocation>& relocations() { return mRelocations; }

	const std::vector<symbol>& symbols() const { return mSymbols; }
	std::vector<symbol>& symbols() { return mSymbols; }

	int mapping_type_at(unsigned int offset) const;
	void set_mapping(unsigned int offset, mapping::type_enum type);

private:
	std::string mName;

	std::vector<relocation> mRelocations;
	std::vector<symbol> mSymbols;
	std::vector<mapping> mMappings;
};

} // namespace lyn

#endif // SECTION_DATA_H
