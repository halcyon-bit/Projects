# -*- coding: utf-8 -*-
# 进程
import os
import time
# Queue 为进程间的队列
from multiprocessing import Process, Queue

# 进程间不共享全局变量
nums = [1, 2]

def run_process_1(q, name, arg, **kwargs):
    print(f"子进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}")
    print(f"子进程参数：name={name}, arg={arg}, kwargs={kwargs}")
    
    for i in range(3):
        nums.append(i)
    time.sleep(1)
    print(f"子进程[pid]: {os.getpid()}, 全局变量：{nums}")

    for value in range(10):
        q.put(value)
        print(f"子进程[pid]: {os.getpid()}, 塞入的元素为: {value}")
        time.sleep(1)


def run_process_2(q):
    time.sleep(1)
    print(f"子进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}, 全局变量: {nums}")

    while True:
        if not q.empty():
            value = q.get()
            print(f"子进程[pid]: {os.getpid()}, 获取到的元素为: {value}")
            time.sleep(2)
        else:
            break

if __name__ == '__main__':
    # 队列
    q = Queue()

    # 创建进程对象
    p_1 = Process(target=run_process_1, args=(q, "test", 123), kwargs={"info": "hello"})
    # 启动进程
    p_1.start()
    
    p_2 = Process(target=run_process_2, args=(q, ))
    p_2.start()

    time.sleep(1)
    print(f"主进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}, 全局变量：{nums}")
    time.sleep(1)

    p_1.join()
    p_2.join()