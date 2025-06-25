#ifndef APA_BINT_CPP
#define APA_BINT_CPP

#include "bint.hpp"
#include "integer.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <algorithm>

namespace apa {

// Константы знаков
const limb_t BINT_POSITIVE = 0;
const limb_t BINT_NEGATIVE = 1;

// Глобальные константы
const bint APA_BINT_ZERO = {{1, 1, new limb_t[1]{0}}, BINT_POSITIVE};
const bint APA_BINT_ONE = {{1, 1, new limb_t[1]{1}}, BINT_POSITIVE};
const bint APA_BINT_TWO = {{1, 1, new limb_t[1]{2}}, BINT_POSITIVE};
const bint APA_BINT_TEN = {{1, 1, new limb_t[1]{10}}, BINT_POSITIVE};

// ====================== Инициализация и управление памятью ======================

void bint_init(bint* num) {
    integer_init(&num->number);
    num->sign = BINT_POSITIVE;
}

void bint_init_int(bint* num, int value) {
    integer_init_size(&num->number, std::abs(value));
    num->sign = (value < 0) ? BINT_NEGATIVE : BINT_POSITIVE;
}

void bint_init_string(bint* num, const char* str) {
    std::string input(str);
    
    // Обработка пустой строки
    if (input.empty()) {
        throw std::invalid_argument("Empty string passed to bint_init_string");
    }
    
    // Определение знака
    size_t start = 0;
    limb_t sign = BINT_POSITIVE;
    if (input[0] == '-') {
        sign = BINT_NEGATIVE;
        start = 1;
    } else if (input[0] == '+') {
        start = 1;
    }
    
    // Определение системы счисления
    size_t base = 10;
    if (input.size() >= start + 2) {
        if (input[start] == '0') {
            switch (input[start + 1]) {
                case 'b': base = 2; start += 2; break;
                case 'o': base = 8; start += 2; break;
                case 'x': base = 16; start += 2; break;
            }
        }
    }
    
    // Инициализация целой части
    std::string num_str = input.substr(start);
    integer_init_string(&num->number, num_str, base);
    num->sign = sign;
    
    // Коррекция нуля
    if (num->number.length == 1 && num->number.limbs[0] == 0) {
        num->sign = BINT_POSITIVE;
    }
}

void bint_copy(const bint* src, bint* dest) {
    integer_copy(&src->number, &dest->number);
    dest->sign = src->sign;
}

void bint_move(bint* src, bint* dest) {
    integer_move(&src->number, &dest->number);
    dest->sign = src->sign;
    
    src->sign = BINT_POSITIVE;
}

void bint_free(bint* num) {
    integer_free(&num->number);
    num->sign = BINT_POSITIVE;
}

// ====================== Операции сравнения ======================

int bint_compare(const bint* a, const bint* b) {
    // Разные знаки
    if (a->sign != b->sign) {
        return (a->sign == BINT_NEGATIVE) ? -1 : 1;
    }
    
    // Оба отрицательные
    if (a->sign == BINT_NEGATIVE) {
        return -integer_compare(&a->number, &b->number);
    }
    
    // Оба положительные
    return integer_compare(&a->number, &b->number);
}

// ====================== Арифметические операции ======================

void bint_add(const bint* a, const bint* b, bint* result) {
    // Одинаковые знаки
    if (a->sign == b->sign) {
        integer_add(&a->number, &b->number, &result->number);
        result->sign = a->sign;
        return;
    }
    
    // Разные знаки - фактически вычитание
    int cmp = integer_compare(&a->number, &b->number);
    if (cmp > 0) {
        integer_sub(&a->number, &b->number, &result->number);
        result->sign = a->sign;
    } else if (cmp < 0) {
        integer_sub(&b->number, &a->number, &result->number);
        result->sign = b->sign;
    } else {
        // Числа равны по модулю, результат 0
        integer_set_uint(&result->number, 0);
        result->sign = BINT_POSITIVE;
    }
}

void bint_sub(const bint* a, const bint* b, bint* result) {
    // Противоположные знаки - фактически сложение
    if (a->sign != b->sign) {
        integer_add(&a->number, &b->number, &result->number);
        result->sign = a->sign;
        return;
    }
    
    // Одинаковые знаки - вычитание
    int cmp = integer_compare(&a->number, &b->number);
    if (cmp > 0) {
        integer_sub(&a->number, &b->number, &result->number);
        result->sign = a->sign;
    } else if (cmp < 0) {
        integer_sub(&b->number, &a->number, &result->number);
        result->sign = (a->sign == BINT_POSITIVE) ? BINT_NEGATIVE : BINT_POSITIVE;
    } else {
        // Числа равны, результат 0
        integer_set_uint(&result->number, 0);
        result->sign = BINT_POSITIVE;
    }
}

void bint_mul(const bint* a, const bint* b, bint* result) {
    integer_mul(&a->number, &b->number, &result->number);
    
    // Определение знака результата
    if (a->sign == b->sign) {
        result->sign = BINT_POSITIVE;
    } else {
        result->sign = BINT_NEGATIVE;
    }
    
    // Коррекция нуля
    if (result->number.length == 1 && result->number.limbs[0] == 0) {
        result->sign = BINT_POSITIVE;
    }
}

void bint_negate(bint* num) {
    if (num->number.length == 1 && num->number.limbs[0] == 0) {
        return; // -0 = 0
    }
    num->sign = (num->sign == BINT_POSITIVE) ? BINT_NEGATIVE : BINT_POSITIVE;
}

// ====================== Инкремент/декремент ======================

void bint_inc(bint* num) {
    bint one;
    bint_init_int(&one, 1);
    bint_add(num, &one, num);
    bint_free(&one);
}

void bint_dec(bint* num) {
    bint one;
    bint_init_int(&one, 1);
    bint_sub(num, &one, num);
    bint_free(&one);
}

// ====================== Битовые операции ======================

void bint_lshift(bint* num, size_t bits) {
    integer_lshift(&num->number, bits);
}

void bint_rshift(bint* num, size_t bits) {
    integer_rshift(&num->number, bits);
}

// ====================== Преобразования и вывод ======================

std::string bint_to_string(const bint* num, size_t base) {
    std::string result;
    
    if (num->sign == BINT_NEGATIVE) {
        result += '-';
    }
    
    result += integer_to_string(&num->number, base);
    return result;
}

std::string bint_to_hex(const bint* num) {
    std::string result;
    
    if (num->sign == BINT_NEGATIVE) {
        result += '-';
    }
    
    result += integer_to_hex(&num->number);
    return result;
}

// ====================== Вспомогательные функции ======================

size_t bint_byte_size(const bint* num) {
    return integer_byte_size(&num->number);
}

size_t bint_bit_size(const bint* num) {
    return integer_bit_size(&num->number);
}

const limb_t* bint_limb_view(const bint* num) {
    return num->number.limbs;
}

limb_t* bint_detach(bint* num) {
    limb_t* limbs = integer_detach(&num->number);
    num->sign = BINT_POSITIVE;
    return limbs;
}

} // namespace apa

// Макросы для совместимости
#define __BINT_ZERO apa::APA_BINT_ZERO
#define __BINT_ONE apa::APA_BINT_ONE
#define __BINT_TWO apa::APA_BINT_TWO
#define __BINT_TEN apa::APA_BINT_TEN

#endif // APA_BINT_CPP