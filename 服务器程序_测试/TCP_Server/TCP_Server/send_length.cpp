#include"TCP_server.h"

int send_length(uint8_t* recv_buf, uint8_t* send_buf, int recv_len)
{
	bitset<9999> bitset1;//开线圈空间
	for (int i = 0; i < bitset1.size(); i = i + 2)//0x01和0x10共用
	{
		bitset1.set(i);//将第i + 1位变成1
	}
	bitset<159984> bitset2;//开寄存器空间，一个寄存器=16位  159984  //0x03和0x0F共用
	for (int i = 0; i < bitset2.size(); i = i + 2)//0x01和0x10共用
	{
		bitset2.set(i);//将第i + 1位变成1
	}
	
	int send_length;

	uint8_t k = 0x09; //定义设备号（定死了）（区间1-247）

	uint8_t L = uint8_t(recv_len) - 0x06;//L为MBAP里字节长度，用以判断读进来的数，是否正确

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
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						int a1 = recv_buf[8]; //uint8_t转int
						int a2 = recv_buf[9];
						int start_bits = 0;
						start_bits = start_bits + a1 * 16 * 16 + a2;//用来读取线圈起始地址
						int start_bits1 = 0;//线圈数
						int b1 = recv_buf[10];//uitn8_t转int
						int b2 = recv_buf[11];
						start_bits1 = start_bits1 + b1 * 16 * 16 + b2;//用来记要读多少个线圈
#pragma region 计算整除的
						int xunhauanshu = 0;
						if (start_bits1 % 8 == 0)
						{
							xunhauanshu = start_bits1 / 8;//用来计算因为线圈数而导致的要循环多少次
							int diwuwei = xunhauanshu + 3;
							send_buf[5] = (uint8_t)diwuwei;//int强制转为uint8_t
							send_buf[6] = recv_buf[6];
							send_buf[7] = recv_buf[7];
							send_buf[8] = (uint8_t)xunhauanshu;//int强制转为uint8_t
							for (int t = 0; t < xunhauanshu; t++)
							{
								bitset<8>a;
								unsigned int b;
								a = bitset<8>(bitset1.to_string().substr(start_bits, 8)).to_ulong();//截取bitset里面从地址19开始的8位
								b = a.to_ulong();   
								start_bits = start_bits + 8;
								send_buf[9 + t] = (uint8_t)b;

							}
						}
#pragma endregion

#pragma region 计算不能被整除的
						else
						{
							xunhauanshu = start_bits1 / 8;//用来计算因为线圈数而导致的要循环多少次
							int diwuwei = xunhauanshu + 3 + 1;
							send_buf[5] = (uint8_t)diwuwei;//int强制转为uint8_t
							send_buf[6] = recv_buf[6];
							send_buf[7] = recv_buf[7];
							send_buf[8] = (uint8_t)(xunhauanshu + 1);//int强制转为uint8_t
							for (int t = 0; t < xunhauanshu; t++)
							{
								bitset<8>a;
								unsigned int b;
								a = bitset<8>(bitset1.to_string().substr(start_bits, 8)).to_ulong();//截取bitset里面从地址19开始的8位
								//b = a.flip().to_ulong(); //翻转,再变成int型     .to_ulong()这玩意还必须加
								b = a.to_ulong();
								start_bits = start_bits + 8;
								send_buf[9 + t] = (uint8_t)b;
							}
							int start_start_bits = start_bits + xunhauanshu * start_bits1;//统计最后一个要补零的数据的起始地址
							int bulingshu = start_bits1 % 8;//计算要截取几位
							bitset<8>bulinggeshu;
							bulinggeshu = bitset<8>(bitset1.to_string().substr(start_start_bits, bulingshu)).to_ulong();
							int bulingint = bulinggeshu.to_ulong();
							send_buf[9 + xunhauanshu] = (uint8_t)bulingint; //截取的值前面8-bulingshu位补零
						}
#pragma endregion   

#pragma region	用来计算send要发送多少个字节	

						int b5 = send_buf[8];//uitn8_t转int

#pragma endregion			

						cout << "请输入回复信息:";
						return send_length = 9 + b5;

					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}

