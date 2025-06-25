#ifndef APA_INTEGER_HPP
#define APA_INTEGER_HPP

#include "config.hpp"
#include <cstddef>
#include <string>
#include <stdexcept>

namespace apa {

// Константы систем счисления
constexpr static size_t BIN = 2;
constexpr static size_t OCT = 8;
constexpr static size_t DEC = 10;
constexpr static size_t HEX = 16;
constexpr static size_t EMPTY = 1;
constexpr static size_t INVALID = 20;

// Размеры и лимиты
constexpr static size_t INITIAL_LIMB_CAPACITY = 2;
constexpr static size_t INITIAL_LIMB_LENGTH = 1;
constexpr static size_t LIMB_GROWTH = 2;
constexpr static size_t BASE_BITS = (sizeof(limb_t) * 8);
constexpr static size_t BASE_BITS_MINUS1 = BASE_BITS - 1;
constexpr static size_t BASE_BYTES = BASE_BITS / 8;
constexpr static size_t CAST_BITS = BASE_BITS * 2;
constexpr static limb_t BASE_MAX = std::numeric_limits<limb_t>::max();

// Таблицы преобразования (объявлены в .cpp)
extern const unsigned char CHAR_TO_HEX[127];
extern const unsigned char HEX_TO_CHAR[16];

// Структура большого целого числа без знака
typedef struct {
    size_t capacity;  // Выделенная емкость (в лимбах)
    size_t length;    // Фактическая длина числа (в лимбах)
    limb_t* limbs;    // Массив лимбов (младшие лимбы в начале)
} integer;

// Глобальные константы (определены в .cpp)
extern const integer APA_INTEGER_ZERO;
extern const integer APA_INTEGER_ONE;
extern const integer APA_INTEGER_TWO;
extern const integer APA_INTEGER_TEN;

// ====================== Управление памятью ======================
void integer_init(integer* num);
void integer_init_size(integer* num, size_t value);
void integer_init_string(integer* num, const std::string& value, size_t base = 10);
void integer_copy(const integer* src, integer* dest);
void integer_move(integer* src, integer* dest);
void integer_free(integer* num);
void integer_set_uint(integer* num, limb_t value);

// ====================== Операции сравнения ======================
int integer_compare(const integer* a, const integer* b);

// ====================== Арифметические операции ======================
void integer_add(const integer* a, const integer* b, integer* result);
void integer_sub(const integer* a, const integer* b, integer* result);
void integer_mul(const integer* a, const integer* b, integer* result);
void integer_div(
    const integer* dividend, 
    const integer* divisor, 
    integer* quotient, 
    integer* remainder
);
void integer_inc(integer* num);
void integer_dec(integer* num);

// ====================== Битовые операции ======================
void integer_lshift(integer* num, size_t bits);
void integer_rshift(integer* num, size_t bits);

// ====================== Преобразования и вывод ======================
std::string integer_to_string(const integer* num, size_t base = 10);
std::string integer_to_hex(const integer* num);

// ====================== Вспомогательные функции ======================
size_t integer_byte_size(const integer* num);
size_t integer_bit_size(const integer* num);
void integer_remove_zeros(integer* num);
limb_t* integer_detach(integer* num);

} // namespace apa

// Макросы для совместимости со старым кодом
#define __INTEGER_ZERO apa::APA_INTEGER_ZERO
#define __INTEGER_ONE apa::APA_INTEGER_ONE
#define __INTEGER_TWO apa::APA_INTEGER_TWO
#define __INTEGER_TEN apa::APA_INTEGER_TEN

#endif // APA_INTEGER_HPP