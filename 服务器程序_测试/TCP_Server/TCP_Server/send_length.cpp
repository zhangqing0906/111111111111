#include"TCP_server.h"

int send_length(uint8_t* recv_buf, uint8_t* send_buf, int recv_len)
{
	bitset<9999> bitset1;//����Ȧ�ռ�
	for (int i = 0; i < bitset1.size(); i = i + 2)//0x01��0x10����
	{
		bitset1.set(i);//����i + 1λ���1
	}
	bitset<159984> bitset2;//���Ĵ����ռ䣬һ���Ĵ���=16λ  159984  //0x03��0x0F����
	for (int i = 0; i < bitset2.size(); i = i + 2)//0x01��0x10����
	{
		bitset2.set(i);//����i + 1λ���1
	}
	
	int send_length;

	uint8_t k = 0x09; //�����豸�ţ������ˣ�������1-247��

	uint8_t L = uint8_t(recv_len) - 0x06;//LΪMBAP���ֽڳ��ȣ������ж϶������������Ƿ���ȷ

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
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						int a1 = recv_buf[8]; //uint8_tתint
						int a2 = recv_buf[9];
						int start_bits = 0;
						start_bits = start_bits + a1 * 16 * 16 + a2;//������ȡ��Ȧ��ʼ��ַ
						int start_bits1 = 0;//��Ȧ��
						int b1 = recv_buf[10];//uitn8_tתint
						int b2 = recv_buf[11];
						start_bits1 = start_bits1 + b1 * 16 * 16 + b2;//������Ҫ�����ٸ���Ȧ
#pragma region ����������
						int xunhauanshu = 0;
						if (start_bits1 % 8 == 0)
						{
							xunhauanshu = start_bits1 / 8;//����������Ϊ��Ȧ�������µ�Ҫѭ�����ٴ�
							int diwuwei = xunhauanshu + 3;
							send_buf[5] = (uint8_t)diwuwei;//intǿ��תΪuint8_t
							send_buf[6] = recv_buf[6];
							send_buf[7] = recv_buf[7];
							send_buf[8] = (uint8_t)xunhauanshu;//intǿ��תΪuint8_t
							for (int t = 0; t < xunhauanshu; t++)
							{
								bitset<8>a;
								unsigned int b;
								a = bitset<8>(bitset1.to_string().substr(start_bits, 8)).to_ulong();//��ȡbitset����ӵ�ַ19��ʼ��8λ
								b = a.to_ulong();   
								start_bits = start_bits + 8;
								send_buf[9 + t] = (uint8_t)b;

							}
						}
#pragma endregion

#pragma region ���㲻�ܱ�������
						else
						{
							xunhauanshu = start_bits1 / 8;//����������Ϊ��Ȧ�������µ�Ҫѭ�����ٴ�
							int diwuwei = xunhauanshu + 3 + 1;
							send_buf[5] = (uint8_t)diwuwei;//intǿ��תΪuint8_t
							send_buf[6] = recv_buf[6];
							send_buf[7] = recv_buf[7];
							send_buf[8] = (uint8_t)(xunhauanshu + 1);//intǿ��תΪuint8_t
							for (int t = 0; t < xunhauanshu; t++)
							{
								bitset<8>a;
								unsigned int b;
								a = bitset<8>(bitset1.to_string().substr(start_bits, 8)).to_ulong();//��ȡbitset����ӵ�ַ19��ʼ��8λ
								//b = a.flip().to_ulong(); //��ת,�ٱ��int��     .to_ulong()�����⻹�����
								b = a.to_ulong();
								start_bits = start_bits + 8;
								send_buf[9 + t] = (uint8_t)b;
							}
							int start_start_bits = start_bits + xunhauanshu * start_bits1;//ͳ�����һ��Ҫ��������ݵ���ʼ��ַ
							int bulingshu = start_bits1 % 8;//����Ҫ��ȡ��λ
							bitset<8>bulinggeshu;
							bulinggeshu = bitset<8>(bitset1.to_string().substr(start_start_bits, bulingshu)).to_ulong();
							int bulingint = bulinggeshu.to_ulong();
							send_buf[9 + xunhauanshu] = (uint8_t)bulingint; //��ȡ��ֵǰ��8-bulingshuλ����
						}
#pragma endregion   

#pragma region	��������sendҪ���Ͷ��ٸ��ֽ�	

						int b5 = send_buf[8];//uitn8_tתint

#pragma endregion			

						cout << "������ظ���Ϣ:";
						return send_length = 9 + b5;

					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}

