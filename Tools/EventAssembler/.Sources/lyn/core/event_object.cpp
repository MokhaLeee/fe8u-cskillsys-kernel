#include "event_object.h"

#include <fstream>

#include <iomanip>
#include <algorithm>

#include "elfcpp/elfcpp_file.h"
#include "elfcpp/arm.h"

#include "core/data_file.h"
#include "ea/event_section.h"
#include "util/hex_write.h"

namespace elfcpp {

// for some reason this is required in order to make the whole thing work

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::ehdr_size;

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::phdr_size;

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::shdr_size;

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::sym_size;

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::rel_size;

template<int Size, bool BigEndian, typename File>
const int Elf_file<Size, BigEndian, File>::rela_size;

} // namespace elfcpp

namespace lyn {

void event_object::append_from_elf(const char* fileName)
{
	data_file file(fileName);

	elfcpp::Elf_file<32, false, lyn::data_file> elfFile(&file);

	auto readString = [&file] (elfcpp::Shdr<32, false> section, unsigned offset) -> std::string
	{
		return file.cstr_at(section.get_sh_offset() + offset);
	};

	std::vector<section_data> newSections(elfFile.shnum());
	std::vector<bool> outMap(elfFile.shnum(), false);

	auto getLocalSymbolName = [this] (int section, int index) -> std::string
	{
		std::string result;
		result.reserve(0x10);

		result.append("_L");
		util::append_hex(result, mSections.size() + section);
		result.append("_");
		util::append_hex(result, index);

		return result;
	};

	auto getGlobalSymbolName = [] (const char* name) -> std::string
	{
		std::string result(name);
		std::size_t find = 0;

		while ((find = result.find('.')) != std::string::npos)
			result[find] = '_';

		return result;
	};

	for (unsigned i = 0; i < elfFile.shnum(); ++i)
	{
		auto flags = elfFile.section_flags(i);

		if ((flags & elfcpp::SHF_ALLOC) && !(flags & elfcpp::SHF_WRITE))
		{
			auto& section = newSections.at(i);
			auto  loc     = elfFile.section_contents(i);

			// TODO: put filename in name (whenever name will be useful)

			section.set_name(elfFile.section_name(i));

			section.clear();
			section.reserve(loc.data_size);

			std::copy(
				std::next(file.begin(), loc.file_offset),
				std::next(file.begin(), loc.file_offset+loc.data_size),
				std::back_inserter(section)
			);

			outMap[i] = true;
		}
	}

	for (unsigned si = 0; si < elfFile.shnum(); ++si)
	{
		elfcpp::Shdr<32, false> header(&file, elfFile.section_header(si));

		switch (header.get_sh_type())
		{

		case elfcpp::SHT_SYMTAB:
		{
			const unsigned count = header.get_sh_size() / header.get_sh_entsize();
			const elfcpp::Shdr<32, false> nameShdr(&file, elfFile.section_header(header.get_sh_link()));

			for (unsigned i = 0; i < count; ++i)
			{
				elfcpp::Sym<32, false> sym(&file, data_file::Location(
					header.get_sh_offset() + i * header.get_sh_entsize(),
					header.get_sh_entsize()
				));

				switch (sym.get_st_shndx())
				{

				case elfcpp::SHN_ABS:
				{
					auto name = (sym.get_st_bind() == elfcpp::STB_LOCAL)
						? getLocalSymbolName(si, i)
						: getGlobalSymbolName(readString(nameShdr, sym.get_st_name()).c_str());

					mAbsoluteSymbols.push_back(section_data::symbol {
						name, sym.get_st_value(), false
					});

					break;
				} // case elfcpp::SHN_ABS

				default:
				{
					if (sym.get_st_shndx() >= outMap.size())
						break;

					if (!outMap.at(sym.get_st_shndx()))
						break;

					auto& section = newSections.at(sym.get_st_shndx());

					std::string name = readString(nameShdr, sym.get_st_name());

					if (sym.get_st_type() == elfcpp::STT_NOTYPE && sym.get_st_bind() == elfcpp::STB_LOCAL)
					{
						std::string subString = name.substr(0, 3);

						if ((name == "$t") || (subString == "$t."))
						{
							section.set_mapping(sym.get_st_value(), section_data::mapping::Thumb);
							break;
						}

						if ((name == "$a") || (subString == "$a."))
						{
							section.set_mapping(sym.get_st_value(), section_data::mapping::ARM);
							break;
						}

						if ((name == "$d") || (subString == "$d."))
						{
							section.set_mapping(sym.get_st_value(), section_data::mapping::Data);
							break;
						}
					}

					if (sym.get_st_bind() == elfcpp::STB_LOCAL)
						name = getLocalSymbolName(si, i);
					else
						name = getGlobalSymbolName(name.c_str());

					section.symbols().push_back(section_data::symbol {
						name,
						sym.get_st_value(),
						(sym.get_st_bind() == elfcpp::STB_LOCAL)
					});

					break;
				} // default

				} // switch (sym.get_st_shndx())
			}

			break;
		} // case elfcpp::SHT_SYMTAB

		case elfcpp::SHT_REL:
		{
			if (!outMap.at(header.get_sh_info()))
				break;

			const unsigned count = header.get_sh_size() / header.get_sh_entsize();

			const elfcpp::Shdr<32, false> symShdr(&file, elfFile.section_header(header.get_sh_link()));
			const elfcpp::Shdr<32, false> symNameShdr(&file, elfFile.section_header(symShdr.get_sh_link()));

			auto& section = newSections.at(header.get_sh_info());

			for (unsigned i = 0; i < count; ++i)
			{
				const elfcpp::Rel<32, false> rel(&file, data_file::Location(
					header.get_sh_offset() + i * header.get_sh_entsize(),
					header.get_sh_entsize()
				));

				const elfcpp::Sym<32, false> sym(&file, data_file::Location(
					symShdr.get_sh_offset() + elfcpp::elf_r_sym<32>(rel.get_r_info()) * symShdr.get_sh_entsize(),
					symShdr.get_sh_entsize()
				));

				const std::string name = (sym.get_st_bind() == elfcpp::STB_LOCAL)
					? getLocalSymbolName(header.get_sh_link(), elfcpp::elf_r_sym<32>(rel.get_r_info()))
					: getGlobalSymbolName(readString(symNameShdr, sym.get_st_name()).c_str());

				section.relocations().push_back(section_data::relocation {
					name,
					0,
					elfcpp::elf_r_type<32>(rel.get_r_info()),
					rel.get_r_offset()
				});
			}

			break;
		} // case elfcpp::SHT_REL

		case elfcpp::SHT_RELA:
		{
			if (!outMap.at(header.get_sh_info()))
				break;

			const unsigned count = header.get_sh_size() / header.get_sh_entsize();

			const elfcpp::Shdr<32, false> symShdr(&file, elfFile.section_header(header.get_sh_link()));
			const elfcpp::Shdr<32, false> symNameShdr(&file, elfFile.section_header(symShdr.get_sh_link()));

			auto& section = newSections.at(header.get_sh_info());

			for (unsigned i = 0; i < count; ++i)
			{
				const elfcpp::Rela<32, false> rela(&file, data_file::Location(
					header.get_sh_offset() + i * header.get_sh_entsize(),
					header.get_sh_entsize()
				));

				const elfcpp::Sym<32, false> sym(&file, data_file::Location(
					symShdr.get_sh_offset() + elfcpp::elf_r_sym<32>(rela.get_r_info()) * symShdr.get_sh_entsize(),
					symShdr.get_sh_entsize()
				));

				const std::string name = (sym.get_st_bind() == elfcpp::STB_LOCAL)
					? getLocalSymbolName(header.get_sh_link(), elfcpp::elf_r_sym<32>(rela.get_r_info()))
					: getGlobalSymbolName(readString(symNameShdr, sym.get_st_name()).c_str());

				section.relocations().push_back(section_data::relocation {
					name,
					rela.get_r_addend(),
					elfcpp::elf_r_type<32>(rela.get_r_info()),
					rela.get_r_offset()
				});
			}

			break;
		} // case elfcpp::SHT_RELA

		} // switch (header.get_sh_type())
	}

	// Remove empty sections

	newSections.erase(std::remove_if(newSections.begin(), newSections.end(),
		[] (const section_data& section)
		{
			return (section.size()==0);
		}
	), newSections.end());

	// Add to existing section list

	mSections.reserve(mSections.size() + newSections.size());

	std::copy(
		std::make_move_iterator(newSections.begin()),
		std::make_move_iterator(newSections.end()),
		std::back_inserter(mSections)
	);
}

void event_object::try_transform_relatives() {
	for (auto& section : mSections) {
		for (auto& relocation : section.relocations()) {
			if (auto relocatelet = mRelocator.get_relocatelet(relocation.type)) {
				if (!relocatelet->is_absolute() && relocatelet->can_make_trampoline()) {
					std::string renamed;

					renamed.reserve(4 + relocation.symbolName.size());

					renamed.append("_LP_"); // local proxy
					renamed.append(relocation.symbolName);

					bool exists = false;

					for (auto& section : mSections)
						for (auto& sym : section.symbols())
							if (sym.name == renamed)
								exists = true;

					if (!exists) {
						section_data newData = relocatelet->make_trampoline(relocation.symbolName, relocation.addend);
						newData.symbols().push_back({ renamed, (newData.mapping_type_at(0) == section_data::mapping::Thumb), true });

						mSections.push_back(std::move(newData));
					}

					relocation.symbolName = renamed;
					relocation.addend = 0; // TODO: -4
				}
			}
		}
	}
}

void event_object::try_relocate_relatives() {
	unsigned offset = 0;

	for (auto& section : mSections) {
		section.relocations().erase(
			std::remove_if(
				section.relocations().begin(),
				section.relocations().end(),
				[this, &section, offset] (section_data::relocation& relocation) -> bool {
					unsigned symOffset = 0;

					auto relocatelet = mRelocator.get_relocatelet(relocation.type);

					for (auto& symSection : mSections) {
						for (auto& symbol : symSection.symbols()) {
							if (symbol.name != relocation.symbolName)
								continue;

							if (relocatelet && !relocatelet->is_absolute()) {
								relocatelet->apply_relocation(
									section,
									relocation.offset,
									symOffset + symbol.offset,
									relocation.addend
								);

								return true;
							}

							relocation.symbolName = "CURRENTOFFSET";
							relocation.addend += (symOffset + symbol.offset) - (offset + relocation.offset);

							return false;
						}

						symOffset += symSection.size();

						if (unsigned misalign = (symOffset % 4))
							symOffset += (4 - misalign);
					}

					return false;
				}
			),
			section.relocations().end()
		);

		offset += section.size();

		if (unsigned misalign = (offset % 4))
			offset += (4 - misalign);
	}
}

void event_object::try_relocate_absolutes() {
	for (auto& section : mSections) {
		section.relocations().erase(
			std::remove_if(
				section.relocations().begin(),
				section.relocations().end(),
				[this, &section] (const section_data::relocation& relocation) -> bool {
					for (auto& symbol : mAbsoluteSymbols) {
						if (symbol.name != relocation.symbolName)
							continue;

						if (auto relocatelet = mRelocator.get_relocatelet(relocation.type)) {
							if (relocatelet->is_absolute()) {
								relocatelet->apply_relocation(
									section,
									relocation.offset,
									symbol.offset,
									relocation.addend
								);

								return true;
							}
						}

						return false;
					}

					return false;
				}
			),
			section.relocations().end()
		);
	}
}

void event_object::remove_unnecessary_symbols() {
	for (auto& section : mSections) {
		section.symbols().erase(
			std::remove_if(
				section.symbols().begin(),
				section.symbols().end(),
				[this] (const section_data::symbol& symbol) -> bool {
					if (!symbol.isLocal)
						return false; // symbol may be used outside of the scope of this object

					for (auto& section : mSections)
						for (auto& reloc : section.relocations())
							if (reloc.symbolName == symbol.name)
								return false; // a relocation is dependant on this symbol

					return true; // symbol is local and unused, we can remove it safely (hopefully)
				}
			),
			section.symbols().end()
		);
	}
}

void event_object::cleanup() {
	for (auto& section : mSections) {
		std::sort(
			section.symbols().begin(),
			section.symbols().end(),
			[] (const section_data::symbol& a, const section_data::symbol& b) -> bool {
				return a.offset < b.offset;
			}
		);
	}
}

std::vector<event_object::hook> event_object::get_hooks() const {
	std::vector<hook> result;

	for (auto& absSymbol : mAbsoluteSymbols) {
		if (!(absSymbol.offset & 0x8000000))
			continue; // Not in ROM

		for (auto& section : mSections) {
			for (auto& locSymbol : section.symbols()) {
				if (absSymbol.name != locSymbol.name)
					continue; // Not same symbol

				result.push_back({ (absSymbol.offset & (~0x8000000)), absSymbol.name });
			}
		}
	}

	return result;
}

void event_object::write_events(std::ostream& output) const {
	unsigned offset = 0;

	for (auto& section : mSections) {
		event_section events; // = section.make_events();
		events.resize(section.size());

		output << "ALIGN 4" << std::endl;

		for (auto& relocation : section.relocations())
		{
			if (auto relocatelet = mRelocator.get_relocatelet(relocation.type))
			{
				// This is probably the worst hack I've ever written
				// lyn needs a rewrite

				// (I makes sure that relative relocations to known absolute values will reference the value and not the name)

				auto it = std::find_if(
					mAbsoluteSymbols.begin(),
					mAbsoluteSymbols.end(),
					[&] (const section_data::symbol& sym) { return sym.name == relocation.symbolName; });

				auto symName = (it == mAbsoluteSymbols.end())
					? relocation.symbolName
					: util::make_hex_string("$", it->offset);

				events.map_code(relocation.offset, relocatelet->make_event_code(
					section,
					relocation.offset,
					symName,
					relocation.addend
				));
			}
			else if (relocation.type != elfcpp::R_ARM_V4BX) // dirty hack
				throw std::runtime_error(std::string("unhandled relocation type #").append(std::to_string(relocation.type)));
		}

		events.compress_codes();
		events.optimize();

		if (std::any_of(
			section.symbols().begin(),
			section.symbols().end(),

			[] (const section_data::symbol& sym) {
				return !sym.isLocal;
			}
		)) {
			output << "PUSH" << std::endl;
			int currentOffset = 0;

			for (auto& symbol : section.symbols()) {
				if (symbol.isLocal)
					continue;

				output << "ORG CURRENTOFFSET+$" << std::hex << (symbol.offset - currentOffset) << ";"
					   << symbol.name << ":" << std::endl;
				currentOffset = symbol.offset;
			}

			output << "POP" << std::endl;
		}

		if (std::any_of(
			section.symbols().begin(),
			section.symbols().end(),

			[] (const section_data::symbol& sym) {
				return sym.isLocal;
			}
		)) {
			output << "{" << std::endl;
			output << "PUSH" << std::endl;

			int currentOffset = 0;

			for (auto& symbol : section.symbols()) {
				if (!symbol.isLocal)
					continue;

				output << "ORG CURRENTOFFSET+$" << std::hex << (symbol.offset - currentOffset) << ";"
					   << symbol.name << ":" << std::endl;
				currentOffset = symbol.offset;
			}

			output << "POP" << std::endl;

			events.write_to_stream(output, section);

			output << "}" << std::endl;
		} else {
			events.write_to_stream(output, section);
		}

		offset += section.size();

		if (unsigned misalign = (offset % 4))
			offset += (4 - misalign);
	}
}

} // namespace lyn
