Questions:
1. execve 如何找到命令的envp
2. fork分支父进程需要等待子进程执行完毕
3. 父进程每隔100ms读一次子进程的输出内容,从stdout读取?

Ask:
1. execve 如何实现 execlp 的功能
```c
    execve();
    execl();
    execle();
    execlp();
    execv();
    execvp();
```
2. 子进程 `strace some_exe some_args ... NULL` 通过管道 stdout输出到父进程的是 `some_exe some_args ...`的stdout, strace的结果仍然保留在stdout, 为什么?
3. 获取每一个系统调用运行的时间

Answer:
1. execve 比较灵活, 缺点是filepath必须绝对路径; execlp 使用常量的命令执行
2. 因为strace的输出自身是stderr, 
3. `/bin/strace strace ls` strace执行输出到stderr, ls执行输出到stdout