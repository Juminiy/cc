# CREPL

## 输入一行
1. 函数定义, 编译加载进程空间
2. 表达式，求值输出

## env
工作目录: /tmp/crepl
glibc API: mkstemp
unistd API: execl, execlp, execle, execv, execvp, execvpe

我的思路: crepl.c程序持续监听输入：
1. 是：表达式求值输出
   1. 变量累计:
   2. 函数调用: /tmp/crepl/eval.c
      1. 重写文件 
      ```c
        #include "deps.h"
        int main(){
            func_call(...);
            return 0;
        }
      ```
      2. 编译链接
        execlp("cc", "cc", "-o", "funcs.o", "funcs.c", NULL); 
        execlp("cc", "cc", "-o", "eval.d", "eval.c", "funcs.o", NULL);
      3. 获取输出统计入变量累计
        execlp("eval.d", "./eval.d", NULL);
2. 是：函数定义
   1. 将此行函数Append进/tmp/crepl/funcs.c,即时编译成/tmp/repl/funcs.o
   2. 将函数签名Append进/tmp/crepl/deps.h 

3. 自定义功能:
   1. #include <...> 编译检测正确性
   2. clear, cls 清屏
   3. Ctrl+D, Ctrl+C, exit, !q, quit 退出
   4. 错误输出：函数重定义错误,函数定义语法错误,表达式错误

4. shell功能
   1. 光标上下键