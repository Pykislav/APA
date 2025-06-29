#ifndef APA_MATH_CPP
#define APA_MATH_CPP

#include "math.hpp"

namespace apa {
    template <class T>
    T fibonacci_template(size_t nth, T b0, T b1) {
        T base0 = b0, base1 = b1;
        T nthfib = 0;
        for (size_t i = 2; i < nth; ++i) {
            nthfib = base0 + base1;
            base0 = std::move(base1);
            base1 = std::move(nthfib);
        }
        return base0 + base1;
    }

    template <class T>
    T factorial_template(size_t n) {
        T factorial(1);
        T iter = factorial;

        for (size_t i = 1; i <= n; ++i) {
            factorial *= iter;
            iter++;
        }

        return factorial;
    }

    integer fibonacci_integer(size_t nth, integer b0, integer b1) {
        if (nth < 2) {
            return __INTEGER_ONE;
        }
        return fibonacci_template<integer>(nth, b0, b1);
    }

    bint fibonacci(size_t nth, bint b0, bint b1) {
        if (nth < 2) {
            return __BINT_ONE;
        }
        return fibonacci_template<bint>(nth, b0, b1);
    }

    integer factorial_integer(size_t n) {
        return factorial_template<integer>(n);
    }

    bint factorial(size_t n) {
        return factorial_template<bint>(n);
    }

} // namespace apa

#endif