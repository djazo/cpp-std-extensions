#pragma once

#if __cplusplus >= 202002L

#include <stdx/compiler.hpp>
#include <stdx/ct_string.hpp>
#include <stdx/type_traits.hpp>

#include <boost/mp11/algorithm.hpp>

namespace stdx {
inline namespace v1 {
namespace _env {
template <auto Query, auto Value> struct ct_prop {
    [[nodiscard]] CONSTEVAL static auto query(decltype(Query)) noexcept {
        return Value;
    }
};

template <typename Q, typename Env>
concept valid_query_for = requires(Env const &e) { e.query(Q{}); };

template <typename Q, typename... Envs>
concept valid_query_over = (... or valid_query_for<Q, Envs>);

template <typename Q> struct has_query {
    template <typename Env>
    using fn = std::bool_constant<valid_query_for<Q, Env>>;
};
} // namespace _env

template <typename... Envs> struct env {
    template <_env::valid_query_over<Envs...> Q>
    CONSTEVAL static auto query(Q) noexcept {
        using I = boost::mp11::mp_find_if_q<boost::mp11::mp_list<Envs...>,
                                            _env::has_query<Q>>;
        using E = boost::mp11::mp_at<boost::mp11::mp_list<Envs...>, I>;
        return Q{}(E{});
    }
};

template <typename T>
concept envlike = is_specialization_of_v<T, env>;

namespace _env {
template <typename T> struct autowrap {
    // NOLINTNEXTLINE(google-explicit-constructor)
    CONSTEVAL autowrap(T t) : value(t) {}
    T value;
};

// NOLINTNEXTLINE(modernize-avoid-c-arrays)
template <std::size_t N> using str_lit_t = char const (&)[N];

template <std::size_t N> struct autowrap<str_lit_t<N>> {
    // NOLINTNEXTLINE(google-explicit-constructor)
    CONSTEVAL autowrap(str_lit_t<N> str) : value(str) {}
    stdx::ct_string<N> value;
};

template <typename T> autowrap(T) -> autowrap<T>;
template <std::size_t N> autowrap(str_lit_t<N>) -> autowrap<str_lit_t<N>>;

template <auto V> struct wrap {
    constexpr static auto value = V;
};

template <typename> struct for_each_pair;
template <std::size_t... Is> struct for_each_pair<std::index_sequence<Is...>> {
    template <auto... Args>
    using type = env<
        _env::ct_prop<boost::mp11::mp_at_c<boost::mp11::mp_list<wrap<Args>...>,
                                           2 * Is>::value.value,
                      boost::mp11::mp_at_c<boost::mp11::mp_list<wrap<Args>...>,
                                           (2 * Is) + 1>::value.value>...>;
};

template <envlike Env = env<>>
constexpr auto make_env = []<autowrap... Args> {
    using new_env_t = typename for_each_pair<
        std::make_index_sequence<sizeof...(Args) / 2>>::template type<Args...>;
    return boost::mp11::mp_append<new_env_t, Env>{};
};
} // namespace _env

template <envlike Env, _env::autowrap... Args>
using extend_env_t =
    decltype(_env::make_env<Env>.template operator()<Args...>());

template <envlike... Envs>
using append_env_t = boost::mp11::mp_reverse<boost::mp11::mp_append<Envs...>>;

template <_env::autowrap... Args>
using make_env_t = extend_env_t<env<>, Args...>;
} // namespace v1
} // namespace stdx

#endif