#pragma region		ָ�������ݳ�����Χ	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];

						int start_bits = 0;
						int a1 = recv_buf[8]; //uint8_tתint
						int a2 = recv_buf[9];
						start_bits = (start_bits + a1 * 16 * 16 + a2) * 8;//������ȡ�Ĵ�����ʼ��ַ

						int start_bits1 = 0;//�Ĵ�����
						int b1 = recv_buf[10] * 16 * 16;//uitn8_tתint
						int b2 = recv_buf[11];
						start_bits1 = (start_bits1 + b1 * 16 * 16 + b2) * 2;//������Ҫ�����ٸ��Ĵ�������bit������Գ�2��

						int diwuwei = 2 * (b1 + b2) + 3;
						send_buf[5] = (uint8_t)diwuwei;
						send_buf[6] = recv_buf[6];
						send_buf[7] = recv_buf[7];
						send_buf[8] = (uint8_t)(2 * (b1 + b2));

						for (int t = 0; t < (b1 + b2) * 2; t++)
						{
							bitset<8>a;
							unsigned int b;
							a = bitset<8>(bitset2.to_string().substr(start_bits, 8)).to_ulong();//��ȡbitset����ӵ�ַ19��ʼ��8λ
							b = a.to_ulong(); 
							start_bits = start_bits + 8;
							send_buf[9 + t] = (uint8_t)b;
							/*cout << "���send_buf[9+t]=" << send_buf[9 + t] << endl;
							cout << "���a=" << a << endl;
							cout << "���b=" << b << endl;
							cout << "���t=" << t << endl;*/
						}

						cout << "������ظ���Ϣ:";
						int start_bits2 = 0;
						int b3 = recv_buf[10];//uitn8_tתint
						int b4 = recv_buf[11];
						start_bits2 = (b3 + b4) * 2;//������Ҫ�����ٸ��Ĵ���
						return send_length = start_bits2 + 9;
						
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}

#pragma region		ָ�������ݳ�����Χ	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
#pragma region	������Ϣ֡			
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

#pragma region  ��дд��Ȧ
						int cishu = recv_buf[12];//����Ҫѭ��2����������д��ȥ				
						for (int i = 0; i < cishu; i++)
						{
							int wei = recv_buf[recv_len - 1 - i];//�ȷŸ�λ�ٷŵ�λ
							bitset<9999> m(wei);
							int s = recv_buf[9] + 8 * i;
							m = m <<= s;
							bitset1 = bitset1 ^= m;
						}
					/*	cout << "bitset332=" << bitset1 << endl;*/
						int buf9 = recv_buf[9];
						int buf12 = recv_buf[12];
						int tou = 9999 - buf12 * 8 - buf9;
						int zuoyi = tou - buf9;//������Ϊ�˺ÿ�
						bitset1 = bitset1 <<= zuoyi;
						/*cout << "zuoyi=" << zuoyi << endl;
						cout << "bitset333=" << bitset1 << endl;*/
#pragma endregion
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}
#pragma region		ָ�������ݳ�����Χ	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
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
#pragma region	������Ϣ֡		
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

#pragma region	��дд�Ĵ���
						int cishu = recv_buf[12];//����Ҫѭ��6����������д��ȥ				
						for (int i = 0; i < cishu; i++)
						{
							int wei = recv_buf[recv_len - 1 - i];//�ȷŵ�λ�ٷŸ�λ
							bitset<159984> m(wei);
							int s = recv_buf[9] * 16 + 8 * i;
							m = m <<= s;
							bitset2 = bitset2 ^= m;
						}
				/*		cout << "bitset332=" << bitset2 << endl;*/
						int buf9 = recv_buf[9] * 16;
						int buf12 = recv_buf[12];
						int tou = 1000 - buf12 * 8 - buf9;
						int zuoyi = tou - buf9;//������Ϊ�˺ÿ�
						bitset2 = bitset2 <<= zuoyi;
					/*	cout << "zuoyi=" << zuoyi << endl;
						cout << "bitset333=" << bitset2 << endl;*/




#pragma endregion
					}
#pragma region		ָ�������ݵ�ַ���ڴ��豸��	
					else
					{
						send_buf[0] = recv_buf[0];
						send_buf[1] = recv_buf[1];
						send_buf[2] = recv_buf[2];
						send_buf[3] = recv_buf[3];
						send_buf[4] = recv_buf[4];
						send_buf[5] = uint8_t(0x03);
						send_buf[6] = recv_buf[6];
						send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
						send_buf[8] = uint8_t(0x02);
						return send_length = 9;
					}
#pragma endregion
				}
#pragma region		ָ�������ݳ�����Χ	
				else
				{
					send_buf[0] = recv_buf[0];
					send_buf[1] = recv_buf[1];
					send_buf[2] = recv_buf[2];
					send_buf[3] = recv_buf[3];
					send_buf[4] = recv_buf[4];
					send_buf[5] = uint8_t(0x03);
					send_buf[6] = recv_buf[6];
					send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
					send_buf[8] = uint8_t(0x03);
					return send_length = 9;
				}
#pragma endregion
			}
#pragma endregion  

		}
#pragma region		��վ�豸��֧�ִ˹�����	
			else
			{
			send_buf[0] = recv_buf[0];
			send_buf[1] = recv_buf[1];
			send_buf[2] = recv_buf[2];
			send_buf[3] = recv_buf[3];
			send_buf[4] = recv_buf[4];
			send_buf[5] = uint8_t(0x03);
			send_buf[6] = recv_buf[6];
			send_buf[7] = uint8_t(0x80 + recv_buf[7]);//�쳣������=����������+0x80
			send_buf[8] = uint8_t(0x01);
			return send_length = 9;
			}
#pragma endregion
	}
#pragma region	���豸����	       
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