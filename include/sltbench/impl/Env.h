#pragma once


namespace sltbench {

class Env
{
public:
	static Env& Instance();

private:
	Env() {}
	~Env() {}

public:
	void SetArgs(int argc, char **argv);
	int GetArgc() const;
	char** GetArgv() const;

private:
	int argc_ = 0;
	char** argv_ = 0;
};

} // namespace sltbench
