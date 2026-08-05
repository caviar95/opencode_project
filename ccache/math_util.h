#ifndef MATH_UTIL_H
#define MATH_UTIL_H
constexpr int factorial(int n) { return (n <= 1) ? 1 : n * factorial(n - 1); }
#endif
