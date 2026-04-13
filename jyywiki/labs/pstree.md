# 完成C99标准的程序pstree
1. 完成文件pstree.c的内容
2. 把系统中的进程按照父亲-孩子的树状结构打印到终端。
3. 程序参数
```bash
-p 或 --show-pids: 打印每个进程的进程号。
-n 或 --numeric-sort: 按照 pid 的数值从小到大顺序输出一个进程的直接孩子。
-V 或 --version: 打印版本信息。
```
4. 思路: 读取系统目录 /proc/[pid]/status 的 Name,Pid,PPid, 形成进程间父子关系