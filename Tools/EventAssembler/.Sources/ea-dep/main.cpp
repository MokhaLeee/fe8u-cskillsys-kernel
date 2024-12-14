#include <iostream>

#include "parser.h"
#include "file-utils.h"

#include <fstream>
#include <sstream>

#include <unordered_set>

struct FileList
{
	bool has_file(const std::string& file)
	{
		return files.count(file);
	}

	void add_file(const std::string& file)
	{
		files.insert(file);
		orderedFiles.push_back(file);
	}

	std::unordered_set<std::string> files;
	std::vector<std::string> orderedFiles;
};

struct Config
{
	std::vector<std::string> externalPaths;
	bool addExternalFiles = false;
	bool addMissingFiles = false;
};

template<typename ActionFunc>
void handle_fileexpr(FileList& filelist, const std::string& wd, const std::string& fileexpr, const Config& config, ActionFunc func)
{
	// Check neighbor file

	auto locfile = combine_path(wd, fileexpr);

	resolve_dotdots(locfile);

	if (filelist.has_file(locfile))
		return;

	if (file_exists(locfile))
	{
		filelist.add_file(locfile);
		func(locfile);

		return;
	}

	// End here if neither option is set (small optimization)
	if (!config.addMissingFiles && !config.addExternalFiles)
		return;

	// Check external files

	for (auto& path : config.externalPaths)
	{
		const auto extfile = combine_path(path, fileexpr);

		if (filelist.has_file(extfile))
			return;

		if (file_exists(extfile))
		{
			if (config.addExternalFiles)
			{
				filelist.add_file(extfile);
				func(extfile);
			}

			return;
		}
	}

	// Add missing neighbor file to list if addMissingFiles enabled

	if (config.addMissingFiles)
		filelist.add_file(locfile);
}

void list_depends(FileList& filelist, const std::string& filename, const Config& config)
{
	// List includes
	const auto includes = parse_includes(file_read_content(filename));

	const auto wd = get_directory(filename);

	// Add incbin files and do nothing with them

	for (auto& fileexpr : includes.incbinFiles)
		handle_fileexpr(filelist, wd, normalized(fileexpr), config, [&] (const auto&) {});

	// Add include files and recursively add the files that they are including

	for (auto& fileexpr : includes.includeFiles)
	{
		handle_fileexpr(filelist, wd, normalized(fileexpr), config, [&] (const auto& fullpath)
		{
			list_depends(filelist, fullpath, config);
		});
	}
}

FileList list_depends(const std::string& filename, const Config& config)
{
	FileList list;
	list_depends(list, filename, config);

	return list;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "ea-dep 1.2 - include dependency analyzer for EA scripts" << std::endl;
		std::cerr << "Usage: " << argv[0] << " INPUT [-I PATH]... [--add-missings] [--add-externals]" << std::endl;
		return 0;
	}

	auto it = argv + 1;
	const auto end = argv + argc;

	Config config;
	std::string input;

	while (it != end)
	{
		std::string str(*it);

		if (str == "-I")
		{
			if ((it + 1) == end)
			{
				std::cerr << "ERROR: -I without parameter!" << std::endl;
				return 1;
			}

			config.externalPaths.emplace_back(it[1]);
			it += 2;
		}

		else if (str == "--add-missings")
		{
			config.addMissingFiles = true;
			++it;
		}

		else if (str == "--add-externals")
		{
			config.addExternalFiles = true;
			++it;
		}

		else
		{
			if (!input.empty())
			{
				std::cerr << "ERROR: multiple input files!" << std::endl;
				return 2;
			}

			input = std::move(normalize(str));
			it++;
		}
	}

	if (input.empty())
	{
		std::cerr << "ERROR: no input file!" << std::endl;
		return 3;
	}

	if (!file_exists(input))
	{
		std::cerr << "ERROR: file `" << input << "` doesn't seem to exist!" << std::endl;
		return 4;
	}

	for (auto& inc : list_depends(normalized(input), config).orderedFiles)
		std::cout << inc << std::endl;

	return 0;
}
