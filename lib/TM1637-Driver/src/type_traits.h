#ifndef TM1637_TYPE_TRAITS_H
#define TM1637_TYPE_TRAITS_H

#include <Arduino.h>

namespace type_traits
{
    template <typename T, T v>
    struct integral_constant
    {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;


    template <typename T>
    struct remove_reference
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <bool B, class T, class F>
    struct conditional
    {
        using type = T;
    };

    template <class T, class F>
    struct conditional<false, T, F>
    {
        using type = F;
    };

    template <class T>
    struct is_array: false_type
    {};

    template <class T>
    struct is_array<T[]>: true_type
    {};

    template <class T, size_t N>
    struct is_array<T[N]>: true_type
    {};

    template <class T>
    struct remove_extent
    {
        using type = T;
    };

    template <class T>
    struct remove_extent<T[]>
    {
        using type = T;
    };

    template <class T, size_t N>
    struct remove_extent<T[N]>
    {
        using type = T;
    };

    namespace detail
    {
        template <class T, bool is_function_type = false>
        struct add_pointer
        {
            using type = typename remove_reference<T>::type*;
        };

        template <class T>
        struct add_pointer<T, true>
        {
            using type = T;
        };

        template <class T, class ... Args>
        struct add_pointer<T(Args...), true>
        {
            using type = T(*)(Args...);
        };

        template <class T, class ... Args>
        struct add_pointer<T(Args..., ...), true>
        {
            using type = T(*)(Args..., ...);
        };
    }

    template<class>
    struct is_function: false_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...)>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......)>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile &>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile &>: true_type
    {};
    
    template<class Ret, class... Args>
    struct is_function<Ret(Args...) &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) volatile &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args...) const volatile &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) volatile &&>: true_type
    {};

    template<class Ret, class... Args>
    struct is_function<Ret(Args......) const volatile &&>: true_type
    {};

    template <class T>
    struct add_pointer: detail::add_pointer<T, is_function<T>::value>
    {};


    template <class T>
    struct remove_const
    {
        using type = T;
    };

    template <class T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <class T>
    struct remove_volatile
    {
        using type = T;
    };

    template <class T>
    struct remove_volatile<volatile T>
    {
        using type = T;
    };

    template <class T>
    struct remove_cv
    {
        using type = typename remove_volatile<typename remove_const<T>::type>::type;
    };


    template <class T>
    struct decay
    {
    private:
        using U = typename remove_reference<T>::type;

    public:
        using type = typename conditional<
                is_array<U>::value, typename remove_extent<U>::type*, typename conditional<
                        is_function<U>::value, typename add_pointer<U>::type, typename remove_cv<U>::type
                        >::type
                    >::type ;
     };

    template <typename T>
    struct is_integral_base: false_type
    {};

    template <>
    struct is_integral_base<char> : true_type
    {};

    template <>
    struct is_integral_base<short>: true_type
    {};

    template <>
    struct is_integral_base<int>: true_type
    {};

    template <>
    struct is_integral_base<long>: true_type
    {};

    template <>
    struct is_integral_base<unsigned char>: true_type
    {};

    template <>
    struct is_integral_base<unsigned short>: true_type
    {};

    template <>
    struct is_integral_base<unsigned int>: true_type
    {};

    template <>
    struct is_integral_base<unsigned long>: true_type
    {};

    template <typename T>
    struct is_integral: is_integral_base<typename decay<T>::type>
    {};

    template <typename T>
    struct is_floating_point_base: false_type
    {};

    template <>
    struct is_floating_point_base<float>: true_type
    {};

    template <>
    struct is_floating_point_base<double>: true_type
    {};

    template <>
    struct is_floating_point_base<long double>: true_type
    {};

    template <typename T>
    struct is_floating_point : is_floating_point_base<typename decay<T>::type>
    {};

    template <typename T>
    struct is_string_base: false_type
    {};

    template <>
    struct is_string_base<const char*>: true_type
    {};

    template <>
    struct is_string_base<String>: true_type
    {};

    template <typename T>
    struct is_string: is_string_base<typename remove_reference<T>::type >
    {};

    template <bool B, class T = void>
    struct enable_if
    {};

    template <class T>
    struct enable_if <true, T>
    {
        using type = T;
    };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template <typename>
    struct is_lvalue_reference: public false_type
    {};

    template <typename T>
    struct is_lvalue_reference<T&>: public true_type
    {};

    template <typename>
    struct is_rvalue_reference: public false_type
    {};

    template <typename T>
    struct is_rvalue_reference<T&&>: public true_type
    {};

    template <class ...>
    struct disjunction: false_type
    {};

    template <class T>
    struct disjunction<T>: T
    {};

    template <class T, class ... Ts>
    struct disjunction<T, Ts ...>: conditional<bool(T::value), T, disjunction<Ts...>>
    {};
}

#endif //TM1637_TYPE_TRAITS_H
