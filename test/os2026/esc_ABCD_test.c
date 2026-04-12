#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// 设置终端为非规范模式（无回显、立即读取）
void set_terminal_raw(struct termios *old_tio) {
    struct termios new_tio;
    tcgetattr(STDIN_FILENO, old_tio);           // 保存旧设置
    new_tio = *old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);       // 非规范模式 + 关闭回显
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// 恢复终端设置
void restore_terminal(struct termios *old_tio) {
    tcsetattr(STDIN_FILENO, TCSANOW, old_tio);
}

void read_loop(){
    struct termios old_tio;

    set_terminal_raw(&old_tio);

    char c;
    while (1) {
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n <= 0) break;

        if (c == '\033') {  // ESC 字符
            char seq[2];
            // 读取后续两个字符（超时处理省略）
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[') {
                if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                    // 方向键：A(上)、B(下)、C(右)、D(左)
                    if (seq[1] == 'A' || seq[1] == 'B' || seq[1] == 'C' || seq[1] == 'D') {
                        // 忽略方向键，不输出任何内容
                        continue;
                    }
                    // 其他 ESC [ X 序列也忽略
                    continue;
                }
            }
            // 如果不是方向键序列，可以选择回显 ESC，但这里忽略
            continue;
        }

        // 可打印字符或换行符等回显
        if (c == '\n' || c == '\r' || (c >= 32 && c <= 126)) {
            putchar(c);
            fflush(stdout);
        }
        // 其他控制字符（如退格、删除等）可根据需要处理
    }

    restore_terminal(&old_tio);
}

int main() {
    setbuf(stdout, NULL); // 禁用输出缓冲

    read_loop();

    return 0;
}
