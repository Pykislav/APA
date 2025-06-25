#ifndef APA_INTEGER_CPP
#define APA_INTEGER_CPP

#include "integer.hpp"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

namespace apa {

// Константы преобразования
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

// Константные значения
const integer APA_INTEGER_ZERO = {1, 1, new limb_t[1]{0}};
const integer APA_INTEGER_ONE = {1, 1, new limb_t[1]{1}};
const integer APA_INTEGER_TWO = {1, 1, new limb_t[1]{2}};
const integer APA_INTEGER_TEN = {1, 1, new limb_t[1]{10}};

// Внутренние вспомогательные функции
namespace {
    void ensure_capacity(integer* num, size_t required_capacity) {
        if (num->capacity >= required_capacity) return;
        
        size_t new_capacity = std::max(num->capacity * 2, required_capacity);
        limb_t* new_limbs = new limb_t[new_capacity];
        
        // Копирование существующих данных
        std::memcpy(new_limbs, num->limbs, num->length * sizeof(limb_t));
        
        // Обнуление нового пространства
        std::memset(new_limbs + num->length, 0, 
                   (new_capacity - num->length) * sizeof(limb_t));
        
        // Освобождение старой памяти и обновление указателя
        delete[] num->limbs;
        num->limbs = new_limbs;
        num->capacity = new_capacity;
    }

    void trim_leading_zeros(integer* num) {
        size_t new_length = num->length;
        while (new_length > 1 && num->limbs[new_length - 1] == 0) {
            new_length--;
        }
        num->length = new_length;
    }
}

// Основные функции
void integer_init(integer* num) {
    num->capacity = INITIAL_LIMB_CAPACITY;
    num->length = INITIAL_LIMB_LENGTH;
    num->limbs = new limb_t[INITIAL_LIMB_CAPACITY];
    std::memset(num->limbs, 0, INITIAL_LIMB_CAPACITY * sizeof(limb_t));
}

void integer_init_size(integer* num, size_t value) {
    num->capacity = INITIAL_LIMB_CAPACITY;
    num->length = 1;
    num->limbs = new limb_t[INITIAL_LIMB_CAPACITY];
    std::memset(num->limbs, 0, INITIAL_LIMB_CAPACITY * sizeof(limb_t));
    num->limbs[0] = static_cast<limb_t>(value);
}

void integer_init_string(integer* num, const std::string& value, size_t base) {
    integer_init(num);
    
    // Пропускаем префиксы и знаки
    size_t start = 0;
    while (start < value.size() && 
          (value[start] == '0' || value[start] == '-' || value[start] == ' ')) {
        start++;
    }
    
    if (start >= value.size()) {
        integer_set_uint(num, 0);
        return;
    }
    
    // Обработка по основанию системы счисления
    for (size_t i = start; i < value.size(); i++) {
        char c = value[i];
        limb_t digit = 0;
        
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            digit = 10 + (c - 'A');
        } else {
            throw std::invalid_argument("Invalid character in number string");
        }
        
        if (digit >= base) {
            throw std::invalid_argument("Digit exceeds base value");
        }
        
        // Умножение текущего значения на базу
        integer tmp;
        integer_init_size(&tmp, base);
        integer_mul(num, &tmp);
        integer_free(&tmp);
        
        // Добавление новой цифры
        integer digit_num;
        integer_init_size(&digit_num, digit);
        integer_add(num, &digit_num);
        integer_free(&digit_num);
    }
}

void integer_copy(const integer* src, integer* dest) {
    dest->capacity = src->capacity;
    dest->length = src->length;
    dest->limbs = new limb_t[dest->capacity];
    std::memcpy(dest->limbs, src->limbs, src->length * sizeof(limb_t));
}

void integer_move(integer* src, integer* dest) {
    dest->capacity = src->capacity;
    dest->length = src->length;
    dest->limbs = src->limbs;
    
    src->capacity = 0;
    src->length = 0;
    src->limbs = nullptr;
}

void integer_free(integer* num) {
    if (num->limbs) {
        delete[] num->limbs;
        num->limbs = nullptr;
    }
    num->capacity = 0;
    num->length = 0;
}

void integer_set_uint(integer* num, limb_t value) {
    if (num->capacity < 1) {
        ensure_capacity(num, 1);
    }
    num->length = 1;
    num->limbs[0] = value;
}

int integer_compare(const integer* a, const integer* b) {
    if (a->length > b->length) return 1;
    if (a->length < b->length) return -1;
    
    for (size_t i = a->length; i-- > 0;) {
        if (a->limbs[i] > b->limbs[i]) return 1;
        if (a->limbs[i] < b->limbs[i]) return -1;
    }
    
    return 0;
}

