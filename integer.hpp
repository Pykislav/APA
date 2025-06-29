#ifndef APA_INTEGER_HPP
#define APA_INTEGER_HPP

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <limits>
#include <vector>

#include "config.hpp"

#define PADDING(LENGTH, BLOCK) (LENGTH % BLOCK == 0) ? 0 : (BLOCK - (LENGTH % BLOCK))

namespace apa {

    const unsigned char HEX_TO_CHAR[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    const unsigned char CHAR_TO_HEX[127] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
        0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    /// binary - base2.
    constexpr static size_t BIN = 2;

    /// octal - base8.
    constexpr static size_t OCT = 8;

    /// decimal - base10.
    constexpr static size_t DEC = 10;

    /// hex - base16.
    constexpr static size_t HEX = 16;

    /// empty - invalid
    constexpr static size_t EMPTY = 1;

    /// invalid - invalid
    constexpr static size_t INVALID = 20;

    typedef std::vector<uint8_t> byte_array;

    constexpr static int LESS = -1;
    constexpr static int EQUAL = 0;
    constexpr static int GREAT = 1;

    constexpr static int BITS_PER_BYTE = 8;

    constexpr static size_t BASE_BITS = (sizeof(limb_t) * 8);
    constexpr static size_t BASE_BITS_MINUS1 = BASE_BITS - 1;
    constexpr static size_t BASE_BYTES = BASE_BITS / 8;

    constexpr static size_t LIMB_BITS = BASE_BITS;
    constexpr static size_t LIMB_BYTES = BASE_BYTES;

    constexpr static size_t CAST_BITS = BASE_BITS * 2;
    constexpr static size_t CAST_BYTES = BASE_BYTES * 2;

    constexpr static size_t INITIAL_LIMB_CAPACITY = 2;
    constexpr static size_t INITIAL_LIMB_LENGTH = 1;
    constexpr static size_t LIMB_GROWTH = 2;

    constexpr static limb_t BASE_MAX = std::numeric_limits<limb_t>::max();

    struct integer {
        size_t capacity;
        size_t length;
        limb_t *limbs;

        // Constructors
        integer() noexcept;
        integer(size_t num) noexcept;
        integer(const std::string &text, size_t base = 10);
        integer(std::initializer_list<limb_t> limbs);
        integer(size_t capacity, size_t length, bool AllocateSpace = true);
        integer(limb_t *arr, size_t capacity, size_t length) noexcept;

        // integer Constructors.
        integer(const integer &src);     // copy.
        integer(integer &&src) noexcept; // move.

        // integer Assignments.
        integer &operator=(const integer &src);     // copy.
        integer &operator=(integer &&src) noexcept; // move.

        ~integer();

        // Index Operator
        limb_t &operator[](size_t i) noexcept;
        limb_t &operator[](size_t i) const noexcept;

        /// @return returns; -1 : if less than, 0 : if equal, 1 : if greater than.
        int compare(const integer &with) const noexcept;

        // Relational Operators
        bool operator<(const integer &op) const noexcept;
        bool operator>(const integer &op) const noexcept;
        bool operator==(const integer &op) const noexcept;
        bool operator!=(const integer &op) const noexcept;
        bool operator<=(const integer &op) const noexcept;
        bool operator>=(const integer &op) const noexcept;

        void bit_realloc(const integer &op) noexcept;
        void remove_leading_zeros() noexcept ;

        // Logical Operators
        explicit operator bool() const noexcept;

        // Arithmetic Operators
        integer bit_division(const integer &op) const;
        static void div_mod(integer& q, integer& r, integer& dividen, const integer& divisor);

        integer &operator+=(const integer &op) noexcept;
        integer &operator-=(const integer &op) noexcept;
        integer &operator*=(const integer &op) noexcept;
        integer &operator/=(const integer &op);
        integer &operator%=(const integer &op);
        integer operator+(const integer &op) const noexcept;
        integer operator-(const integer &op) const noexcept;
        integer operator*(const integer &op) const noexcept;

        // pre-fix increment/decrement
        integer &operator++() noexcept;
        integer &operator--() noexcept;

        // post-fix increment/decrement
        integer operator++(int) noexcept;
        integer operator--(int) noexcept;

        // Shift Operators
        integer &operator<<=(size_t bits) noexcept;
        integer &operator>>=(size_t bits) noexcept;
        integer operator<<(size_t bits) const noexcept;
        integer operator>>(size_t bits) const noexcept;
        // for left shift (<<) with parameter integer type use the formula : x*2^k
        // for right shift (>>) with parameter integer type use the formula : x/2^k

        // Print Methods
        void printBin_spaced_out() const;

        /// @return returns a string that represent the value of a integer number in
        /// base 10 form or decimal.
        std::string to_base10_string() const;

        /// @return returns a string that represent the value of a integer number in
        /// base 16 form or hexadecimal.
        std::string to_base16_string() const;

        // Methods
        size_t byte_size() const noexcept;
        size_t bit_size() const noexcept;
        limb_t *detach();
    };

    // functions
    void swap(integer &a, integer &b);

    // IO Operators
    std::ostream &operator<<(std::ostream &out, const integer &num);
    std::istream &operator>>(std::istream &in, integer &num);

    // integer constants
    static const integer __INTEGER_ZERO = 0;
    static const integer __INTEGER_ONE = 1;
    static const integer __INTEGER_TWO = 2;
    static const integer __INTEGER_TEN = 10;
} // namespace apa
#endif
