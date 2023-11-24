﻿#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <string>

std::array<std::uint_fast32_t, 256> generate_crc_lookup_table() noexcept
{
	auto const reversed_polynomial = std::uint_fast32_t{ 0xEDB88320uL };
	struct byte_checksum
	{
		std::uint_fast32_t operator()() noexcept
		{
			auto checksum = static_cast<std::uint_fast32_t>(n++);

			for (auto i = 0; i < 8; ++i)
				checksum = (checksum >> 1) ^ ((checksum & 0x1u) ? reversed_polynomial : 0);

			return checksum;
		}

		unsigned n = 0;
	};

	auto table = std::array<std::uint_fast32_t, 256>{};
	std::generate(table.begin(), table.end(), byte_checksum{});

	return table;
}

template <typename InputIterator>
std::uint_fast32_t crc(InputIterator first, InputIterator last)
{

	static auto const table = generate_crc_lookup_table();

	return std::uint_fast32_t{ 0xFFFFFFFFuL } &
		~std::accumulate(first, last,
			~std::uint_fast32_t{ 0 } &std::uint_fast32_t{ 0xFFFFFFFFuL },
			[](std::uint_fast32_t checksum, std::uint_fast8_t value)
			{ return table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8); });
}

int main()
{
	auto const s = std::string{ "fhghgfhgfythgffhytfh" };

	std::cout << std::hex << std::setw(8) << std::setfill('0') << crc(s.begin(), s.end()) << '\n';
}