#pragma region		指定的数据超过范围	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];

						int start_bits = 0;
						int a1 = recv_buf[8]; //uint8_t转int
						int a2 = recv_buf[9];
						start_bits = (start_bits + a1 * 16 * 16 + a2) * 8;//用来读取寄存器起始地址

						int start_bits1 = 0;//寄存器数
						int b1 = recv_buf[10] * 16 * 16;//uitn8_t转int
						int b2 = recv_buf[11];
						start_bits1 = (start_bits1 + b1 * 16 * 16 + b2) * 2;//用来记要读多少个寄存器（用bit存的所以乘2）

						int diwuwei = 2 * (b1 + b2) + 3;
						send_buf[5] = (uint8_t)diwuwei;
						send_buf[6] = recv_buf[6];
						send_buf[7] = recv_buf[7];
						send_buf[8] = (uint8_t)(2 * (b1 + b2));

						for (int t = 0; t < (b1 + b2) * 2; t++)
						{
							bitset<8>a;
							unsigned int b;
							a = bitset<8>(bitset2.to_string().substr(start_bits, 8)).to_ulong();//截取bitset里面从地址19开始的8位
							b = a.to_ulong(); 
							start_bits = start_bits + 8;
							send_buf[9 + t] = (uint8_t)b;
							/*cout << "输出send_buf[9+t]=" << send_buf[9 + t] << endl;
							cout << "输出a=" << a << endl;
							cout << "输出b=" << b << endl;
							cout << "输出t=" << t << endl;*/
						}

						cout << "请输入回复信息:";
						int start_bits2 = 0;
						int b3 = recv_buf[10];//uitn8_t转int
						int b4 = recv_buf[11];
						start_bits2 = (b3 + b4) * 2;//用来记要读多少个寄存器
						return send_length = start_bits2 + 9;
						
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}

#pragma region		指定的数据超过范围	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
#pragma region	构造消息帧			
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x06);  ///
						send_buf[6] = recv_buf[6];
						send_buf[7] = recv_buf[7];
						send_buf[8] = recv_buf[8];
						send_buf[9] = recv_buf[9];
						send_buf[10] = recv_buf[10];
						send_buf[11] = recv_buf[11];
						return send_length = 12;
#pragma endregion			

#pragma region  编写写线圈
						int cishu = recv_buf[12];//表明要循环2次来把数据写进去				
						for (int i = 0; i < cishu; i++)
						{
							int wei = recv_buf[recv_len - 1 - i];//先放高位再放地位
							bitset<9999> m(wei);
							int s = recv_buf[9] + 8 * i;
							m = m <<= s;
							bitset1 = bitset1 ^= m;
						}
					/*	cout << "bitset332=" << bitset1 << endl;*/
						int buf9 = recv_buf[9];
						int buf12 = recv_buf[12];
						int tou = 9999 - buf12 * 8 - buf9;
						int zuoyi = tou - buf9;//向左移为了好看
						bitset1 = bitset1 <<= zuoyi;
						/*cout << "zuoyi=" << zuoyi << endl;
						cout << "bitset333=" << bitset1 << endl;*/
#pragma endregion
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}
#pragma region		指定的数据超过范围	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
#pragma region	构造消息帧		
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x06);
						send_buf[6] = recv_buf[6];
						send_buf[7] = recv_buf[7];
						send_buf[8] = recv_buf[8];
						send_buf[9] = recv_buf[9];
						send_buf[10] = recv_buf[10];
						send_buf[11] = recv_buf[11];
						return send_length = 12;
#pragma endregion

#pragma region	编写写寄存器
						int cishu = recv_buf[12];//表明要循环6次来把数据写进去				
						for (int i = 0; i < cishu; i++)
						{
							int wei = recv_buf[recv_len - 1 - i];//先放低位再放高位
							bitset<159984> m(wei);
							int s = recv_buf[9] * 16 + 8 * i;
							m = m <<= s;
							bitset2 = bitset2 ^= m;
						}
				/*		cout << "bitset332=" << bitset2 << endl;*/
						int buf9 = recv_buf[9] * 16;
						int buf12 = recv_buf[12];
						int tou = 1000 - buf12 * 8 - buf9;
						int zuoyi = tou - buf9;//向左移为了好看
						bitset2 = bitset2 <<= zuoyi;
					/*	cout << "zuoyi=" << zuoyi << endl;
						cout << "bitset333=" << bitset2 << endl;*/




#pragma endregion
					}
#pragma region		指定的数据地址不在从设备中	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}
#pragma region		指定的数据超过范围	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
				}
#pragma endregion
			}
#pragma endregion  

		}
#pragma region		从站设备不支持此功能码	
			else
			{
			send_buf[0] = recv_buf[0];
			send_buf[1] = recv_buf[1];
			send_buf[2] = recv_buf[2];
			send_buf[3] = recv_buf[3];
			send_buf[4] = recv_buf[4];
			send_buf[5] = uint8_t(0x03);
			send_buf[6] = recv_buf[6];
			send_buf[7] = uint8_t(0x80 + recv_buf[7]);//异常功能码=正常功能码+0x80
			send_buf[8] = uint8_t(0x01);
			return send_length = 9;
			}
#pragma endregion
	}
#pragma region	从设备故障	       
		else
		{
		send_buf[0] = recv_buf[0];
		send_buf[1] = recv_buf[1];
		send_buf[2] = recv_buf[2];
		send_buf[3] = recv_buf[3];
		send_buf[4] = recv_buf[4];
		send_buf[5] = uint8_t(0x03);
		send_buf[6] = recv_buf[6];
		send_buf[7] = uint8_t(0x83);
		send_buf[8] = uint8_t(0x04);
		return send_length = 9;
		}
#pragma endregion
}