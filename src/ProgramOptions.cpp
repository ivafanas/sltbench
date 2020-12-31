#include "ProgramOptions.h"

#include <algorithm>
#include <cstring>
#include <utility>


namespace {

enum class Type
{
	Key,       // --option
	KeyValue,  // --option value
	Value,     // value (having --option before, other kind of values not parsed)
	Incorrect  // failed to parse
};

struct ArgAndType
{
	const char* arg;
	Type type;
};

}  // namespace

// yep, this function doesn't work well with incorrect user input
static Type DetectArgType(const char* const s)
{
	const int len = static_cast<int>(std::strlen(s));

	if (len == 0)
		return Type::Incorrect;

	// --option=value
	// Assume that '=' symbol should not appear in another values.
	// No checks against letters in "option" part.
	const char* it = std::find(s, s + len, '=');
	if (it != s + len)
	{
		const int eq_pos = static_cast<int>(std::distance(s, it));
		return
			len > 3 &&
			s[0] == '-' && s[1] == '-' &&
			eq_pos > 2 && eq_pos + 1 != len
			? Type::KeyValue
			: Type::Incorrect;
	}

	// --option
	// No check against letters in "option" part
	if (len >= 3 && s[0] == '-' && s[1] == '-' && s[2] != '-')
		return Type::Key;

	// -o is not supported
	if (s[0] == '-')
		return Type::Incorrect;

	// oh!
	return Type::Value;
}

namespace {

struct KeyAndValue
{
	std::string key;
	std::string val;
};

}  // namespace

static KeyAndValue GetKeyAndValue(const char* const s)
{
	// precondition: |s| is like "--option=value".
	const auto len = std::strlen(s);
	const char* it = std::find(s, s + len, '=');

	if (len <= 3 || it == s + len || s[0] != '-' || s[1] != '-')
		return {};  // should never happen

	return {std::string(s, it), std::string(it + 1, s + len)};
}


namespace sltbench {

ProgramOptions ParseCommandLine(int argc, char **argv)
{
	// boost::program_options would be the best solution,
	// but dependency on boost is not so good

	// well, this algorithm will be neither fast neither correct,
	// but correct enough for our case.

	// NOTE: parsing contains several drawbacks. The mose important is ignoring
	// of incorrect user input. It would be nice to report mistakes in user
	// input and terminate program execution. But it would change the program
	// behavior: incorrect input would start to lead program to fail.
	//
	// It is possible to rework parsing and change behavior only with major
	// version update.

	// detect arguments types
	std::vector<ArgAndType> args;
	args.reserve(argc);
	for (int i = 1; i < argc; ++i)
		args.push_back({argv[i], DetectArgType(argv[i])});

	// shortcut to ensure that there is value-like argument by index
	const auto is_value_at = [&args](const size_t ix) {
		return ix < args.size() && args[ix].type == Type::Value;
	};

	ProgramOptions rv;
	for (size_t i = 0, count = args.size(); i < count; ++i)
	{
		if (args[i].type == Type::KeyValue)
		{
			auto res = GetKeyAndValue(args[i].arg);
			rv.named_values.emplace(std::move(res.key), std::move(res.val));
			continue;
		}
		if (args[i].type == Type::Key && is_value_at(i + 1))
		{
			rv.named_values.emplace(args[i].arg, args[i + 1].arg);
			continue;
		}
		if (args[i].type == Type::Key && !is_value_at(i + 1))
		{
			rv.switches.emplace_back(args[i].arg);
			continue;
		}
	}
	return rv;
}

} // namespace sltbench

