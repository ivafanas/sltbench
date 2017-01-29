#include <sltbench/impl/Verdict.h>

#include <iostream>


namespace sltbench {

std::string ToString(Verdict verdict)
{
	switch (verdict)
	{
	case Verdict::OK: return "ok";
	case Verdict::CRASHED: return "CRASHED";
	case Verdict::IMPRECISE: return "IMPRECISE";
	}
	return "UNKNOWN";
}

} // namespace sltbench
