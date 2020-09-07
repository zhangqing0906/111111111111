#include"TCP_server.h"


bool receve_shuju(uint8_t* recv_buf,int recv_len)
{
	string fanhui;

	uint8_t L = uint8_t(recv_len) - 0x06;//L为MBAP里字节长度，用以判断读进来的数，是否正确

	uint8_t k = 0x09; //定义设备号（定死了）（区间1-247）
	if (recv_buf[6] == k && recv_buf[5] == L)
	{
		if (recv_buf[7] == 0x01 || recv_buf[7] == 0x03 || recv_buf[7] == 0x10 || recv_buf[7] == 0x0F)
		{
#pragma region	0x01(读线圈)
			if (recv_buf[7] == 0x01)//先判断是不是这个功能码
			{
#pragma region	计算线圈起始地址		
				int xianquanquzhi1 = recv_buf[8];
				int xianquanquzhi2 = recv_buf[9];
				int xianquanquzhi3 = 16 * 16 * xianquanquzhi1 + xianquanquzhi2;
#pragma endregion

#pragma region 计算读线圈个数
				int xianquangeshu1 = recv_buf[10];
				int xianquangeshu2 = recv_buf[11];
				int xianquangeshu3 = 16 * 16 * xianquangeshu1 + xianquangeshu2;
#pragma endregion
				int qishidizhijiaxianquanshu = xianquanquzhi3 + xianquangeshu3;//起始地址加线圈个数
				if (xianquangeshu3 >= 1 && xianquangeshu3 <= 2000) //线圈数量（1~2000）并且起始地址加线圈数不能大于总空间数
				{
					if (qishidizhijiaxianquanshu <= 9999)
					{
						cout<< "客户端发送的数据正确"<<endl;
						return TRUE;
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						cout << "指定的数据地址不在从设备中！"<<endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		指定的数据超过范围	
				else
				{
					cout << "指定的数据超过范围！" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion

#pragma region	0x03(读保持寄存器)
			if (recv_buf[7] == 0x03)
			{
#pragma region		计算寄存器数		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    寄存器地址
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int zongshuliang = 16 * (jicunqishuliang3 + jicunqidizhi3);
				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 125)
				{
					if (zongshuliang <= 159984)
					{
						cout << "客户端发送的数据正确" << endl;
						return TRUE;
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						cout << "指定的数据地址不在从设备中！" << endl;
						return FALSE;
					}
#pragma endregion
				}

#pragma region		指定的数据超过范围	
				else
				{
					cout << "指定的数据超过范围！" << endl;
					return FALSE;
				}
#pragma endregion
			}

#pragma endregion

#pragma region	0x0f(写线圈)
			if (recv_buf[7] == 0x0F)
			{
#pragma region		计算线圈数		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    寄存器地址
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int qishidizhijiaxianquanshu = jicunqishuliang3 + jicunqidizhi3;//起始地址加线圈个数

				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 1968)
				{
					if (qishidizhijiaxianquanshu <= 9999)
					{			
						cout << "客户端发送的数据正确" << endl;
						return TRUE;
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						cout << "指定的数据地址不在从设备中！" << endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		指定的数据超过范围	
				else
				{
					cout << "指定的数据超过范围！" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion

#pragma region	0x10(写保持寄存器)
			if (recv_buf[7] == 0x10)
			{
#pragma region		计算寄存器数		
				int jicunqishuliang1 = recv_buf[10];
				int jicunqishuliang2 = recv_buf[11];
				int jicunqishuliang3 = 16 * 16 * jicunqishuliang1 + jicunqishuliang2;
#pragma endregion

#pragma region    寄存器地址
				int jicunqidizhi1 = recv_buf[8];
				int jicunqidizhi2 = recv_buf[9];
				int jicunqidizhi3 = 16 * 16 * jicunqidizhi1 + jicunqidizhi2;
#pragma endregion
				int zongshuliang = 16 * (jicunqishuliang3 + jicunqidizhi3);
				if (jicunqishuliang3 >= 1 && jicunqishuliang3 <= 123)
				{
					if (zongshuliang <= 159984)
					{
						cout << "客户端发送的数据正确" << endl;
						return TRUE;
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						cout << "指定的数据地址不在从设备中！" << endl;
						return FALSE;
					}
#pragma endregion
				}
#pragma region		指定的数据超过范围	
				else
				{
					cout << "指定的数据超过范围！" << endl;
					return FALSE;
				}
#pragma endregion
			}
#pragma endregion 
		}
#pragma region		从站设备不支持此功能码	
		else
		{
			cout<<"从站设备不支持此功能码！"<<endl;
			return FALSE;
		}
#pragma endregion
	}
#pragma region	从设备故障	       
	else
	{
	cout << "从设备故障！" << endl;
	return FALSE;
	}
#pragma endregion
}