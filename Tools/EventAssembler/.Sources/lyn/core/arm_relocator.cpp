#include "arm_relocator.h"

#include <algorithm>

#include "util/hex_write.h"

#include "elfcpp/elfcpp.h"
#include "elfcpp/arm.h"

namespace lyn {

struct arm_data_abs32_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string& sym, int addend) const {
		return lyn::event_code(
			lyn::event_code::CODE_POIN,
			arm_relocator::abs_reloc_string(sym, data.read<std::uint32_t>(offset) + addend),
			false
		);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		data.write<std::uint32_t>(offset, (data.read<std::uint32_t>(offset) + value + addend));
	}
};

struct arm_data_rel32_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string &sym, int addend) const {
		return lyn::event_code(
			lyn::event_code::CODE_WORD,
			arm_relocator::rel_reloc_string(sym, data.read<std::uint32_t>(offset) + addend),
			false
		);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		data.write<std::uint32_t>(offset, (data.read<std::uint32_t>(offset) + value + addend - offset));
	}
};

struct arm_data_abs16_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string& sym, int addend) const {
		return lyn::event_code(
			lyn::event_code::CODE_SHORT,
			arm_relocator::abs_reloc_string(sym, data.read<std::uint16_t>(offset) + addend),
			false
		);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		data.write<std::uint16_t>(offset, (data.read<std::uint16_t>(offset) + value + addend));
	}
};

struct arm_data_abs8_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string& sym, int addend) const {
		return lyn::event_code(
			lyn::event_code::CODE_BYTE,
			arm_relocator::abs_reloc_string(sym, data.read_byte(offset) + addend),
			false
		);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		data.write_byte(offset, (data.read_byte(offset) + value + addend));
	}
};

struct arm_thumb_b_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data&, unsigned int, const std::string& sym, int addend) const {
		std::string value = arm_relocator::pcrel_reloc_string(sym, addend);

		return lyn::event_code(lyn::event_code::CODE_SHORT, {
			arm_relocator::b_string(value)
		}, false);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		std::uint32_t relocatedValue = (value + addend - offset - 4);

		data.write<std::uint16_t>(offset, (((relocatedValue>>1) & 0x7FF) | 0xE000));
	}

	bool is_absolute() const {
		return false;
	}

	bool can_make_trampoline() const {
		return true;
	}

	section_data make_trampoline(const std::string& symbol, int addend) const {
		return arm_relocator::make_thumb_veneer(symbol, addend);
	}
};

struct arm_thumb_bcond_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string& sym, int addend) const {
		std::string value  = arm_relocator::pcrel_reloc_string(sym, addend);

		return lyn::event_code(lyn::event_code::CODE_SHORT, {
			arm_relocator::bcond_string((data.read<std::uint16_t>(offset) & 0xFF00), value)
		}, false);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		std::uint32_t relocatedValue = (value + addend - offset - 4);

		data.write<std::uint16_t>(offset, (((relocatedValue>>1) & 0xFF) | (data.read<std::uint16_t>(offset) & 0xFF00)));
	}

	bool is_absolute() const {
		return false;
	}

	bool can_make_trampoline() const {
		return true;
	}

	section_data make_trampoline(const std::string& symbol, int addend) const {
		return arm_relocator::make_thumb_veneer(symbol, addend);
	}
};

struct arm_thumb_bl_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data&, unsigned int, const std::string& sym, int addend) const {
		std::string value = arm_relocator::pcrel_reloc_string(sym, addend);

		return lyn::event_code(lyn::event_code::CODE_SHORT, {
			arm_relocator::bl_op1_string(value),
			arm_relocator::bl_op2_string(value)
		}, false);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		std::uint32_t relocatedValue = (value + addend - offset - 4);

		data.write<std::uint16_t>(offset + 0, (((relocatedValue>>12) & 0x7FF) | 0xF000));
		data.write<std::uint16_t>(offset + 2, (((relocatedValue>>1)  & 0x7FF) | 0xF800));
	}

	bool is_absolute() const {
		return false;
	}

	bool can_make_trampoline() const {
		return true;
	}

	section_data make_trampoline(const std::string& symbol, int addend) const {
		return arm_relocator::make_thumb_veneer(symbol, addend);
	}
};

struct arm_arm_b_reloc : public arm_relocator::relocatelet {
	event_code make_event_code(const section_data& data, unsigned int offset, const std::string &sym, int addend) const {
		std::string value = arm_relocator::pcrel_reloc_string(sym, addend);

		return lyn::event_code(
			lyn::event_code::CODE_WORD,
			arm_relocator::b24_arm_string(data.read<uint32_t>(offset), value),
			false
		);
	}

	void apply_relocation(section_data& data, unsigned int offset, unsigned int value, int addend) const {
		std::uint32_t relocatedValue = (value + addend - offset - 8);

		data.write<std::uint32_t>(offset, (((relocatedValue>>2) & 0xFFFFFF) | (data.read<uint32_t>(offset) & 0xFF000000)));
	}

	bool is_absolute() const {
		return false;
	}
};

struct arm_arm_bl_reloc : public arm_arm_b_reloc {
	bool can_make_trampoline() const {
		return true;
	}

	section_data make_trampoline(const std::string& symbol, int addend) const {
		return arm_relocator::make_arm_veneer(symbol, addend);
	}
};

