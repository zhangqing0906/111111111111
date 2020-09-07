#include"TCP_server.h"

bitset<9999> bitset1;//开线圈空间
bitset<159984> bitset2;//开保持寄存器空间

int main()
{
#pragma region  定义
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//定义发送缓冲区和接受缓冲区
	uint8_t send_buf[260];
	uint8_t recv_buf[260];//TCP协议消息帧的最大长度为260，这个应该是客户端考虑的问题，我只定义一个接受的存储器最大接受260
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;//AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(502);
#pragma endregion  

#pragma region 套接字       
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);



	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "套接字绑定失败！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "套接字绑定成功！" << endl;
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else
	{
		cout << "设置监听状态成功！" << endl;
	}
	cout << "服务端正在监听连接，请稍候...." << endl;
	//接受连接请求
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "连接失败！" << endl;
		WSACleanup();
		return 0;
	}
	cout << "连接建立，准备接受数据" << endl;
#pragma endregion  

	//接收数据
	while (1)
	{
		uint8_t k = 0x09; //定义设备号（定死了）（区间1-247）
		memset(recv_buf, 0, 260);//对recv_buf进行清零	
		recv_len = recv(s_accept, (char*)recv_buf, 260, 0);
		if (recv_len < 0)
		{
			cout << "客户端信息:" << endl;
			return 0;
		}

		bool receve_zhenjia= receve_shuju(recv_buf,recv_len);	
		
		if (receve_zhenjia == TRUE)//表示：客户端发送的数据正确
		{
			 send_len= send_length(recv_buf, send_buf, recv_len);
			 if (send_len < 0)
			 {
				 cout << "发送失败！" << endl;
				 break;
			 }
			 else
			 {
				 send(s_accept, (char*)send_buf, send_len, 0);
			 }
		}
		else
		{
			send_len = send_length(recv_buf, send_buf, recv_len);
			if (send_len < 0)
			{
				cout << "发送失败！" << endl;
				break;
			}
			else
			{
				send(s_accept, (char*)send_buf, send_len, 0);
			}
		}
	}
}