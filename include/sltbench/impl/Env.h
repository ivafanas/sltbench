#pragma once


namespace sltbench {

class Env
{
public:
	static Env& Instance();

private:
	Env() noexcept = default;
	~Env() noexcept = default;

public:
	void SetArgs(int argc, char **argv);
	int GetArgc() const noexcept;
	char** GetArgv() const noexcept;

private:
	int argc_ = 0;
	char** argv_ = 0;
};

} // namespace sltbench
