# -*- coding: utf-8 -*-
# 线程池
import time
import threading
from concurrent.futures import ThreadPoolExecutor

# 导入 log 包
import os, sys
LOG_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(LOG_DIR)
from log import log

logging = log.Logger("thread_pool", log.logging.DEBUG, log.logging.DEBUG)

# 线程函数
def add(max):
    logging.debug(f'{threading.current_thread().name}: {max}')
    sum = 0
    for i in range(max):
        sum += i
    return sum


def get_result(future):
    logging.debug(f'获取任务2的结果: {future.result()}')

if __name__ == '__main__':
    # 创建大小为4的线程池
    pool = ThreadPoolExecutor(max_workers=4)

    # 提交任务
    future1 = pool.submit(add, 10)
    future2 = pool.submit(add, 20)

    # 判断任务是否结束
    logging.debug(f'任务1是否结束: {future1.done()}')
    time.sleep(3)
    logging.debug(f'任务2是否结束: {future2.done()}')

    # 取出任务结果(会阻塞)
    logging.debug(f'获取任务1的结果: {future1.result()}')

    # 取出任务结果(非阻塞)
    future2.add_done_callback(get_result)


    # 一次启动多个任务
    logging.debug('同时创建3个任务')
    results = pool.map(add, (50, 100, 150))  # 3 个任务

    for r in results:
        logging.debug(f'结果: {r}')

    # 关闭线程池
    pool.shutdown()