#pragma once

#include <iostream>


#define SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(RequiredTypeName) \
	template<typename T> \
	struct SLT_HasInnerTypedef_##RequiredTypeName \
	{ \
	private:\
		typedef char                      yes;\
		typedef struct { char array[2]; } no;\
		template<typename C> static yes test(typename C::RequiredTypeName*);\
		template<typename C> static no  test(...);\
	public:\
		static const bool value = sizeof(test<T>(0)) == sizeof(yes);\
	};


namespace sltbench {

template<typename T>
struct SLT_HasInsertionOperator
{
private:
	typedef char                      yes;
	typedef struct { char array[2]; } no;

	template<typename U> static yes test(U&);
	template<typename U> static no  test(...);

	static std::ostream &s;
	static T const &t;

public:
	static bool const value = sizeof(test(s << t)) == sizeof(yes);
};

} // namespace sltbench


namespace sltbench {

SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(ArgType);
SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(Type);
SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(value_type);

} // namespace sltbench

