#include <stdio.h>
#include <stdlib.h>

// 定义一个足够大的栈空间
// 对于n个盘子，栈的最大深度大约为n
#define MAX_STACK_SIZE 1024

// 定义栈帧结构体，用于模拟递归调用栈
typedef struct {
    int n;
    char src;
    char aux;
    char dst;
    int stage; 
} StackFrame;

// 全局栈和栈顶指针
StackFrame stack[MAX_STACK_SIZE];
int top = -1;

// 压栈操作
void push(int n, char src, char aux, char dst, int stage) {
    if (top >= MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Stack Overflow\n");
        exit(EXIT_FAILURE);
    }
    // C99 复合字面量
    stack[++top] = (StackFrame){n, src, aux, dst, stage};
}

// 弹栈操作
StackFrame pop() {
    if (top < 0) {
        fprintf(stderr, "Stack Underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack[top--];
}

// 判断栈是否为空
int is_empty() {
    return top == -1;
}

// 非递归汉诺塔实现
void hanoi_iterative(int n, char src, char aux, char dst) {
    // 初始任务入栈：将n个盘子从src移动到dst，借助aux
    push(n, src, aux, dst, 0);

    while (!is_empty()) {
        // 取出当前任务
        StackFrame f = pop();

        if (f.n <= 0) {
            continue;
        }

        // 递归的基本情况模拟：n=1时直接移动
        if (f.n == 1) {
            printf("Move disk 1 from %c to %c\n", f.src, f.dst);
            continue;
        }

        // 状态机处理不同阶段
        switch (f.stage) {
            case 0:
                // 阶段 0: 开始处理 hanoi(n, src, aux, dst)
                // 递归逻辑第一步：先处理 hanoi(n-1, src, dst, aux)
                // 为了回来继续执行，将当前状态重新入栈，stage 设为 1
                push(f.n, f.src, f.aux, f.dst, 1);
                // 压入第一步的子任务
                push(f.n - 1, f.src, f.dst, f.aux, 0);
                break;

            case 1:
                // 阶段 1: hanoi(n-1, src, dst, aux) 已完成
                // 递归逻辑第二步：移动底盘 n 从 src 到 dst
                printf("Move disk %d from %c to %c\n", f.n, f.src, f.dst);
                
                // 准备处理第三步：hanoi(n-1, aux, src, dst)
                // 将当前状态重新入栈，stage 设为 2 (表示此任务即将彻底结束)
                push(f.n, f.src, f.aux, f.dst, 2);
                // 压入第三步的子任务
                push(f.n - 1, f.aux, f.src, f.dst, 0);
                break;

            case 2:
                // 阶段 2: hanoi(n-1, aux, src, dst) 也已完成
                // 当前任务 hanoi(n, ...) 彻底结束，直接出栈丢弃
                break;
        }
    }
}

int main() {
    int n = 3;
    printf("--- Non-recursive Hanoi Tower (n=%d) ---\n", n);
    hanoi_iterative(n, 'A', 'B', 'C');
    return 0;
}