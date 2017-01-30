#pragma once


namespace sltbench {

#define SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(RequiredTypeName) \
	template<typename T> \
	struct has_inner_type_##RequiredTypeName \
	{ \
	private:\
		typedef char                      yes;\
		typedef struct { char array[2]; } no;\
		template<typename C> static yes test(typename C::RequiredTypeName*);\
		template<typename C> static no  test(...);\
	public:\
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);\
	};

SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(ArgType);
SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(Type);
SLTBENCH_PRIVATE_DEFINE_CHECKER_HAS_INNER_TYPEDEF(value_type);


template<typename T>
struct is_constructible_from_env
{
private:
	static constexpr int argc = 0;
	static constexpr char** argv = nullptr;

private:
	typedef char                      yes;
	typedef struct { char array[2]; } no;

	template<typename U> static yes test(decltype(U(argc, argv))*);
	template<typename U> static no  test(...);

public:
	static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
};


template<class F>
struct function_traits;

// function pointer
template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(Args...)>
{
	using return_t = R;

	static constexpr std::size_t arity = sizeof...(Args);

	template <std::size_t N>
	struct argument
	{
		static_assert(N < arity, "error: invalid parameter index.");
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};
};

// member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : public function_traits<R(C&, Args...)>
{};

// const member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> : public function_traits<R(C&, Args...)>
{};

// member object pointer
template<class C, class R>
struct function_traits<R(C::*)> : public function_traits<R(C&)>
{};

} // namespace sltbench
