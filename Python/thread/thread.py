# -*- coding: utf-8 -*-
import threading
import time

# 全局变量(被多个线程共享)
num = 0

# 互斥锁
mutex = threading.Lock()

# 通过继承实现线程
class MyThread(threading.Thread):
    # 实现 run 方法
    def run(self):
        print("MyThread::run")
        time.sleep(5)


def test1(n):
    global num
    for i in range(n):
        mutex.acquire()  # 上锁
        num += 1
        mutex.release()  # 解锁
    print(f'test1: num = {num}')

def test2():
    global num
    for i in range(10):
        mutex.acquire() # 上锁
        num += 1
        mutex.release() # 解锁
        time.sleep(1)
    print(f'test2: num = {num}')


def main():
    # 创建线程
    t1 = threading.Thread(target=test1, args=(1000000,))
    t2 = threading.Thread(target=test2)

    t1.start()  # 启动 t1 线程
    t2.start()

    t1.join()  # 等待 t1 线程执行结束

    # 查看当前线程数量：threading.enumerate()
    print(len(threading.enumerate()))
    t2.join()

    t3 = MyThread()
    t3.start()

if __name__ == '__main__':
    main()