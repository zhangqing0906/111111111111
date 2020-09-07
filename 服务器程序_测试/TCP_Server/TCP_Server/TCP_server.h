#pragma once
#include<iostream>
#include<string>
#include<winsock.h>
#include <bitset>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

void initialization();



bool receve_shuju(uint8_t*recv_buf,int recv_len);

int send_length(uint8_t* recv_buf, uint8_t* send_buf,int recv_len);

