#ifndef APA_INTEGER_CPP
#define APA_INTEGER_CPP

#include <cstddef>
#include <type_traits>
#include <complex>
#include "integer.hpp"

template <typename T1, typename T2>
struct get_initial_capacity {
    private:
    static constexpr size_t partitions = sizeof(T1) / sizeof(T2);

    public:
    static constexpr size_t value = partitions ? partitions : 1;
};

namespace apa {

class FFT {
public:
    using Complex = std::complex<double>;

    static void transform(std::vector<Complex>& a, bool invert) {
        size_t n = a.size();
        if (n == 0) return;

        // Бит-реверс перестановка
        for (size_t i = 1, j = 0; i < n; i++) {
            size_t bit = n >> 1;
            for (; j >= bit; bit >>= 1)
                j -= bit;
            j += bit;
            if (i < j)
                std::swap(a[i], a[j]);
        }

        // Бабочки Cooley-Tukey
        for (size_t len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            Complex wlen(cos(ang), sin(ang));
            for (size_t i = 0; i < n; i += len) {
                Complex w(1);
                for (size_t j = 0; j < len / 2; j++) {
                    Complex u = a[i + j];
                    Complex v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (Complex& x : a)
                x /= n;
        }
    }

    static integer multiply(const integer& a, const integer& b) {
            // Проверка нулей
            if (!a || !b) return __INTEGER_ZERO;

            // Обработка малых чисел без FFT
            if (a.length <= 8 || b.length <= 8) {
                return a.schoolbook_mult(b);
            }

            // Конвертация в коэффициенты
            std::vector<limb_t> a_coeffs = convert_to_coeffs(a);
            std::vector<limb_t> b_coeffs = convert_to_coeffs(b);

            // Размер для FFT
            size_t n = 1;
            while (n < a_coeffs.size() + b_coeffs.size()) 
                n <<= 1;

            std::vector<Complex> fa(n), fb(n);
            for (size_t i = 0; i < a_coeffs.size(); i++)
                fa[i] = a_coeffs[i];
            for (size_t i = 0; i < b_coeffs.size(); i++)
                fb[i] = b_coeffs[i];

            // Прямое преобразование
            transform(fa, false);
            transform(fb, false);

            // Поэлементное умножение
            for (size_t i = 0; i < n; i++)
                fa[i] *= fb[i];

            // Обратное преобразование
            transform(fa, true);

            // Сборка результата с переносами
            std::vector<limb_t> result;
            limb_t carry = 0;
            for (size_t i = 0; i < n; i++) {
                double raw_val = fa[i].real();
                limb_t val = static_cast<limb_t>(std::round(raw_val)) + carry;
                carry = val >> FFT_BASE_BITS;
                result.push_back(val & (FFT_BASE - 1));
            }
            
            while (carry) {
                result.push_back(carry & (FFT_BASE - 1));
                carry >>= FFT_BASE_BITS;
            }

            // Удаление ведущих нулей
            while (result.size() > 1 && result.back() == 0)
                result.pop_back();
            
            // Конвертация обратно в integer
            return convert_from_coeffs(result);
        }

    private:
        static std::vector<limb_t> convert_to_coeffs(const integer& num) {
            std::vector<limb_t> coeffs;
            if (!num) {
                coeffs.push_back(0);
                return coeffs;
            }
            
            integer temp = num;
            integer base = FFT_BASE;
            
            while (temp) {
                integer quotient;
                integer remainder;
                integer::div_mod(quotient, remainder, temp, base);
                coeffs.push_back(remainder.limbs[0]);
                temp = quotient;
            }
            return coeffs;
        }

        static integer convert_from_coeffs(const std::vector<limb_t>& coeffs) {
            integer result = 0;
            for (auto it = coeffs.rbegin(); it != coeffs.rend(); ++it) {
                result = result << FFT_BASE_BITS;
                result += integer(*it);
            }
            return result;
        }
    };


    integer::integer() noexcept
    :   capacity(INITIAL_LIMB_CAPACITY),
        length(INITIAL_LIMB_LENGTH),
        limbs((limb_t *) std::malloc(INITIAL_LIMB_CAPACITY * LIMB_BYTES))
    {}

    integer::integer(size_t num) noexcept
    :   capacity(get_initial_capacity<size_t, limb_t>::value),
        length(capacity),
        limbs((limb_t *) std::calloc(capacity, sizeof(limb_t)))
    {
        std::memcpy(limbs, &num, sizeof(num));
        remove_leading_zeros();
    }

    integer::integer(size_t capacity, size_t length, bool AllocateSpace)
    :   capacity(capacity),
        length(length)
    {
        if (AllocateSpace) {
            limbs = (limb_t *) std::malloc(capacity * LIMB_BYTES);
        } else {
            limbs = NULL;
        }
    }

    // constructor for conveniece
    integer::integer(const std::string &text, size_t base) {
        std::vector<uint8_t> output(text.size(), 0);
        std::string newText;

        if (base > 1 && base < HEX) {
            for (size_t i = 0; i < text.size(); ++i) {
                uint8_t carry = text[i] - '0';
                size_t j = text.size();
                while (j--) {
                    uint8_t tmp = output[j] * base + carry;
                    output[j] = tmp % 16;
                    carry = tmp / 16;
                }
            }

            size_t leading_zeros = 0;
            while (output.size() > 1 && !output[leading_zeros]) {
                leading_zeros++;
            }

            output.erase(output.begin(), output.begin() + leading_zeros);

            std::string stringForm(output.size(), '0');

            for (size_t i = 0; i < stringForm.size(); ++i) {
                stringForm[i] = HEX_TO_CHAR[output[i]];
            }

            newText = stringForm;
        } else if (base != HEX) {
            throw std::domain_error(
                "integer - string contructor : supported number "
                "base range is only from 2 to 16"
            );
        } else {
            newText = text;
        }

        size_t blocks = newText.size() / (CAST_BYTES);
        size_t remain = newText.size() % (CAST_BYTES);

        length = blocks;
        length += !!remain;

        capacity = length + LIMB_GROWTH;
        limbs = (limb_t *) std::calloc(capacity, sizeof(limb_t));

        for (size_t i = 0; i < newText.size(); ++i) {
            unsigned char CharByte = CHAR_TO_HEX[(unsigned char) newText[newText.size() - 1 - i]];
            if (CharByte == 0xff)
                break;

            size_t multiplier = std::pow(0x10, i % CAST_BYTES);
            limbs[i / CAST_BYTES] |= CharByte * multiplier;
        }

        remove_leading_zeros();
    }

    // a read only constructor
    integer::integer(limb_t *arr, size_t capacity, size_t length) noexcept
    :   capacity(capacity),
        length(length),
        limbs(arr)
    {}

    /// copy constructor.
    integer::integer(const integer &src)
    :   capacity(src.capacity),
        length(src.length),
        limbs((limb_t *) std::malloc(capacity * LIMB_BYTES))
    {
        std::memcpy(limbs, src.limbs, length * LIMB_BYTES);
    }

    /// move constructor.
    integer::integer(integer &&src) noexcept
    :   capacity(src.capacity),
        length(src.length),
        limbs(src.limbs)
    {
        src.limbs = NULL;
    }

    /// copy assignment.
    integer &integer::operator=(const integer &src) {
        if (this != &src) {
            if (capacity <= src.length) {
                limbs = (limb_t *) std::realloc(limbs, src.capacity * LIMB_BYTES);
                capacity = src.capacity;
            }

            length = src.length;
            std::memcpy(limbs, src.limbs, src.length * LIMB_BYTES);
        }
        return *this;
    }

    /// move assignment.
    integer &integer::operator=(integer &&src) noexcept {
        if (this != &src) {
            std::free(limbs);
            capacity = src.capacity;
            length = src.length;
            limbs = src.limbs;
            src.limbs = NULL;
        }
        return *this;
    }

    integer::integer(std::initializer_list<limb_t> limbs)
    :   capacity(limbs.size() + LIMB_GROWTH),
        length(limbs.size()),
        limbs((limb_t *) std::malloc(capacity * LIMB_BYTES))
    {
        size_t i = 0;
        for (auto limb: limbs) {
            this->limbs[length - 1 - i++] = limb;
        }
    }

    integer::~integer() {
        std::free(limbs);
        capacity = 0;
        length = 0;
    }

    // Index Operator
    limb_t &integer::operator[](size_t i) noexcept {
        return limbs[i];
    }

    limb_t &integer::operator[](size_t i) const noexcept {
        return limbs[i];
    }

    /// @return returns; -1 : if less than, 0 : if equal, 1 : if greater than.
    int integer::compare(const integer &op) const noexcept {
        if (length < op.length) {
            return LESS;
        } else if (length > op.length) {
            return GREAT;
        }

        for (size_t i = 0; i < length; ++i) {
            if (limbs[length - 1 - i] < op.limbs[length - 1 - i]) {
                return LESS;
            } else if (limbs[length - 1 - i] > op.limbs[length - 1 - i]) {
                return GREAT;
            }
        }

        return EQUAL;
    }

    // Logical Operators

    bool integer::operator<(const integer &op) const noexcept {
        return compare(op) == LESS;
    }

    bool integer::operator>(const integer &op) const noexcept {
        return compare(op) == GREAT;
    }

    bool integer::operator==(const integer &op) const noexcept {
        return compare(op) == EQUAL;
    }

    bool integer::operator!=(const integer &op) const noexcept {
        return (*this == op) ^ 1u;
    }

    bool integer::operator<=(const integer &op) const noexcept {
        return compare(op) <= EQUAL;
    }

    bool integer::operator>=(const integer &op) const noexcept {
        return compare(op) >= EQUAL;
    }

    // Bit-Wise Logical Operators

    void integer::bit_realloc(const integer &op) noexcept {
        size_t zero_set = length * LIMB_BYTES;
        capacity = op.capacity;
        limbs = (limb_t *) std::realloc(limbs, capacity * LIMB_BYTES);
        std::memset(limbs + length, 0x00, (op.length * LIMB_BYTES) - zero_set);
        length = op.length;
    }

    void integer::remove_leading_zeros() noexcept {
        for (size_t i = 0; i < length; ++i) {
            if (limbs[length - 1 - i]) {
                length -= i;
                return;
            }
        }
        length = 1;
    }

    // Logical Operators
    integer::operator bool() const noexcept {
        return (length == 1 && !limbs[0]) ^ 1u;
    }

    // Arithmetic Operators

    integer &integer::operator+=(const integer &op) noexcept {
        if (capacity <= op.length + 1) {
            capacity = op.length + LIMB_GROWTH;
            limbs = (limb_t *) std::realloc(limbs, capacity * LIMB_BYTES);
        }

        if (length == capacity) {
            capacity = length + LIMB_GROWTH;
            limbs = (limb_t *) std::realloc(limbs, capacity * LIMB_BYTES);
            limbs[length++] = 0;
        }

        if (length <= op.length) {
            std::memset(limbs + length, 0x00, ((op.length + 1) - length) * LIMB_BYTES);
            length = op.length + 1;
        }

        limb_t carry = 0;

        for (size_t i = 0; i < op.length; ++i) {
            cast_t sum = (cast_t) limbs[i] + op.limbs[i] + carry;
            limbs[i] = sum;
            carry = sum >> BASE_BITS;
        }

        for (size_t i = op.length; i < length; ++i) {
            cast_t sum = (cast_t) limbs[i] + carry;
            limbs[i] = sum;
            carry = sum >> BASE_BITS;
        }

        remove_leading_zeros();
        return *this;
    }

    integer integer::operator+(const integer &op) const noexcept {
        limb_t *max_limb, *min_limb;
        size_t max_len, min_len;

        if (length > op.length) {
            max_limb = limbs;
            max_len = length;
            min_limb = op.limbs;
            min_len = op.length;
        } else {
            max_limb = op.limbs;
            max_len = op.length;
            min_limb = limbs;
            min_len = length;
        }

        limb_t *sum_array = (limb_t *) std::malloc(LIMB_BYTES * (max_len + 1));
        limb_t carry = 0;

        for (size_t i = 0; i < min_len; ++i) {
            cast_t sum = (cast_t) max_limb[i] + min_limb[i] + carry;
            sum_array[i] = sum;
            carry = sum >> BASE_BITS;
        }

        for (size_t i = min_len; i < max_len; ++i) {
            cast_t sum = (cast_t) max_limb[i] + carry;
            sum_array[i] = sum;
            carry = sum >> BASE_BITS;
        }

        sum_array[max_len] = carry;
        return integer(sum_array, max_len + 1, max_len + carry);
    }

    integer &integer::operator-=(const integer &op) noexcept {
        limb_t carry = 0;

        for (size_t i = 0; i < op.length; ++i) {
            cast_t dif_index = (cast_t) limbs[i] - op.limbs[i] - carry;
            limbs[i] = dif_index;
            carry = (dif_index >> BASE_BITS);
            carry &= 0x01;
        }

        for (size_t i = op.length; i < length; ++i) {
            cast_t dif_index = (cast_t) limbs[i] - carry;
            limbs[i] = dif_index;
            carry = (dif_index >> BASE_BITS);
            carry &= 0x01;
        }

        remove_leading_zeros();
        return *this;
    }

    integer integer::operator-(const integer &op) const noexcept {
        size_t dif_len = std::max(length, op.length);
        limb_t *dif_array = (limb_t *) std::malloc(dif_len * LIMB_BYTES);

        limb_t carry = 0;

        for (size_t i = 0; i < op.length; ++i) {
            cast_t dif_index = (cast_t) limbs[i] - op.limbs[i] - carry;
            dif_array[i] = dif_index;
            carry = (dif_index >> BASE_BITS);
            carry &= 0x01;
        }

        for (size_t i = op.length; i < length; ++i) {
            cast_t dif_index = (cast_t) limbs[i] - carry;
            dif_array[i] = dif_index;
            carry = (dif_index >> BASE_BITS);
            carry &= 0x01;
        }

        integer dif_int(dif_array, dif_len, dif_len);
        dif_int.remove_leading_zeros();
        return dif_int;
    }

    integer &integer::operator*=(const integer &op) noexcept {
        integer product = *this * op;
        swap(product, *this);
        return *this;
    }

    integer integer::schoolbook_mult(const integer& op) const noexcept {
    if ((*this && op) ^ 1u) {
        return __INTEGER_ZERO;
    }

    limb_t* arr = (limb_t*)std::malloc(LIMB_BYTES * (length + op.length));
    integer product(arr, length + op.length, length + op.length);

    size_t i = 0, j = 0;
    limb_t carry = 0;
    
    // Первый проход (умножение на первый limb)
    for (j = 0; j < length; ++j) {
        cast_t product_index = (cast_t)limbs[j] * op.limbs[0] + carry;
        product.limbs[j] = product_index;
        carry = product_index >> BASE_BITS;
    }
    product.limbs[length] = carry;

    // Последующие проходы
    for (i = 1; i < op.length; ++i) {
        carry = 0;
        for (j = 0; j < length; ++j) {
            cast_t product_index = (cast_t)limbs[j] * op.limbs[i] 
                                + product.limbs[i + j] 
                                + carry;
            product.limbs[i + j] = product_index;
            carry = product_index >> BASE_BITS;
        }
        product.limbs[i + length] = carry;
    }

    product.length = length + op.length;
    product.remove_leading_zeros();
    return product;
}

integer integer::slice(size_t start, size_t len) const {
    if (start >= length || len == 0) {
        return __INTEGER_ZERO;
    }
    
    size_t actual_len = std::min(len, length - start);
    integer result(actual_len, actual_len);
    std::memcpy(result.limbs, limbs + start, actual_len * LIMB_BYTES);
    result.remove_leading_zeros();
    return result;
}

integer integer::karatsuba_mult(const integer& x, const integer& y) {
    const size_t threshold = 32; // Порог переключения на школьный метод

    // Проверка на маленькие числа
    if (x.length <= threshold || y.length <= threshold) {
        return x.schoolbook_mult(y);
    }

    // Вычисляем точку разбиения
    size_t m = (std::max(x.length, y.length) + 1) / 2;

    // Разбиваем числа на части
    integer x0 = x.slice(0, m);
    integer x1 = x.slice(m, x.length - m);
    integer y0 = y.slice(0, m);
    integer y1 = y.slice(m, y.length - m);

    // Рекурсивные вычисления
    integer z0 = karatsuba_mult(x0, y0);
    integer z2 = karatsuba_mult(x1, y1);
    
    // Промежуточные суммы
    integer x10 = x1 + x0;
    integer y10 = y1 + y0;
    integer z1 = karatsuba_mult(x10, y10) - z0 - z2;

    // Комбинирование результатов
    integer part2 = z2 << (2 * m * BASE_BITS);
    integer part1 = z1 << (m * BASE_BITS);
    integer result = part2 + part1 + z0;
    
    return result;
}

integer integer::operator*(const integer& op) const noexcept {
    const size_t threshold = 32; // Порог для Карацубы

    // Проверка на ноль
    if ((*this && op) ^ 1u) {
        return __INTEGER_ZERO;
    }

    // Выбор алгоритма умножения
    // if (length <= threshold || op.length <= threshold) {
        // return schoolbook_mult(op);
    // }
    return FFT::multiply(*this, op);
}


    void div_n_by_1(limb_t *quotient, limb_t *dividen, size_t length, limb_t divisor) {
        cast_t remainder = 0;
        
        remainder = dividen[length - 1] % divisor;
        remainder <<= BASE_BITS;
        quotient[length - 1] = dividen[length - 1] / divisor;

        for (size_t i = 1; i < length; ++i) {
            remainder |= dividen[length - 1 - i];
            quotient[length - 1 - i] = remainder / divisor;
            remainder = (remainder % divisor) << BASE_BITS;
        }
    }

    void div_n_by_m(limb_t *quotient, const integer& dividen, const integer& divisor) {
        integer remainder(dividen.length, dividen.length);
        remainder.length = 1;
        remainder.limbs[0] = 0;

        limb_t bit = 0, current_index = dividen.length, onebit = 1;
        size_t total_bits = dividen.length * BASE_BITS;
        
        for (size_t h = 0; h < total_bits; h += BASE_BITS) {
            remainder <<= BASE_BITS;
            remainder.limbs[0] = dividen.limbs[--current_index];
            quotient[current_index] = 0;

            if (remainder >= divisor) {
                limb_t r = remainder.limbs[0];
                remainder >>= BASE_BITS;

                for (size_t i = 0; i < BASE_BITS; ++i) {
                    bit = (r << i);
                    bit >>= BASE_BITS_MINUS1;

                    remainder <<= 1;
                    remainder.limbs[0] |= bit;

                    if (remainder >= divisor) {
                        remainder -= divisor;
                        quotient[current_index] |= (onebit << (BASE_BITS_MINUS1 - i));
                    }
                }
            }
        }
    }

    void integer::div_mod(integer& q, integer& r, integer& dividen, const integer& divisor) {
        if (divisor.length == 1) {
            cast_t remainder = 0;
            limb_t onelimb_divisor = divisor[0];
        
            remainder = dividen.limbs[dividen.length - 1] % onelimb_divisor;
            remainder <<= BASE_BITS;
            q.limbs[dividen.length - 1] = dividen.limbs[dividen.length - 1] / onelimb_divisor;

            for (size_t i = 1; i < dividen.length; ++i) {
                remainder |= dividen.limbs[dividen.length - 1 - i];
                q.limbs[dividen.length - 1 - i] = remainder / onelimb_divisor;
                remainder = (remainder % onelimb_divisor) << BASE_BITS;
            }

            r = remainder >> BASE_BITS;
        } else {
            integer remainder(dividen.length, dividen.length);
            remainder.length = 1;
            remainder.limbs[0] = 0;

            limb_t bit = 0, current_index = dividen.length, onebit = 1;
            size_t total_bits = dividen.length * BASE_BITS;
            
            for (size_t h = 0; h < total_bits; h += BASE_BITS) {
                remainder <<= BASE_BITS;
                remainder.limbs[0] = dividen.limbs[--current_index];
                q.limbs[current_index] = 0;

                if (remainder >= divisor) {
                    limb_t r = remainder.limbs[0];
                    remainder >>= BASE_BITS;

                    for (size_t i = 0; i < BASE_BITS; ++i) {
                        bit = (r << i);
                        bit >>= BASE_BITS_MINUS1;

                        remainder <<= 1;
                        remainder.limbs[0] |= bit;

                        if (remainder >= divisor) {
                            remainder -= divisor;
                            q.limbs[current_index] |= (onebit << (BASE_BITS_MINUS1 - i));
                        }
                    }
                }
            }

            r = remainder;
        }
        q.remove_leading_zeros();
    }

    // pre-fix increment/decrement
    integer &integer::operator++() noexcept {
        return *this += __INTEGER_ONE;
    }

    integer &integer::operator--() noexcept {
        return *this -= __INTEGER_ONE;
    }

    // post-fix increment/decrement
    integer integer::operator++(int) noexcept {
        integer prev = *this;
        ++*this;
        return prev;
    }

    integer integer::operator--(int) noexcept {
        integer prev = *this;
        --*this;
        return prev;
    }

    // Shift Operators
    integer &integer::operator<<=(size_t bits) noexcept {
        if (*this && bits) {
            size_t limb_shifts = bits / BASE_BITS;
            size_t bit_shifts = bits % BASE_BITS;
            size_t new_length = length + limb_shifts + 1;
            size_t zero_limbs = new_length - length - 1;

            if (new_length > capacity) {
                capacity = new_length + LIMB_GROWTH;
                limbs = (limb_t *) std::realloc(limbs, capacity * LIMB_BYTES);
            }

            limbs[new_length - 1] = 0;

            for (size_t i = 0; i < length; ++i) {
                cast_t temp_shift = limbs[length - 1 - i];
                temp_shift <<= bit_shifts;
                limbs[new_length - 1 - i] |= temp_shift >> BASE_BITS;
                limbs[new_length - 2 - i] = temp_shift;
            }

            if (zero_limbs) {
                std::memset(limbs, 0x00, zero_limbs * LIMB_BYTES);
            }

            length = new_length;
            length -= !limbs[length - 1];
        }
        return *this;
    }

    integer &integer::operator>>=(size_t bits) noexcept {
        if (*this && bits) {
            size_t limb_shifts = bits / BASE_BITS;
            if (limb_shifts >= length) {
                length = 1;
                limbs[0] = 0;
            } else {
                size_t bit_shifts = bits % BASE_BITS;
                size_t new_length = length - limb_shifts;

                // initial limb shift
                limbs[0] = limbs[limb_shifts] >> bit_shifts;

                // looped shifts
                for (size_t i = 1; i < new_length; ++i) {
                    cast_t temp_shift = limbs[i + limb_shifts];
                    temp_shift = (temp_shift << BASE_BITS) >> bit_shifts;
                    limbs[i - 1] |= temp_shift;
                    limbs[i] = temp_shift >> BASE_BITS;
                }

                length = new_length;
                length -= !limbs[length - 1] && length != 1;
            }
        }
        return *this;
    }

    integer integer::operator<<(size_t bits) const noexcept {
        integer shifted = *this;
        return shifted <<= bits;
    }

    integer integer::operator>>(size_t bits) const noexcept {
        integer shifted = *this;
        return shifted >>= bits;
    }

    void integer::printHex() const {
        std::cout << "0x";
        printf(PRINT_LIMBHEX_NOPAD, (limb_t) limbs[length - 1]);
        for (size_t i = 1; i < length; ++i) {
            printf(PRINT_LIMBHEX, (limb_t) limbs[length - 1 - i]);
        }
        std::cout << "\n";
    }

    void integer::printHex_spaced_out() const {
        printf(PRINT_LIMBHEX, (limb_t) limbs[length - 1]);
        for (size_t i = 1; i < length; ++i) {
            printf(PRINT_LIMBHEX_SPACED, (limb_t) limbs[length - 1 - i]);
        }
        std::cout << "\n";
    }

    void integer::printBin_spaced_out() const {
        for (size_t i = 0; i < length; ++i) {
            std::cout << std::bitset<BASE_BITS>(limbs[length - 1 - i]) << " ";
        }
        std::cout << "\n";
    }

    void integer::printStatus(std::string printIdentifier) const {
        std::cout << "\n-----\n";
        std::cout << printIdentifier << "\n";
        std::cout << "capacity : " << capacity << "\n";
        std::cout << "length   : " << length << "\n";
        printHex_spaced_out();
    }

    std::string integer::to_base10_string() const {
        std::string Base10;
        integer quotient = *this, remainder = 0;
        Base10.reserve(quotient.length);
        if (quotient) {
            while (quotient) {
                div_mod(quotient, remainder, quotient, __INTEGER_TEN);
                Base10.push_back('0' + remainder.limbs[0]);
            }

            std::reverse(Base10.begin(), Base10.end());
        } else {
            Base10 = "0";
        }
        return Base10;
    }

    std::string integer::to_base16_string() const {
        char buffer[17];
        std::string hexform = "";
        sprintf(buffer, PRINT_LIMBHEX_NOPAD, (limb_t) limbs[length - 1]);
        hexform.append(buffer);
        for (size_t i = 1; i < length; ++i) {
            sprintf(buffer, PRINT_LIMBHEX, (limb_t) limbs[length - 1 - i]);
            hexform.append(buffer);
        }
        return hexform;
    }

    // Methods

    size_t integer::byte_size() const noexcept {
        limb_t ms_limb = limbs[length - 1];
        size_t cnt = 0;
        while (ms_limb) {
            ms_limb >>= 8;
            cnt++;
        }
        return (length - 1) * BASE_BYTES + cnt;
    }

    size_t integer::bit_size() const noexcept {
        limb_t ms_limb = limbs[length - 1];
        size_t cnt = 0;
        while (ms_limb) {
            ms_limb >>= 1;
            cnt++;
        }
        return (length - 1) * BASE_BITS + cnt;
    }

    limb_t *integer::detach() {
        limb_t *detached = limbs;
        limbs = NULL;
        capacity = 0;
        length = 0;
        return detached;
    }

    void swap(integer &a, integer &b) {
        integer temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }

    // IO Operators
    std::ostream &operator<<(std::ostream &out, const integer &num) {
        out << num.to_base16_string();
        return out;
    }

    std::istream &operator>>(std::istream &in, integer &num) {
        std::string input;
        in >> input;
        num = integer(input, 16);
        return in;
    }
} // namespace apa

#endif