#ifndef FILE_UTILS_INCLUDED
#define FILE_UTILS_INCLUDED

#include <string>
#include <fstream>
#include <sstream>

// TODO (C++17): namespace fs = std::filesystem;
namespace fs { using path = std::string; }

inline
std::string file_read_content(const fs::path& filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);

	if (in)
	{
		std::ostringstream contents;

		contents << in.rdbuf() << "\n";
		in.close();

		return contents.str();
	}

	return {};
}

inline
bool file_exists(const fs::path& name)
{
	// https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c

	if (std::FILE* file = std::fopen(name.c_str(), "r"))
	{
		std::fclose(file);
		return true;
	}

	return false;
}

inline
fs::path& normalize(fs::path& p)
{
	fs::path::size_type idx = 0;

	while ((idx = p.find('\\', idx)) != fs::path::npos)
		p[idx] = '/';

	return p;
}

inline
fs::path normalized(const fs::path& p)
{
	fs::path result(p);
	normalize(result);

	return result;
}

inline
fs::path get_directory(const fs::path& path)
{
	const auto idx = path.find_last_of('/');

	if (idx != fs::path::npos)
		return path.substr(0, idx);

	return {};
}

inline
fs::path combine_path(const fs::path& left, const fs::path& right)
{
	if (left.empty())
		return right;

	fs::path result;
	result.reserve(left.size() + right.size() + 1);

	result.append(left);

	if (left.back() != '/' && left.back() != '\\')
		result.push_back('/');

	result.append(right);

	return result;
}

inline
fs::path& resolve_dotdots(fs::path& p)
{
	// expects normalized path

	fs::path::size_type beg = 0;
	fs::path::size_type sep = 0;

	while ((sep = p.find('/', beg)) != fs::path::npos)
	{
		int cmp = p.compare(sep + 1, 3, "../");

		if (cmp == 0)
		{
			p.erase(beg, (sep - beg) + 4);

			// rapply the algorithm recursively
			return resolve_dotdots(p);
		}

		beg = sep + 1;
	}

	return p;
}

#endif // FILE_UTILS_INCLUDED
