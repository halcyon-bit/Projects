import os
import socket
import threading

# 处理客户端请求下载文件的操作
def deal_client_request(ip_port, client_socket):
    print(f"客户端连接成功: {ip_port}")
    # 接收客户端的请求
    file_name = client_socket.recv(1024).decode("utf-8")
    # 判断文件是否存在
    if os.path.exists(file_name):
        # 输出文件字节数
        file_size = os.path.getsize(file_name)
        # 要传输的文件信息
        send_data = f"文件名: {file_name} 文件大小: {(file_size/float(1024*1024)):.2f}MB"
        # 发送和打印文件信息
        client_socket.send(send_data.encode("utf-8"))
        print(f"请求{send_data}")
        # 接受客户是否需要下载
        options = client_socket.recv(1024).decode("utf-8")
        if options == "y":
            # 打开文件
            with open(file_name, "rb") as f:
                # 计算总数据包数目
                nums = file_size / 1024
                # 当前传输的数据包数目
                cur_num = 0

                while True:
                    file_data = f.read(1024)
                    cur_num += 1
                    progress = cur_num/nums*100
                    print(f"当前已发送: {progress:.2f}%")
                    if file_data:
                        client_socket.send(file_data)
                    else:
                        print("请求的文件数据发送完成")
                        break
        else:
            print("下载取消")
    else:
        client_socket.send(b'no')
        print("下载的文件不存在")
    client_socket.close()

if __name__ == '__main__':
    # 把工作目录切换到data目录下
    os.chdir("./data")
    # 创建套接字
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 绑定端口
    tcp_server_socket.bind(("", 3356))
    # 设置监听，将主动套接字变为被动套接字
    tcp_server_socket.listen(128)

    # 循环调用accept，可以支持多个客户端同时连接，和多个客户端同时下载文件
    while True:
        client_socket, ip_port = tcp_server_socket.accept()
        # 创建子线程
        sub_thread = threading.Thread(target=deal_client_request, args=(ip_port, client_socket))
        # 启动子线程
        sub_thread.start()
    
