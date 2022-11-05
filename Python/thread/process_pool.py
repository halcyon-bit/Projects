# -*- coding: utf-8 -*-
# 进程池
import os, time
import random
from multiprocessing import Pool, Manager

def worker(msg):
    # 记录当前程序开始执行的时间
    p_start = time.time()
    print(f'{msg}开始执行, 进程号为: {os.getpid()}')
    time.sleep(random.random() * 2)
    p_stop = time.time()
    print(msg, f'执行完毕, 耗时: {p_stop - p_start}')

def process_pool_1():
    main_start = time.time()
    po = Pool(3)
    for i in range(10):
        # 如何在进程池中运行代码
        # 异步执行 apply_async
        po.apply_async(worker, (i,))
        # 同步执行
        # po.apply(worker, (i,))

    po.close()
    # 等待进程池中的执行完毕之后主进程解堵塞
    # join必须要在关闭进程池之后使用
    po.join()
    main_stop = time.time()
    print('程序耗时:', main_stop - main_start)



# 写数据
def writer(q):
    print(f'写入进程启动, 进程为: {os.getpid()}')
    for i in 'abcdefg':
        q.put(i)


def reader(q):
    print(f'读取进程启动, 进程为: {os.getpid()}')
    for i in range(q.qsize()):
        print(f'获取到的元素为: {q.get()}')


def process_pool_2():
    q = Manager().Queue()
    po = Pool()
    po.apply_async(writer, (q,))
    time.sleep(2)
    po.apply_async(reader, (q,))
    po.close()
    po.join()

if __name__ == '__main__':
    process_pool_1()
    process_pool_2()