typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

/**
 * \brief Clamps a value within a specified range.
 *
 * \param min The minimum value of the range.
 * \param val The value to be clamped.
 * \param max The maximum value of the range.
 * \return The clamped value.
 *
 * This function ensures that the input value `val` remains within the specified range defined by `min` and `max`.
 * If the value `val` is less than `min`, the function returns `min`. If `val` is greater than `max`, the function
 * returns `max`. Otherwise, it returns `val` unchanged.
 */
inline int clamp(int min, int val, int max) { // Функция фиксации пикселя
    if (val < min) return min;
    if (val > max) return max;
    return val;
}