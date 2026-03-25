#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// 定义返回结构体
typedef struct {
    long long i64;    // 整数值
    double f64;       // 浮点数值
    int typ;          // 类型：1=整数，2=浮点数
} NumberResult;

// 辅助函数：跳过空白字符
static const char* skip_whitespace(const char* str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || 
           *str == '\r' || *str == '\v' || *str == '\f') {
        str++;
    }
    return str;
}

// 检查字符是否是数字
static int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

// 主解析函数
NumberResult parse_number(const char* input) {
    NumberResult result = {0, 0.0, 0};
    
    // 跳过前导空白
    const char* str = skip_whitespace(input);
    
    // 检查空字符串
    if (*str == '\0') {
        return result;
    }
    
    // 检查符号
    int has_sign = 0;
    if (*str == '-' || *str == '+') {
        has_sign = 1;
        str++;
    }
    
    // 检查是否以数字开头（允许前导0）
    if (!is_digit_char(*str) && *str != '.') {
        return result;  // 不符合数字格式
    }
    
    // 解析整数部分
    int has_digits_before_decimal = 0;
    int has_digits_after_decimal = 0;
    int has_exponent = 0;
    int exponent_has_digits = 0;
    
    const char* start_ptr = str;
    
    // 处理整数部分：([1-9][0-9]*|0)
    if (is_digit_char(*str)) {
        has_digits_before_decimal = 1;
        
        if (*str == '0') {
            str++;
            // 如果只有单个0，后面不能跟其他数字（除非是小数点或指数）
            if (is_digit_char(*str)) {
                return result;  // 无效格式如 "0123"
            }
        } else {
            // [1-9][0-9]*
            while (is_digit_char(*str)) {
                str++;
            }
        }
    }
    
    // 处理小数部分：(.[0-9]*)?
    if (*str == '.') {
        str++;
        // 允许小数点后没有数字的情况
        while (is_digit_char(*str)) {
            has_digits_after_decimal = 1;
            str++;
        }
    }
    
    // 处理指数部分：([eE][-+]?[0-9]+)?
    if (*str == 'e' || *str == 'E') {
        has_exponent = 1;
        str++;
        
        // 检查指数符号
        if (*str == '+' || *str == '-') {
            str++;
        }
        
        // 指数必须至少有一个数字
        if (!is_digit_char(*str)) {
            return result;  // 无效的指数格式
        }
        
        exponent_has_digits = 1;
        while (is_digit_char(*str)) {
            str++;
        }
    }
    
    // 检查是否整个字符串都被消耗（除了可能的尾随空白）
    const char* end_ptr = str;
    str = skip_whitespace(str);
    if (*str != '\0') {
        return result;  // 字符串中有额外字符
    }
    
    // 根据格式确定类型
    // 如果有小数点或指数，则为浮点数
    if (has_digits_after_decimal || has_exponent) {
        result.typ = 2;
        
        // 使用strtod转换
        char* end_parse;
        double value = strtod(start_ptr, &end_parse);
        
        // 验证整个字符串都被解析了
        if (end_parse == end_ptr) {
            result.f64 = value;
        } else {
            result.typ = 0;  // 解析失败
        }
    } else {
        result.typ = 1;
        
        // 使用strtoll转换
        char* end_parse;
        long long value = strtoll(start_ptr, &end_parse, 10);
        
        // 验证整个字符串都被解析了
        if (end_parse == end_ptr) {
            result.i64 = value;
        } else {
            result.typ = 0;  // 解析失败
        }
    }
    
    return result;
}

// 测试函数
void test_parse_number() {
    const char* test_cases[] = {
        "123",
        "-456",
        "0",
        "3.14",
        "-2.718",
        "0.5",
        ".5",
        "123.",
        "1.23e10",
        "-4.56E-7",
        "1e100",
        "invalid",
        "12a34",
        "",
        "  123  ",
        "0123",      // 应该失败
        "1.2.3",     // 应该失败
        "1e",        // 应该失败
        "1e+",       // 应该失败
        NULL
    };
    
    for (int i = 0; test_cases[i] != NULL; i++) {
        printf("Testing: \"%s\"\n", test_cases[i]);
        NumberResult result = parse_number(test_cases[i]);
        
        switch (result.typ) {
            case 1:
                printf("  Result: INTEGER %lld\n", result.i64);
                break;
            case 2:
                printf("  Result: FLOAT %f\n", result.f64);
                break;
            default:
                printf("  Result: INVALID\n");
                break;
        }
        printf("\n");
    }
}

// 示例使用
int main() {
    test_parse_number();
    
    // 单独示例
    const char* examples[] = {"42", "-3.14", "1.23e-4", "0", 
        "-0.000000000000000000000000000000000000000000000000000000000000000000000000000001",
        "-1",
        "1e-2",
        "1e+2",
        "-0",
        "-123",
        "1E-2",
        "-0",
        "0e+1",
        "1E+2",
    };
    for (int i = 0; i < 14; i++) {
        NumberResult res = parse_number(examples[i]);
        printf("Input: %s -> Type: %d, ", examples[i], res.typ);
        if (res.typ == 1) {
            printf("i64: %lld\n", res.i64);
        } else if (res.typ == 2) {
            printf("f64: %f\n", res.f64);
        } else {
            printf("Invalid\n");
        }
    }
    
    return 0;
}
