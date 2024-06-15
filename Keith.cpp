#include "stdafx.h"

#ifdef WIN32
typedef unsigned long int uintmax_fast;
#else 
typedef unsigned long long int uintmax_fast;
#endif

bool testKeith(uintmax_fast candidate)
{
	assert(candidate);
	decltype(candidate) buff[std::numeric_limits<decltype(candidate)>::digits10];
	auto *const stop{ std::end(buff) };
	auto *ptr{ stop - 1 };
	for (auto digit_seq{ candidate }; digit_seq; digit_seq /= 10)
		*ptr-- = digit_seq % 10;
	++ptr;
	auto const len{ stop - ptr };
	std::make_unsigned_t<std::decay_t<decltype(len)>> index{ 0 };
	auto sum{ std::accumulate(ptr, stop, decltype(candidate){0}) };
	while (sum < candidate)
	{
		//swap sum into ptr[index], AND into term in expression for sum representing ptr[index]
		sum += /*add new term*/sum - /*remove old term*/std::exchange(ptr[index], sum);
		index = (index + 1) % len;
	}
	return candidate == sum;
}

template<typename base_t, typename exp_t>
constexpr auto ct_pow(base_t base, exp_t exp, base_t retval = 1)->base_t
{
	static_assert(std::is_integral_v<exp_t> && std::is_unsigned_v<exp_t>, "Cannot calculate exponent at compile-time");
	return exp ? ct_pow(base, exp - 1, retval * base) : retval;
}

//Finds and prints Keith numbers
//c.f. https://en.wikipedia.org/wiki/Keith_number
int _cdecl main(void)
{
	//Heuristics suggest that there should be ~3 Keith #s/decade; add a few more for safety
	uintmax_fast keiths[3 * std::numeric_limits<uintmax_fast>::digits10 + 50];
	constexpr uintmax_fast const max_digits{ 10 }; //Inclusive
	constexpr auto const stop{ ct_pow(uintmax_fast{10u}, max_digits) };
	auto *index{ keiths };
	for (uintmax_fast curr{ 1 }; curr < stop; ++curr)
		if (testKeith(curr))
			*index++ = curr;
	if (index - keiths)
	{
		std::cout << (index - keiths) << " Keith #s found:" << std::endl;
		auto const *jndex{ keiths };
		std::cout << *jndex++;
		while(jndex < index)
			std::cout << ", " << *jndex++;
		std::cout << std::endl;
	}
	else std::cout << "No Keiths found!" << std::endl;
	std::system("pause");
}