
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <optional>

using u8 = std::uint8_t;

// adapted from https://github.com/pret/pokeemerald/blob/master/tools/gbagfx/lz.c
std::vector<u8> compress(std::vector<u8> const& src, int const min_distance = 2)
{
    int worst_cast_dest_size = 4 + src.size() + ((src.size() + 7) / 8);

    // Round up to the next multiple of four.
    worst_cast_dest_size = (worst_cast_dest_size + 3) & ~3;

    std::vector<u8> result(worst_cast_dest_size);

    // header
    result[0] = 0x10; // LZ compression type
    result[1] = (u8) (src.size());
    result[2] = (u8) (src.size() >> 8);
    result[3] = (u8) (src.size() >> 16);

    unsigned src_pos = 0;
    unsigned dst_pos = 4;

    while (true)
    {
        unsigned const flags_pos = dst_pos++;
        result[flags_pos] = 0;

        for (unsigned i = 0; i < 8; i++)
        {
            unsigned best_block_distance = 0;
            unsigned best_block_size = 0;
            unsigned block_distance = min_distance;

            while (block_distance <= src_pos && block_distance <= 0x1000)
            {
                unsigned const block_start = src_pos - block_distance;
                unsigned block_size = 0;

                while (block_size < 18
                    && src_pos + block_size < src.size()
                    && src[block_start + block_size] == src[src_pos + block_size])
                    block_size++;

                if (block_size > best_block_size)
                {
                    best_block_distance = block_distance;
                    best_block_size = block_size;

                    if (block_size == 18)
                        break;
                }

                block_distance++;
            }

            if (best_block_size >= 3)
            {
                result[flags_pos] |= (0x80 >> i);
                src_pos += best_block_size;
                best_block_size -= 3;
                best_block_distance--;
                result[dst_pos++] = (best_block_size << 4) | ((unsigned) best_block_distance >> 8);
                result[dst_pos++] = (u8) best_block_distance;
            }
            else
            {
                result[dst_pos++] = src[src_pos++];
            }

            if (src_pos == src.size())
            {
                // Pad to multiple of 4 bytes.
                unsigned const remainder = dst_pos % 4;

                if (remainder != 0)
                {
                    for (unsigned i = 0; i < 4 - remainder; i++)
                        result[dst_pos++] = 0;
                }

                result.resize(dst_pos);
                return result;
            }
        }
    }
}

static char const* const s_usage_string =
    "Usage:\n"
    "  compress INPUT OUTPUT\n"
    "  compress INPUT --to-stdout\n";

int main(int argc, char** argv)
{
    std::optional<std::string> opt_filename_input;
    std::optional<std::string> opt_filename_output;
    bool to_stdout = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg(argv[i]);

        if (arg == "--to-stdout")
        {
            to_stdout = true;
            continue;
        }

        if (!opt_filename_input)
        {
            opt_filename_input = std::string(arg);
            continue;
        }

        if (!opt_filename_output)
        {
            opt_filename_output = std::string(arg);
            continue;
        }

        std::cerr << "Too many arguments!" << std::endl;
        std::cerr << s_usage_string << std::endl;

        return 1;
    }

    if (!opt_filename_input || (!to_stdout && !opt_filename_output))
    {
        std::cerr << "Not enough arguments!" << std::endl;
        std::cerr << s_usage_string << std::endl;

        return 1;
    }

    std::ifstream file_input(*opt_filename_input, std::ios::in | std::ios::binary | std::ios::ate);

    if (!file_input.is_open())
    {
        std::cerr << "Couldn't open file for read: `" << *opt_filename_input << "`" << std::endl;
        return 2;
    }

    std::vector<u8> data_input(file_input.tellg());

    file_input.seekg(0, std::ios::beg);
    file_input.read(reinterpret_cast<char*>(data_input.data()), data_input.size());
    file_input.close();

    std::vector<u8> const data_output = compress(data_input);

    if (to_stdout)
    {
        std::cout.write(reinterpret_cast<char const*>(data_output.data()), data_output.size());
    }
    else
    {
        std::ofstream file_output(*opt_filename_output, std::ios::out | std::ios::binary);

        if (!file_output.is_open())
        {
            std::cerr << "Couldn't open file for write: `" << *opt_filename_output << "`" << std::endl;
            return 2;
        }

        file_output.write(reinterpret_cast<char const*>(data_output.data()), data_output.size());
        file_output.close();
    }

    return 0;
}
