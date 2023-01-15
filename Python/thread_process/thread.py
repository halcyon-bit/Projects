# -*- coding: utf-8 -*-
# 线程
import time
import threading

# 导入 log 包
import os, sys
LOG_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(LOG_DIR)
from log import log

logging = log.Logger("thread", log.logging.DEBUG, log.logging.DEBUG)

# 全局变量(被多个线程共享)
num = 0

# 互斥锁
mutex = threading.Lock()

# 通过继承实现线程
class MyThread(threading.Thread):
    # 实现 run 方法
    def run(self):
        logging.debug("MyThread::run")
        time.sleep(5)

# 线程函数
def work_1(n):
    global num
    for _ in range(n):
        mutex.acquire()  # 上锁
        num += 1
        mutex.release()  # 解锁
    logging.debug(f'work_1: num = {num}')

def work_2():
    global num
    for _ in range(10):
        mutex.acquire() # 上锁
        num += 1
        mutex.release() # 解锁
        time.sleep(1)
    logging.debug(f'work_2: num = {num}')

def main():
    # 创建线程
    thd1 = threading.Thread(target=work_1, args=(10000,))
    thd2 = threading.Thread(target=work_2)

    # 启动线程
    thd1.start()
    thd2.start()

    # 等待线程执行完成
    thd1.join()

    # 查看当前线程数量：threading.enumerate()
    logging.debug(f'当前线程数量: {len(threading.enumerate())}')
    thd2.join()

    thd3 = MyThread()
    thd3.start()
    thd3.join()

if __name__ == '__main__':
    main()