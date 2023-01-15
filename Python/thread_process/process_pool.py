# -*- coding: utf-8 -*-
# 进程池
import time
import random
from multiprocessing import Pool, Manager

# 导入 log 包
import os, sys
LOG_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(LOG_DIR)
from log import log

logging = log.Logger("process_pool", log.logging.DEBUG, log.logging.DEBUG)

def worker(msg):
    start_time = time.time()
    logging.debug(f'[{msg}] 开始执行, 进程号为: {os.getpid()}')
    time.sleep(random.random() * 2)
    stop_time = time.time()
    logging.debug(f'[{msg}] 执行完毕, 耗时: {stop_time - start_time}')

# 进程池测试_1
def process_pool_1():
    main_start = time.time()
    # 创建有3个进程的进程池
    pool = Pool(3)
    for i in range(10):
        # 异步执行，任务函数，函数参数
        pool.apply_async(worker, (i,))
        # 同步执行
        # pool.apply(worker, (i,))

    # 设置进程池为关闭状态
    pool.close()
    # 等待进程池中的执行完毕之后主进程解堵塞
    # join 必须要在关闭进程池之后使用
    pool.join()
    main_stop = time.time()
    logging.debug(f'主进程耗时: {main_stop - main_start}')


#------------------------------------------------------------------------
# 进程池测试_2，进程间通信
# 写数据
def writer_process(q):
    logging.debug(f'写入进程启动, 进程为: {os.getpid()}')
    for i in 'abcdefg':
        q.put(i)

# 读数据
def reader_process(q):
    logging.debug(f'读取进程启动, 进程为: {os.getpid()}')
    for _ in range(q.qsize()):
        logging.debug(f'获取到的元素为: {q.get()}')

def process_pool_2():
    # 进程间使用队列通信
    q = Manager().Queue()
    pool = Pool()
    pool.apply_async(writer_process, (q,))
    pool.apply_async(reader_process, (q,))
    pool.close()
    pool.join()

if __name__ == '__main__':
    process_pool_1()
    process_pool_2()