#include"TCP_server.h"


bool receve_shuju(uint8_t* recv_buf,int recv_len)
{
	string fanhui;

	uint8_t L = uint8_t(recv_len) - 0x06;//LΪMBAP���ֽڳ��ȣ������ж϶������������Ƿ���ȷ

	uint8_t k = 0x09; //�����豸�ţ������ˣ�������1-247��
	if (recv_buf[6] == k && recv_buf[5] == L)
	{
		if (recv_buf[7] == 0x01 || recv_buf[7] == 0x03 || recv_buf[7] == 0x10 || recv_buf[7] == 0x0F)
		{
#pragma region	0x01(����Ȧ)
			if (recv_buf[7] == 0x01)//���ж��ǲ������������
			{
#pragma region	������Ȧ��ʼ��ַ		
				int xianquanquzhi1 = recv_buf[8];
				int xianquanquzhi2 = recv_buf[9];
				int xianquanquzhi3 = 16 * 16 * xianquanquzhi1 + xianquanquzhi2;
#pragma endregion

#pragma region �������Ȧ����
				int xianquangeshu1 = recv_buf[10];
				int xianquangeshu2 = recv_buf[11];
				int xianquangeshu3 = 16 * 16 * xianquangeshu1 + xianquangeshu2;
#pragma endregion
				int qishidizhijiaxianquanshu = xianquanquzhi3 + xianquangeshu3;//��ʼ��ַ����Ȧ����
				if (xianquangeshu3 >= 1 && xianquangeshu3 <= 2000) //��Ȧ������1~2000��������ʼ��ַ����Ȧ�����ܴ����ܿռ���
				{
					if (qishidizhijiaxianquanshu <= 9999)
					{
						cout<< "�ͻ��˷��͵�������ȷ"<<endl;
						return TRUE;
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						cout << "ָ�������ݵ�ַ���ڴ��豸�У�"<<endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		ָ�������ݳ�����Χ	
				else
				{
					cout << "ָ�������ݳ�����Χ��" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion

#pragma region	0x03(�����ּĴ���)
			if (recv_buf[7] == 0x03)
			{
#pragma region		����Ĵ�����		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    �Ĵ�����ַ
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int zongshuliang = 16 * (jicunqishuliang3 + jicunqidizhi3);
				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 125)
				{
					if (zongshuliang <= 159984)
					{
						cout << "�ͻ��˷��͵�������ȷ" << endl;
						return TRUE;
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						cout << "ָ�������ݵ�ַ���ڴ��豸�У�" << endl;
						return FALSE;
					}
#pragma endregion
				}

#pragma region		ָ�������ݳ�����Χ	
				else
				{
					cout << "ָ�������ݳ�����Χ��" << endl;
					return FALSE;
				}
#pragma endregion
			}

#pragma endregion

#pragma region	0x0f(д��Ȧ)
			if (recv_buf[7] == 0x0F)
			{
#pragma region		������Ȧ��		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    �Ĵ�����ַ
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int qishidizhijiaxianquanshu = jicunqishuliang3 + jicunqidizhi3;//��ʼ��ַ����Ȧ����

				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 1968)
				{
					if (qishidizhijiaxianquanshu <= 9999)
					{			
						cout << "�ͻ��˷��͵�������ȷ" << endl;
						return TRUE;
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						cout << "ָ�������ݵ�ַ���ڴ��豸�У�" << endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		ָ�������ݳ�����Χ	
				else
				{
					cout << "ָ�������ݳ�����Χ��" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion

#pragma region	0x10(д���ּĴ���)
			if (recv_buf[7] == 0x10)
			{
#pragma region		����Ĵ�����		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    �Ĵ�����ַ
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int zongshuliang = 16 * (jicunqishuliang3 + jicunqidizhi3);
				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 123)
				{
					if (zongshuliang <= 159984)
					{
						cout << "�ͻ��˷��͵�������ȷ" << endl;
						return TRUE;
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						cout << "ָ�������ݵ�ַ���ڴ��豸�У�" << endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		ָ�������ݳ�����Χ	
				else
				{
					cout << "ָ�������ݳ�����Χ��" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion 
		}
#pragma region		��վ�豸��֧�ִ˹�����	
		else
		{
			cout<<"��վ�豸��֧�ִ˹����룡"<<endl;
			return FALSE;
		}
#pragma endregion
	}
#pragma region	���豸����	       
	else
	{
	cout << "���豸���ϣ�" << endl;
	return FALSE;
	}
#pragma endregion
}