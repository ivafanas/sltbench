#include <sltbench/impl/Env.h>

#include <cassert>


namespace sltbench {

Env& Env::Instance()
{
	static Env inst;
	return inst;
}

void Env::SetArgs(const int argc, char **argv)
{
	argc_ = argc;
	argv_ = argv;
}

int Env::GetArgc() const
{
	assert(argc_ != 0);
	return argc_;
}

char **Env::GetArgv() const
{
	assert(argv_ != 0);
	return argv_;
}
} // namespace sltbench