void integer_add(const integer* a, const integer* b, integer* result) {
    size_t max_len = std::max(a->length, b->length);
    ensure_capacity(result, max_len + 1);
    
    limb_t carry = 0;
    size_t i = 0;
    
    // Сложение поразрядно
    for (; i < max_len; i++) {
        cast_t sum = carry;
        if (i < a->length) sum += static_cast<cast_t>(a->limbs[i]);
        if (i < b->length) sum += static_cast<cast_t>(b->limbs[i]);
        
        result->limbs[i] = static_cast<limb_t>(sum);
        carry = static_cast<limb_t>(sum >> BASE_BITS);
    }
    
    // Обработка переноса
    result->limbs[i] = carry;
    result->length = max_len + (carry ? 1 : 0);
    trim_leading_zeros(result);
}

void integer_sub(const integer* a, const integer* b, integer* result) {
    if (integer_compare(a, b) < 0) {
        throw std::underflow_error("Subtraction would result in negative value");
    }
    
    ensure_capacity(result, a->length);
    limb_t borrow = 0;
    
    for (size_t i = 0; i < a->length; i++) {
        cast_t diff = static_cast<cast_t>(a->limbs[i]) - borrow;
        if (i < b->length) diff -= b->limbs[i];
        
        if (diff < 0) {
            diff += (static_cast<cast_t>(1) << BASE_BITS);
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result->limbs[i] = static_cast<limb_t>(diff);
    }
    
    result->length = a->length;
    trim_leading_zeros(result);
}

void integer_mul(const integer* a, const integer* b, integer* result) {
    if (a->length == 0 || b->length == 0 || 
       (a->length == 1 && a->limbs[0] == 0) || 
       (b->length == 1 && b->limbs[0] == 0)) {
        integer_set_uint(result, 0);
        return;
    }
    
    size_t result_len = a->length + b->length;
    ensure_capacity(result, result_len);
    std::memset(result->limbs, 0, result_len * sizeof(limb_t));
    
    for (size_t i = 0; i < a->length; i++) {
        limb_t carry = 0;
        for (size_t j = 0; j < b->length; j++) {
            size_t idx = i + j;
            cast_t product = static_cast<cast_t>(a->limbs[i]) * b->limbs[j];
            product += static_cast<cast_t>(result->limbs[idx]);
            product += carry;
            
            result->limbs[idx] = static_cast<limb_t>(product);
            carry = static_cast<limb_t>(product >> BASE_BITS);
        }
        
        // Обработка оставшегося переноса
        if (carry) {
            size_t idx = i + b->length;
            cast_t sum = static_cast<cast_t>(result->limbs[idx]) + carry;
            result->limbs[idx] = static_cast<limb_t>(sum);
        }
    }
    
    result->length = result_len;
    trim_leading_zeros(result);
}

void integer_div(const integer* dividend, const integer* divisor, 
                integer* quotient, integer* remainder) {
    // Проверка деления на ноль
    if (divisor->length == 0 || (divisor->length == 1 && divisor->limbs[0] == 0)) {
        throw std::domain_error("Division by zero");
    }
    
    // Деление на 1
    if (divisor->length == 1 && divisor->limbs[0] == 1) {
        integer_copy(dividend, quotient);
        integer_set_uint(remainder, 0);
        return;
    }
    
    // Если делимое меньше делителя
    if (integer_compare(dividend, divisor) < 0) {
        integer_set_uint(quotient, 0);
        integer_copy(dividend, remainder);
        return;
    }
    
    // Инициализация временных переменных
    integer temp;
    integer_copy(dividend, &temp);
    integer_set_uint(quotient, 0);
    integer one;
    integer_init_size(&one, 1);
    
    // Алгоритм деления вычитанием
    while (integer_compare(&temp, divisor) >= 0) {
        integer_sub(&temp, divisor, &temp);
        integer_add(quotient, &one, quotient);
    }
    
    // Копирование остатка
    integer_copy(&temp, remainder);
    
    // Освобождение ресурсов
    integer_free(&temp);
    integer_free(&one);
}

void integer_lshift(integer* num, size_t bits) {
    if (bits == 0) return;
    
    size_t limb_shift = bits / BASE_BITS;
    size_t bit_shift = bits % BASE_BITS;
    
    size_t new_length = num->length + limb_shift + (bit_shift ? 1 : 0);
    ensure_capacity(num, new_length);
    
    // Сдвиг целых лимбов
    if (limb_shift > 0) {
        // Сдвигаем существующие данные вправо
        for (size_t i = num->length; i-- > 0;) {
            num->limbs[i + limb_shift] = num->limbs[i];
        }
        // Обнуляем освободившееся пространство
        std::memset(num->limbs, 0, limb_shift * sizeof(limb_t));
        num->length += limb_shift;
    }
    
    // Биттовый сдвиг
    if (bit_shift > 0) {
        limb_t carry = 0;
        for (size_t i = 0; i < num->length; i++) {
            cast_t value = static_cast<cast_t>(num->limbs[i]) << bit_shift;
            num->limbs[i] = static_cast<limb_t>(value) | carry;
            carry = static_cast<limb_t>(value >> BASE_BITS);
        }
        
        // Обработка оставшегося переноса
        if (carry) {
            num->limbs[num->length] = carry;
            num->length++;
        }
    }
    
    trim_leading_zeros(num);
}

void integer_rshift(integer* num, size_t bits) {
    if (bits == 0) return;
    if (bits >= num->length * BASE_BITS) {
        integer_set_uint(num, 0);
        return;
    }
    
    size_t limb_shift = bits / BASE_BITS;
    size_t bit_shift = bits % BASE_BITS;
    
    // Проверка на полный сдвиг лимбов
    if (limb_shift >= num->length) {
        integer_set_uint(num, 0);
        return;
    }
    
    // Сдвиг целых лимбов
    if (limb_shift > 0) {
        num->length -= limb_shift;
        for (size_t i = 0; i < num->length; i++) {
            num->limbs[i] = num->limbs[i + limb_shift];
        }
    }
    
    // Биттовый сдвиг
    if (bit_shift > 0) {
        limb_t carry = 0;
        for (size_t i = num->length; i-- > 0;) {
            limb_t value = num->limbs[i];
            num->limbs[i] = (value >> bit_shift) | carry;
            carry = value << (BASE_BITS - bit_shift);
        }
    }
    
    trim_leading_zeros(num);
}

void integer_inc(integer* num) {
    integer one;
    integer_init_size(&one, 1);
    integer_add(num, &one, num);
    integer_free(&one);
}

void integer_dec(integer* num) {
    integer one;
    integer_init_size(&one, 1);
    integer_sub(num, &one, num);
    integer_free(&one);
}

std::string integer_to_string(const integer* num, size_t base) {
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Base must be between 2 and 36");
    }
    
    // Специальный случай для нуля
    if (num->length == 1 && num->limbs[0] == 0) {
        return "0";
    }
    
    std::string result;
    integer temp;
    integer_copy(num, &temp);
    integer base_num;
    integer_init_size(&base_num, base);
    
    while (temp.length > 1 || temp.limbs[0] > 0) {
        integer quotient, remainder;
        integer_init(&quotient);
        integer_init(&remainder);
        
        integer_div(&temp, &base_num, &quotient, &remainder);
        
        // Преобразование цифры в символ
        char digit = HEX_TO_CHAR[remainder.limbs[0]];
        result.push_back(digit);
        
        // Обновление временных переменных
        integer_move(&quotient, &temp);
        
        integer_free(&quotient);
        integer_free(&remainder);
    }
    
    // Освобождение ресурсов
    integer_free(&temp);
    integer_free(&base_num);
    
    // Реверс результата
    std::reverse(result.begin(), result.end());
    return result;
}

