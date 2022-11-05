# -*- coding: utf-8 -*-
from concurrent.futures import ThreadPoolExecutor
import threading
import time

# 定义一个准备作为线程任务的函数
def add(max):
    sum = 0
    for i in range(max):
        print(threading.current_thread().name + "  " + str(i))
        sum += i
    return sum

def get_result(future):
    print(future.result())

# 创建一个包含4条线程的线程池
pool = ThreadPoolExecutor(max_workers=4)

# 提交任务
future1 = pool.submit(add, 10)
future2 = pool.submit(add, 20)

# 判断任务是否结束
print(future1.done())

time.sleep(3)

print(future2.done())

# 取出任务结果(会阻塞)
print(future1.result())

# 取出任务结果(非阻塞)
future2.add_done_callback(get_result)

# 一次启动多个任务
results = pool.map(add, (50, 100, 150))  # 3 个任务

for r in results:
    print(r)

# 关闭线程池
pool.shutdown()