arm_relocator::arm_relocator() {
	mRelocatelets[elfcpp::R_ARM_ABS32].reset(new arm_data_abs32_reloc);  // R_ARM_ABS32
	mRelocatelets[elfcpp::R_ARM_REL32].reset(new arm_data_rel32_reloc);  // R_ARM_REL32
	mRelocatelets[elfcpp::R_ARM_ABS16].reset(new arm_data_abs16_reloc);  // R_ARM_ABS16
	mRelocatelets[elfcpp::R_ARM_ABS8].reset(new arm_data_abs8_reloc);   // R_ARM_ABS8
	mRelocatelets[elfcpp::R_ARM_THM_CALL].reset(new arm_thumb_bl_reloc);    // R_ARM_THM_CALL
	mRelocatelets[elfcpp::R_ARM_CALL].reset(new arm_arm_bl_reloc);      // R_ARM_CALL
	mRelocatelets[elfcpp::R_ARM_JUMP24].reset(new arm_arm_b_reloc);       // R_ARM_JUMP24
	mRelocatelets[elfcpp::R_ARM_THM_JUMP11].reset(new arm_thumb_b_reloc);     // R_ARM_THM_JUMP11
	mRelocatelets[elfcpp::R_ARM_THM_JUMP8].reset(new arm_thumb_bcond_reloc); // R_ARM_THM_JUMP8
}

const arm_relocator::relocatelet* arm_relocator::get_relocatelet(int relocationIndex) const {
	auto it = mRelocatelets.find(relocationIndex);

	if (it == mRelocatelets.end())
		return nullptr;

	return it->second.get();
}

std::string arm_relocator::abs_reloc_string(const std::string& symbol, int addend) {
	if (addend == 0)
		return symbol;

	std::string result;
	result.reserve(3 + 8 + symbol.size()); // 5 ("(-)") + 8 (addend literal int) + symbol string

	result.append(symbol);

	if (addend < 0)
		result.append("-").append(std::to_string(-addend));
	else
		result.append("+").append(std::to_string(addend));

	return result;
}

std::string arm_relocator::rel_reloc_string(const std::string& symbol, int addend) {
	if (addend == 0)
		return symbol;

	std::string result;
	result.reserve(17 + 8 + symbol.size()); // 17 ("(--CURRENTOFFSET)") + 8 (addend literal int) + symbol string

	result.append(symbol);

	if (addend < 0)
		result.append("-").append(std::to_string(-addend));
	else
		result.append("+").append(std::to_string(addend));

	result.append("-CURRENTOFFSET");
	return result;
}

std::string arm_relocator::pcrel_reloc_string(const std::string& symbol, int addend) {
	return rel_reloc_string(symbol, addend-4);
}

std::string arm_relocator::pcrel_arm_reloc_string(const std::string& symbol, int addend) {
	return rel_reloc_string(symbol, addend-8);
}

std::string arm_relocator::b24_arm_string(std::uint32_t base, const std::string& valueString) {
	std::string result;
	result.reserve(3 + valueString.size() + 15 + 8 + 1);

	base &= 0xFF000000;

	result.append("((");
	result.append(valueString);
	result.append(">>2)&$FFFFFF)|$");
	util::append_hex(result, base);

	return result;
}

std::string arm_relocator::b_string(const std::string& valueString) {
	std::string result;
	result.reserve(19 + valueString.size());

	result.append("((");
	result.append(valueString);
	result.append(">>1)&$7FF)|$E000");

	return result;
}

std::string arm_relocator::bcond_string(std::uint16_t base, const std::string& valueString) {
	std::string result;
	result.reserve(18 + valueString.size());

	result.append("((");
	result.append(valueString);
	result.append(">>1)&$FF)|$");
	util::append_hex(result, base);

	return result;
}

std::string arm_relocator::bl_op1_string(const std::string& valueString) {
	std::string result;
	result.reserve(20 + valueString.size());

	result.append("((");
	result.append(valueString);
	result.append(">>12)&$7FF)|$F000");

	return result;
}

std::string arm_relocator::bl_op2_string(const std::string& valueString) {
	std::string result;
	result.reserve(19 + valueString.size());

	result.append("((");
	result.append(valueString);
	result.append(">>1)&$7FF)|$F800");

	return result;
}

section_data arm_relocator::make_thumb_veneer(const std::string& symbol, int addend) {
	section_data result;

	result.resize(0x10);

	result.write<std::uint16_t>(0x00, 0x4778);     // bx pc
	result.write<std::uint16_t>(0x02, 0x46C0);     // nop
	result.write<std::uint32_t>(0x04, 0xE59FC000); // ldr ip, =target
	result.write<std::uint32_t>(0x08, 0xE12FFF1C); // bx ip
	result.write<std::uint32_t>(0x0C, 0);          // .word target

	result.set_mapping(0x00, section_data::mapping::Thumb);
	result.set_mapping(0x04, section_data::mapping::ARM);
	result.set_mapping(0x0C, section_data::mapping::Data);

	result.relocations().push_back({ symbol, addend, 0x02, 0x0C });

	return result;
}

section_data arm_relocator::make_arm_veneer(const std::string& symbol, int addend) {
	section_data result;

	result.resize(0x0C);

	result.write<std::uint32_t>(0x00, 0xE59FC000); // ldr ip, =target
	result.write<std::uint32_t>(0x04, 0xE12FFF1C); // bx ip
	result.write<std::uint32_t>(0x08, 0);          // .word target

	result.set_mapping(0x00, section_data::mapping::ARM);
	result.set_mapping(0x08, section_data::mapping::Data);

	result.relocations().push_back({ symbol, addend, 0x02, 0x08 });

	return result;
}

} // namespace lyn