std::string integer_to_hex(const integer* num) {
    if (num->length == 1 && num->limbs[0] == 0) {
        return "0";
    }
    
    std::string result;
    char buf[17];
    
    // Обработка старшего лимба без ведущих нулей
    snprintf(buf, sizeof(buf), PRINT_LIMBHEX_NOPAD, num->limbs[num->length - 1]);
    result.append(buf);
    
    // Обработка остальных лимбов
    for (size_t i = num->length - 1; i-- > 0;) {
        snprintf(buf, sizeof(buf), PRINT_LIMBHEX, num->limbs[i]);
        result.append(buf);
    }
    
    return result;
}

size_t integer_byte_size(const integer* num) {
    size_t bits = integer_bit_size(num);
    return (bits + 7) / 8;
}

size_t integer_bit_size(const integer* num) {
    // Специальный случай для нуля
    if (num->length == 1 && num->limbs[0] == 0) {
        return 0;
    }
    
    // Определение количества битов в старшем лимбе
    limb_t top = num->limbs[num->length - 1];
    size_t count = 0;
    
    while (top) {
        count++;
        top >>= 1;
    }
    
    return (num->length - 1) * BASE_BITS + count;
}

void integer_remove_zeros(integer* num) {
    trim_leading_zeros(num);
}

limb_t* integer_detach(integer* num) {
    limb_t* detached = num->limbs;
    num->capacity = 0;
    num->length = 0;
    num->limbs = nullptr;
    return detached;
}

} // namespace apa

#endif // APA_INTEGER_CPP