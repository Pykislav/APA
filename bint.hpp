#ifndef APA_BINT_HPP
#define APA_BINT_HPP

#include "integer.hpp"
#include <cstddef>
#include <string>
#include <stdexcept>

namespace apa {

// Константы знаков
constexpr limb_t BINT_POSITIVE = 0;
constexpr limb_t BINT_NEGATIVE = 1;

// Структура знакового большого целого числа
typedef struct {
    integer number;   // Беззнаковое целое (модуль числа)
    limb_t sign;      // Знак числа (0 = положительное, 1 = отрицательное)
} bint;

// Глобальные константы (определены в .cpp)
extern const bint APA_BINT_ZERO;
extern const bint APA_BINT_ONE;
extern const bint APA_BINT_TWO;
extern const bint APA_BINT_TEN;

// ====================== Инициализация и управление памятью ======================
void bint_init(bint* num);
void bint_init_int(bint* num, int value);
void bint_init_string(bint* num, const char* str);
void bint_copy(const bint* src, bint* dest);
void bint_move(bint* src, bint* dest);
void bint_free(bint* num);

// ====================== Операции сравнения ======================
int bint_compare(const bint* a, const bint* b);

// ====================== Арифметические операции ======================
void bint_add(const bint* a, const bint* b, bint* result);
void bint_sub(const bint* a, const bint* b, bint* result);
void bint_mul(const bint* a, const bint* b, bint* result);
void bint_negate(bint* num);

// ====================== Инкремент/декремент ======================
void bint_inc(bint* num);
void bint_dec(bint* num);

// ====================== Битовые операции ======================
void bint_lshift(bint* num, size_t bits);
void bint_rshift(bint* num, size_t bits);

// ====================== Преобразования и вывод ======================
std::string bint_to_string(const bint* num, size_t base = 10);
std::string bint_to_hex(const bint* num);

// ====================== Вспомогательные функции ======================
size_t bint_byte_size(const bint* num);
size_t bint_bit_size(const bint* num);
const limb_t* bint_limb_view(const bint* num);
limb_t* bint_detach(bint* num);

} // namespace apa

// Макросы для совместимости со старым кодом
#define __BINT_ZERO apa::APA_BINT_ZERO
#define __BINT_ONE apa::APA_BINT_ONE
#define __BINT_TWO apa::APA_BINT_TWO
#define __BINT_TEN apa::APA_BINT_TEN

#endif // APA_BINT_HPP