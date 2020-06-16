#pragma once

#include <type_traits>

namespace TemplateUtils
{
    template<typename T, typename... Rest> struct IsAnyOfTrait : std::false_type {};

    template<typename T, typename First>
    struct IsAnyOfTrait<T, First> : std::is_same<T, First> {};

    template<typename T, typename First, typename... Rest>
    struct IsAnyOfTrait<T, First, Rest...> : std::integral_constant<bool, std::is_same<T, First>::value || IsAnyOfTrait<T, Rest...>::value> {};

    template<typename T, typename... Rest> struct CanCastToAnyOfTrait : std::false_type {};

    template<typename T, typename First>
    struct CanCastToAnyOfTrait<T, First> : std::is_convertible<T, First> {};

    template<typename T, typename First, typename... Rest>
    struct CanCastToAnyOfTrait<T, First, Rest...> : std::integral_constant<bool, std::is_convertible<T, First>::value || CanCastToAnyOfTrait<T, Rest...>::value> {};

    template<std::size_t ...>
    struct AddAllTrait : std::integral_constant< std::size_t, 0 > {};

    template<std::size_t X, std::size_t ... Xs>
    struct AddAllTrait<X, Xs...> : std::integral_constant< std::size_t, X + AddAllTrait<Xs...>::value > {};


#define ENABLE_IF(...)                  typename = std::enable_if<__VA_ARGS__>
#define IS_REAL(NUM)                    std::is_floating_point<NUM>::value
#define IS_INT(NUM)                     std::is_integral<NUM>::value
#define IS_CLASS(C)                     std::is_class<C>::value
#define IS_ENUM(E)                      std::is_enum<E>::value
#define IS_FUNCTION(F)                  std::is_function<F>::value
#define IS_REFERENCE(R)                 std::is_reference<R>::value
#define IS_POINTER(P)                   std::is_pointer<P>::value
#define IS_ARRAY(A)                     std::is_array<A>::value
#define IS_BASE_OF(BASE, DERIVED)       std::is_base_of<BASE, DERIVED>::value
#define ARE_BASE_OF(Ts, BASE)           std::conjunction_v<std::is_base_of<BASE, Ts>...>
#define IS_SAME(A, B)                   std::is_same<A, B>::value
#define ARE_SAME(T, Ts)                 std::conjunction_v<std::is_same<T, Ts>...>
#define IS_ANY_OF(T, ...)               Framework::TemplateUtils::IsAnyOfTrait<T, __VA_ARGS__>::value
#define CAN_CAST(FROM, TO)              std::is_convertible<FROM, TO>::value
#define CAN_CAST_TO_ANY_OF(FROM, ...)   Framework::TemplateUtils::CanCastToAnyOfTrait<FROM, __VA_ARGS__>::value
#define IF(CONDITION, THEN, ELSE)       std::conditional<CONDITION, THEN, ELSE>::type
#define VARIADIC_SIZE(Ts)               Framework::TemplateUtils::AddAllTrait< sizeof(Ts)... >::value
#define VARIADIC_COUNT(Ts)              std::tuple_size<std::tuple<Ts...>>::value
#define VARIADIC_TYPE_AT(Ts, N)         typename Framework::TemplateUtils::TypeList<Ts...>::type<N>
}