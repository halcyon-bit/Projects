import socket
import threading

def test_1():
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
        send_data = input('请输入你要发送的数据(退出请输入: exit): ')
        udp_socket.sendto(send_data.encode('utf-8'), ('127.0.0.1', 6666))

        if send_data == 'exit':
            break

        recv_data, addr = udp_socket.recvfrom(1024)
        recv_data = recv_data.decode()
        if recv_data == 'exit':
            break
        print(f"Alice: 接受的数据为：{recv_data}")

    udp_socket.close()

def test_2():
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    udp_socket.bind(('', 6666))

    while True:
        recv_data, addr = udp_socket.recvfrom(1024)
        recv_data = recv_data.decode()
        if recv_data == 'exit':
            break
        print(f"Bob: 接受的数据为：{recv_data}")

        send_data = input('请输入你要发送的数据(退出请输入: exit): ')
        udp_socket.sendto(send_data.encode('utf-8'), addr)

        if send_data == 'exit':
            break

def main():
    t1 = threading.Thread(target=test_1)
    t2 = threading.Thread(target=test_2)

    t2.start()
    t1.start()

    t1.join()
    t2.join()

if __name__ == '__main__':
    main()