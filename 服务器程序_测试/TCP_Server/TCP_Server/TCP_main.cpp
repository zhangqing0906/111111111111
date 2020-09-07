#include"TCP_server.h"

bitset<9999> bitset1;//����Ȧ�ռ�
bitset<159984> bitset2;//�����ּĴ����ռ�

int main()
{
#pragma region  ����
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	uint8_t send_buf[260];
	uint8_t recv_buf[260];//TCPЭ����Ϣ֡����󳤶�Ϊ260�����Ӧ���ǿͻ��˿��ǵ����⣬��ֻ����һ�����ܵĴ洢��������260
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;//AF_INET������Ҫ��ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ������
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(502);
#pragma endregion  

#pragma region �׽���       
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);



	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
	{
		cout << "�׽��ְ󶨳ɹ���" << endl;
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0)
	{
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
	{
		cout << "���ü���״̬�ɹ���" << endl;
	}
	cout << "��������ڼ������ӣ����Ժ�...." << endl;
	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR)
	{
		cout << "����ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	cout << "���ӽ�����׼����������" << endl;
#pragma endregion  

	//��������
	while (1)
	{
		uint8_t k = 0x09; //�����豸�ţ������ˣ�������1-247��
		memset(recv_buf, 0, 260);//��recv_buf��������	
		recv_len = recv(s_accept, (char*)recv_buf, 260, 0);
		if (recv_len < 0)
		{
			cout << "�ͻ�����Ϣ:" << endl;
			return 0;
		}

		bool receve_zhenjia= receve_shuju(recv_buf,recv_len);	
		
		if (receve_zhenjia == TRUE)//��ʾ���ͻ��˷��͵�������ȷ
		{
			 send_len= send_length(recv_buf, send_buf, recv_len);
			 if (send_len < 0)
			 {
				 cout << "����ʧ�ܣ�" << endl;
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
				cout << "����ʧ�ܣ�" << endl;
				break;
			}
			else
			{
				send(s_accept, (char*)send_buf, send_len, 0);
			}
		}
	}
}