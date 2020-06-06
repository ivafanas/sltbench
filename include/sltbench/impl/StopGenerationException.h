#pragma once

#include <exception>


namespace sltbench {

class StopGenerationException
	: public std::exception
{
public:
	const char * what() const noexcept override
	{
		return "generation was stopped";
	}
};

} // namespace sltbench
