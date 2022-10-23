# -*- coding: utf-8 -*-
import socket
import threading

def server():
    # 1. 创建tcp实例对象
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 2. 绑定端口
    tcp_server_socket.bind(('', 6667))
    # 3. 等待客户端进行连接
    tcp_server_socket.listen(128)
    # 4. 如果连接成功, 则生成一个新的套接字对象负责数据接收和数据发送
    new_socket, client_address = tcp_server_socket.accept()

    while True:
        recv_data = new_socket.recv(1024)
        recv_data = recv_data.decode()
        if recv_data == 'exit':
            break
        print(f"Bob: 接受的数据为：{recv_data}")

        send_data = input('请输入你要发送的数据(退出请输入: exit): ')
        new_socket.send(send_data.encode('utf-8'))

        if send_data == 'exit':
            break

    new_socket.close()
    tcp_server_socket.close()


def client():
    tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_client_socket.connect(("127.0.0.1", 6667))
    
    while True:
        send_data = input('请输入你要发送的数据(退出请输入: exit): ')
        tcp_client_socket.send(send_data.encode('utf-8'))

        if send_data == 'exit':
            break

        recv_data = tcp_client_socket.recv(1024)
        recv_data = recv_data.decode()
        if recv_data == 'exit':
            break
        print(f"Alice: 接受的数据为：{recv_data}")

    tcp_client_socket.close()

def main():
    t1 = threading.Thread(target=server)
    t2 = threading.Thread(target=client)

    t1.start()
    t2.start()

    t1.join()
    t2.join()

if __name__ == '__main__':
    main()