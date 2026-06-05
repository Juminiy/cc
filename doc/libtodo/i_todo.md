
记录于2026.04.14建金中心,希望未来的工作能在高性能(网络/计算/存储)领域有所建树。

# TODO List
1. 调试 musllib,glibc 库函数, string/memcpy,memchr,memcmp,memcpy,memmove,printf
2. 理解glibc在现代化指令集体系结构所进行的优化策略
3. 硬件-OS内核-编译器-网络 协同优化数据加速 RDMA-IB网络协议栈

# HPC高性能计算历届竞赛题目
| 时间  | 赛事 | 题目  | 规则描述 |
| ---- | ---- | ---- | ------- |
| October 2013 | The 1st Student RDMA Programming Competition | 未披露 | 将传统基于TCP通信的Socket程序改成基于RDMA的程序 |
| October 2014 | The 2nd Student RDMA Programming Competition | 未披露 | 参赛者将传统基于TCP通信的Socket程序改成基于RDMA verbs的程序 |
| October 2015 | The 3rd Student RDMA Programming Competition | 未披露 | 未披露 |
| October 2016 | The 4th Student RDMA Programming Competition | 通过高效RDMA通讯协议，加速深度学习最新并行应用框架 |
| October 2017 | The 5th Student RDMA Programming Competition | 通过高效RDMA通讯协议，加速MXNet深度学习框架 | 
| 2018         | 6th Annual Student RDMA Competition          | 通过高效RDMA通讯协议，自行寻找一个 NoSQL 的数据库程序做相关通讯加速 | 今年的比赛将重点关注 NoSQL 应用，例如 HBASE、Cassandra、MongoDB 等 |
| May 8th - November (APAC) | 2019 APAC Student RDMA Programming Competition | 如何使用 RDMA 技术优化在容器环境下的HPC 应用 | 赛队伍可自行选择容器解决方案：Dockers 或 Singularity |
| November 15, 2020         | 8th RDMA Programming Competition | Modify one application to use the UCX interface https://spdk.io; https://github.com/pmem/pmdk, https://pmem.io/ |
| August 22 to 29, 2021     | 9th Annual APAC RDMA Lecture & Competition | DPU + UCX RDMA Practice | 1. Run one MPI instance on the host or just run the application 2. Run independent MPI on the DPU, which does the collectives 3. Communicate using UCX (client-server mode) between applic |
| Beijing, July 31, 2022    | 10th RDMA Programming Competition | Implementing BlueField offloaded small-data nonblocking MPI_Iallgather() | 1. Implement host-based MPI_Iallgather() with small-data optimization 2. Offload the algorithm to the BlueField DPU 3. Measure the overlap capacity with and without the DPU offload, with provided test code 4. Extend the algorithm to deal with large process count, and measure performance improvement |
| August 07 - 13, 2023      | 11th RDMA Programming Competition | 高性能网络,RDMA/UCX,MPI/UCC,SHARP | 课程和题目问答 |
| July 22 - 28, 2024        | 12th RDMA Programming Competition | 高性能网络,UCX,MPI,NCCL | 课程和题目问答 |
| September 4 - 6, 2025     | 13th RDMA Programming Competition | 利用 RDMA 技术实现 Qwen3 SGLang 离线模式推理吞吐量优化 | 

