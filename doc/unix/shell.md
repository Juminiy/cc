# shell-CLI

## 实现 shell的程序
1. gun_bash: https://github.com/gitGNU/gnu_bash
2. zsh: https://github.com/zsh-users/zsh
3. WSL: https://github.com/microsoft/WSL
4. git: https://github.com/git/git
5. msys2: https://github.com/msys2/msys2-runtime
6. cygwin: https://cygwin.org/install.html

## builtin-cmd
内建命令在shell里,用于改变shell行为,不能用fork-execve模型否则执行子进程而非shell自身
1. cd,exit,export,unset,echo,pwd
## 外部命令: coreutils
https://www.gnu.org/software/coreutils/#source

## 实现Shell-CLI基本设计原则:
1. 字符显示: termios.h, ANSI读取char转义显示
2. 语法制导翻译器: 贪心匹配 '||','|','>>','>' + 递归下降分析
3. 处理PATH: 所有envp搜索,access(filename)判断
4. 执行: fork-pipe-dup-dup2-exec*-close,管道连接IPC
5. 中断信号: SIGABRT,SIGALRM,SIGBUS,SIGCHLD,SIGCLD...

## exec*-family
execve如何实现execvp, filepath不以'/'开头则搜索envp参数,access()==0返回

## signal
https://man7.org/linux/man-pages/man7/signal.7.html