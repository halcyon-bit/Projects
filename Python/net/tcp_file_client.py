import socket

def main():
    # 创建套接字
    tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 连接服务器
    server_ip = input("请输入服务器IP: ")
    tcp_client_socket.connect((server_ip, 3356))
    # 发送下载文件的请求
    file_name = input("请输入要下载的文件名: ")
    # 发送文件下载请求数据
    tcp_client_socket.send(file_name.encode('utf-8'))
    # 接受要下载的文件信息
    file_info = tcp_client_socket.recv(1024).decode("utf-8")
    if file_info == 'no':
        print("文件不存在!!!")
    else:
        print(file_info)
        # 获取文件大小
        file_size = float(file_info.split(':')[2].split('MB')[0]) * 1024
        options = input("是否下载?(y 确认 q 取消): ")
        if options == 'q':
            print("下载取消! 程序退出!")
        else:
            print("正在下载...")
            tcp_client_socket.send(b'y')

            # 把数据写入到文件里
            with open("./" + file_name, "wb") as file:
                # 目前接收到的数据包数目
                cur_num = 0

                while True:
                    file_data = tcp_client_socket.recv(1024)
                    if file_data:
                        file.write(file_data)
                        cur_num = cur_num+1
                        progress = cur_num / file_size * 100
                        print(f"当前已下载: {progress:.2f}%")
                    else:
                        print("下载完成!")
                        break

    tcp_client_socket.close()

if __name__ == '__main__':
    main()