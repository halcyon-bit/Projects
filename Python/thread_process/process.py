# -*- coding: utf-8 -*-
# 进程
import time
# Queue 为进程间的队列
from multiprocessing import Process, Queue

# 导入 log 包
import os, sys
LOG_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(LOG_DIR)
from log import log

logging = log.Logger("process", log.logging.DEBUG, log.logging.DEBUG)

# 进程间不共享全局变量
nums = [1, 2]

def process_1(q, name, arg, **kwargs):
    logging.debug(f"子进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}, 参数: name={name}, arg={arg}, kwargs={kwargs}")
    
    for i in range(3):
        nums.append(i)
    logging.debug(f"子进程[pid]: {os.getpid()}, 全局变量：{nums}")

    for value in range(10):
        q.put(value)
        logging.debug(f"子进程[pid]: {os.getpid()}, 塞入的元素为: {value}")
        time.sleep(1)


def process_2(q):
    time.sleep(1)
    logging.debug(f"子进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}, 全局变量: {nums}")

    while True:
        if not q.empty():
            logging.debug(f"子进程[pid]: {os.getpid()}, 获取到的元素为: {q.get()}")
            time.sleep(2)
        else:
            break

if __name__ == '__main__':
    # 队列
    q = Queue()

    # 创建进程对象
    work_1 = Process(target=process_1, args=(q, "test", 123), kwargs={"info": "hello process 1"})
    # 启动进程
    work_1.start()
    
    work_2 = Process(target=process_2, args=(q, ))
    work_2.start()

    time.sleep(1)
    logging.debug(f"主进程[pid]: {os.getpid()}, 父进程[pid]: {os.getppid()}, 全局变量：{nums}")
    time.sleep(1)

    work_1.join()
    work_2.join()