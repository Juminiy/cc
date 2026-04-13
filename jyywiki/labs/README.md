# compile C program to executable file
```bash
  -E                       Preprocess only; do not compile, assemble or link.
  -S                       Compile only; do not assemble or link.
  -c                       Compile and assemble, but do not link.
  -pie                     Create a dynamically linked position independent
                           executable.
  -shared                  Create a shared library.
```

# environment is important
> 任何程序没有环境都执行不了
```
PATH="" go version
PATH="" strace ls
PATH="" ls
```