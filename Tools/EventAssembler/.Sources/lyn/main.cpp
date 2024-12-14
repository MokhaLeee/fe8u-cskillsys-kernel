#include <iostream>
#include <fstream>
#include <cstring>

#include "config.h"

#include "core/event_object.h"
#include "util/hex_write.h"

void print_usage(std::ostream& out)
{
	out << PROJECT_NAME " " PROJECT_VERSION " usage:" << std::endl;
	out << "  lyn <object>... [-[no]link] [-[no]longcalls] [-[no]temp] [-[no]hook] [-raw]" << std::endl;
	out << "  lyn diff <old object> <new object>" << std::endl;
}

int do_diff(int argc, const char* const* argv)
{
   if (argc != 2)
   {
	   print_usage(std::cerr);
	   return 1;
   }

   try
   {
	   lyn::event_object baseObject;
	   lyn::event_object otherObject;

	   baseObject.append_from_elf(argv[0]);
	   otherObject.append_from_elf(argv[1]);

	   struct sym_diff_data
	   {
		   std::string baseName;
		   std::string otherName;
	   };

	   std::map<unsigned, sym_diff_data> symMap;

	   for (auto& sym : baseObject.absolute_symbols())
		   symMap[sym.offset].baseName = sym.name;

	   for (auto& sym : otherObject.absolute_symbols())
		   symMap[sym.offset].otherName = sym.name;

	   for (auto& pair : symMap)
	   {
		   auto  address  = pair.first;
		   auto& nameDiff = pair.second;

		   if (nameDiff.baseName == nameDiff.otherName)
			   continue;

		   if (nameDiff.baseName.empty())
			   std::cout << "+" << util::make_hex_string("$", address) << " " << nameDiff.otherName << std::endl;

		   else if (nameDiff.otherName.empty())
			   std::cout << "-" << util::make_hex_string("$", address) << " " << nameDiff.baseName << std::endl;

		   else
			   std::cout << ">" << util::make_hex_string("$", address) << " " << nameDiff.baseName << " " << nameDiff.otherName << std::endl;
	   }
   }
   catch (const std::exception& e)
   {
	   std::cerr << "[lyn diff] ERROR: " << e.what() << std::endl;
	   return 1;
   }

   return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		print_usage(std::cerr);
		return 1;
	}

	if (!std::strcmp(argv[1], "diff"))
		return do_diff(argc - 2, argv + 2);

	struct
	{
		bool doLink          = true;
		bool longCall        = false;
		bool applyHooks      = true;
		bool printTemporary  = false;
	} options;

	std::vector<std::string> elves;

	for (int i = 1; i < argc; ++i)
	{
		std::string argument(argv[i]);

		if (argument.size() == 0)
			continue;

		if (argument[0] == '-') // option
		{
			if (argument  == "-nolink")
			{
				options.doLink = false;
				continue;
			}

			if (argument == "-link")
			{
				options.doLink = true;
				continue;
			}

			if (argument == "-longcalls")
			{
				options.longCall = true;
				continue;
			}

			if (argument == "-nolongcalls")
			{
				options.longCall = false;
				continue;
			}

			if (argument == "-raw")
			{
				options.doLink = false;
				options.longCall = false;
				options.applyHooks = false;
				continue;
			}

			if (argument == "-temp")
			{
				options.printTemporary = true;
				continue;
			}

			if (argument == "-notemp")
			{
				options.printTemporary = false;
				continue;
			}

			if (argument == "-hook")
			{
				options.applyHooks = true;
				continue;
			}

			if (argument == "-nohook")
			{
				options.applyHooks = false;
				continue;
			}
		} else { // elf
			elves.push_back(std::move(argument));
		}
	}

	try
	{
		lyn::event_object object;

		for (auto& elf : elves)
			object.append_from_elf(elf.c_str());

		if (options.doLink)
			object.try_relocate_relatives();

		if (options.longCall)
			object.try_transform_relatives();

		if (options.doLink)
			object.try_relocate_absolutes();

		if (!options.printTemporary)
			object.remove_unnecessary_symbols();

		object.cleanup();

		if (options.applyHooks)
		{
			for (auto& hook : object.get_hooks())
			{
				lyn::event_object temp;

				std::cout << "PUSH" << std::endl;
				std::cout << "ORG $" << std::hex << (hook.originalOffset & (~1)) << std::endl;

				temp.add_section(lyn::arm_relocator::make_thumb_veneer(hook.name, 0));
				temp.write_events(std::cout);

				std::cout << "POP" << std::endl;
			}
		}

		object.write_events(std::cout);
	}
	catch (const std::exception& e)
	{
		std::cerr << "[lyn] ERROR: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
