#ifndef _fn_hpp_
#define _fn_hpp_

#include "process.hpp"
#include "arg_positions.hpp"
#include "function_traits.hpp"
#include "pointer_to_member.hpp"
#include <tuple>
#include <type_traits>

template<typename F, typename T>
struct fn_t {

   fn_t(F&& f) 
   : _f(std::move(f)) {
   }

   fn_t(F&& f, T&& t) 
   : _f(std::move(f)), _t(std::move(t)) {
   }

   template<typename ...A>
   auto operator()(A&&... a) const& {
      return process(F(_f), T(_t), std::forward<A>(a)...);
   }

   template<typename ...A>
   auto operator()(A&&... a) && {
      return process(std::forward<F>(_f), std::forward<T>(_t), std::forward<A>(a)...);
   }

private:
   F _f;
   T _t;
};

template<typename F>
auto fn(F&& f) {
   return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f));
}

template<typename F, typename ...A>
auto fn(F&& f, A&& ...a) {
   return fn_t<F, typename arg_positions<function_traits<F>::arity>::type>(std::forward<F>(f))(std::forward<A>(a)...);
}

template<typename R, typename T, typename ...A>
auto fn(R T::* const f, A&& ...a) {
   using F = pointer_to_member<R, T, R T::*>;
   return fn(F(f), std::forward<A>(a)...);
}

#endif//_fn_hpp_
