#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

void read3(){
    struct termios old_tio, new_tio;
    char buf[3];
    
    set_terminal_raw(&old_tio);
    printf("Press arrow keys (Ctrl+C or 'q' to quit):\n");
    
    while (1) {
        read(STDIN_FILENO, buf, 3);  // 读取3个字符（如 \033[A）
        
        if (buf[0] == '\033' && buf[1] == '[') {
            switch (buf[2]) {
                case 'A': printf("Up\n"); break;
                case 'B': printf("Down\n"); break;
                case 'C': printf("Right\n"); break;
                case 'D': printf("Left\n"); break;
                default: break;
            }
        } else if (buf[0] == 'q') {
            break;
        }
    }
    
    restore_terminal(&old_tio);
}

int main() {

    read3();

    return 0;
}
