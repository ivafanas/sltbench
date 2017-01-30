#pragma once

#include <stdexcept>


namespace sltbench {

class StopGenerationException
	: public std::exception
{
public:
	StopGenerationException() noexcept
	{}

public:
	virtual const char * what() const noexcept override
	{
		return "generation was stopped";
	}
};

} // namespace sltbench
