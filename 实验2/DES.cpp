#include <iostream>
#include <bitset>
#include <fstream>
#include <stdlib.h>
#include <ctype.h>
#include <string>

using namespace std;

#define bit_8  std::bitset<8>
#define bit_28 std::bitset<28>
#define bit_32 std::bitset<32>
#define bit_48 std::bitset<48>
#define bit_56 std::bitset<56>
#define bit_64 std::bitset<64>

//初始置换IP
const static int IP[64] = 
{
	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 ,
	62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8 ,
	57 , 49 , 41 , 33 , 25 , 17 ,  9 , 1 , 59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
	61 , 53 , 45 , 37 , 29 , 21 , 13 , 5 , 63 , 55 , 47 , 39 , 31 , 23 , 15 , 7
} ;

//逆初始置换IP
const static int InvIP[64] =
{
	40 , 8 , 48 , 16 , 56 , 24 , 64 , 32 , 39 , 7 , 47 , 15 , 55 , 23 , 63 , 31 ,
	38 , 6 , 46 , 14 , 54 , 22 , 62 , 30 , 37 , 5 , 45 , 13 , 53 , 21 , 61 , 29 ,
	36 , 4 , 44 , 12 , 52 , 20 , 60 , 28 , 35 , 3 , 43 , 11 , 51 , 19 , 59 , 27 ,
	34 , 2 , 42 , 10 , 50 , 18 , 58 , 26 , 33 , 1 , 41 ,  9 , 49 , 17 , 57 , 25
} ;

//选择置换PC-1
const static int PC_1[56] =
{
	57 , 49 , 41 , 33 , 25 , 17 ,  9 ,  
	 1 , 58 , 50 , 42 , 34 , 26 , 18 , 
	10 ,  2 , 59 , 51 , 43 , 35 , 27 , 
	19 , 11 ,  3 , 60 , 52 , 44 , 36 , 
	63 , 55 , 47 , 39 , 31 , 23 , 15 ,  
	 7 , 62 , 54 , 46 , 38 , 30 , 22 , 
	14 ,  6 , 61 , 53 , 45 , 37 , 29 , 
	21 , 13 ,  5 , 28 , 20 , 12 ,  4
} ;

//选择置换PC-2
const static int PC_2[48] = 
{
	14 , 17 , 11 , 24 ,  1 ,  5 ,  
	 3 , 28 , 15 ,  6 , 21 , 10 , 
	23 , 19 , 12 ,  4 , 26 ,  8 , 
	16 ,  7 , 27 , 20 , 13 ,  2 ,
	41 , 52 , 31 , 37 , 47 , 55 , 
	30 , 40 , 51 , 45 , 33 , 48 , 
	44 , 49 , 39 , 56 , 34 , 53 , 
	46 , 42 , 50 , 36 , 29 , 32
} ;

//位选择函数E
const static int E[48] =
{
	32 ,  1 ,  2 ,  3 ,  4 ,  5 , 
	 4 ,  5 ,  6 ,  7 ,  8 ,  9 , 
	 8 ,  9 , 10 , 11 , 12 , 13 , 
	12 , 13 , 14 , 15 , 16 , 17 , 
	16 , 17 , 18 , 19 , 20 , 21 , 
	20 , 21 , 22 , 23 , 24 , 25 , 
	24 , 25 , 26 , 27 , 28 , 29 , 
	28 , 29 , 30 , 31 , 32 ,  1
} ;

//置换函数P
const static int P[32] =
{
	16 ,  7 , 20 , 21 , 
	29 , 12 , 28 , 17 , 
	 1 , 15 , 23 , 26 , 
	 5 , 18 , 31 , 10 ,
	 2 ,  8 , 24 , 14 , 
	32 , 27 ,  3 ,  9 , 
	19 , 13 , 30 ,  6 , 
	22 , 11 ,  4 , 25
} ;

//S盒
const static int S[8][64] =
{
	//S1
	{
		14 ,  4 , 13 , 1 ,  2 , 15 , 11 ,  8 ,  3 , 10 ,  6 , 12 ,  5 ,  9 , 0 ,  7 ,
	 	 0 , 15 ,  7 , 4 , 14 ,  2 , 13 ,  1 , 10 ,  6 , 12 , 11 ,  9 ,  5 , 3 ,  8 ,
		 4 ,  1 , 14 , 8 , 13 ,  6 ,  2 , 11 , 15 , 12 ,  9 ,  7 ,  3 , 10 , 5 ,  0 ,
     		15 , 12 ,  8 , 2 ,  4 ,  9 ,  1 ,  7 ,  5 , 11 ,  3 , 14 , 10 ,  0 , 6 , 13
	} ,

	//S2
	{
    		15 ,  1 ,  8 , 14 ,  6 , 11 ,  3 ,  4 ,  9 , 7 ,  2 , 13 , 12 , 0 ,  5 , 10 ,
    		 3 , 13 ,  4 ,  7 , 15 ,  2 ,  8 , 14 , 12 , 0 ,  1 , 10 ,  6 , 9 , 11 ,  5 ,
		 0 , 14 ,  7 , 11 , 10 ,  4 , 13 ,  1 ,  5 , 8 , 12 ,  6 ,  9 , 3 ,  2 , 15 ,
		13 ,  8 , 10 ,  1 ,  3 , 15 ,  4 ,  2 , 11 , 6 ,  7 , 12 ,  0 , 5 , 14 ,  9
	} ,

	//S3
	{
		10 ,  0 ,  9 , 14 , 6 ,  3 , 15 ,  5 ,  1 , 13 , 12 ,  7 , 11 ,  4 ,  2 ,  8 ,
		13 ,  7 ,  0 ,  9 , 3 ,  4 ,  6 , 10 ,  2 ,  8 ,  5 , 14 , 12 , 11 , 15 ,  1 ,
	   	13 ,  6 ,  4 ,  9 , 8 , 15 ,  3 ,  0 , 11 ,  1 ,  2 , 12 ,  5 , 10 , 14 ,  7 ,
	   	 1 , 10 , 13 ,  0 , 6 ,  9 ,  8 ,  7 ,  4 , 15 , 14 ,  3 , 11 ,  5 ,  2 , 12
	} ,

	//S4
	{
		 7 , 13 , 14 , 3 ,  0 ,  6 ,  9 , 10 ,  1 , 2 , 8 ,  5 , 11 , 12 ,  4 , 15 ,
		13 ,  8 , 11 , 5 ,  6 , 15 ,  0 ,  3 ,  4 , 7 , 2 , 12 ,  1 , 10 , 14 ,  9 ,
	   	10 ,  6 ,  9 , 0 , 12 , 11 ,  7 , 13 , 15 , 1 , 3 , 14 ,  5 ,  2 ,  8 ,  4 ,
	   	 3 , 15 ,  0 , 6 , 10 ,  1 , 13 ,  8 ,  9 , 4 , 5 , 11 , 12 ,  7 ,  2 , 14
	} ,

	//S5
	{
		 2 , 12 ,  4 ,  1 ,  7 , 10 , 11 ,  6 ,  8 ,  5 ,  3 , 15 , 13 , 0 , 14 ,  9 ,
	   	14 , 11 ,  2 , 12 ,  4 ,  7 , 13 ,  1 ,  5 ,  0 , 15 , 10 ,  3 , 9 ,  8 ,  6 ,
	   	 4 ,  2 ,  1 , 11 , 10 , 13 ,  7 ,  8 , 15 ,  9 , 12 ,  5 ,  6 , 3 ,  0 , 14 ,
	   	11 ,  8 , 12 ,  7 ,  1 , 14 ,  2 , 13 ,  6 , 15 ,  0 ,  9 , 10 , 4 ,  5 ,  3
	} ,

	//S6
	{
		12 ,  1 , 10 , 15 , 9 ,  2 ,  6 ,  8 ,  0 , 13 ,  3 ,  4 , 14 ,  7 ,  5 , 11 ,
 		10 , 15 ,  4 ,  2 , 7 , 12 ,  9 ,  5 ,  6 ,  1 , 13 , 14 ,  0 , 11 ,  3 ,  8 ,
	   	 9 , 14 , 15 ,  5 , 2 ,  8 , 12 ,  3 ,  7 ,  0 ,  4 , 10 ,  1 , 13 , 11 ,  6 ,
   		 4 ,  3 ,  2 , 12 , 9 ,  5 , 15 , 10 , 11 , 14 ,  1 ,  7 ,  6 ,  0 ,  8 , 13
	} ,

	//S7
	{
	   	 4 , 11 ,  2 , 14 , 15 , 0 ,  8 , 13 ,  3 , 12 , 9 ,  7 ,  5 , 10 , 6 ,  1 ,
  		13 ,  0 , 11 ,  7 ,  4 , 9 ,  1 , 10 , 14 ,  3 , 5 , 12 ,  2 , 15 , 8 ,  6 ,
   		 1 ,  4 , 11 , 13 , 12 , 3 ,  7 , 14 , 10 , 15 , 6 ,  8 ,  0 ,  5 , 9 ,  2 ,
   		 6 , 11 , 13 ,  8 ,  1 , 4 , 10 ,  7 ,  9 ,  5 , 0 , 15 , 14 ,  2 , 3 , 12
	} ,

	//S8
	{
	   	13 ,  2 ,  8 , 4 ,  6 , 15 , 11 ,  1 , 10 ,  9 ,  3 , 14 ,  5 ,  0 , 12 ,  7 ,
	   	 1 , 15 , 13 , 8 , 10 ,  3 ,  7 ,  4 , 12 ,  5 ,  6 , 11 ,  0 , 14 ,  9 ,  2 ,
	   	 7 , 11 ,  4 , 1 ,  9 , 12 , 14 ,  2 ,  0 ,  6 , 10 , 13 , 15 ,  3 ,  5 ,  8 ,
	   	 2 ,  1 , 14 , 7 ,  4 , 10 ,  8 , 13 , 15 , 12 ,  9 ,  0 ,  3 ,  5 ,  6 , 11
	}
} ;

bit_48 SubKey[16];
bit_32 L[17], R[17];

void StrToBit(char str[8], bit_64 &bit) {
	int i, j;
	for(i = 0 ; i < 8 ; ++i) {
		bit_8 byte(str[i]);
		for(j=i*8; j<i*8+8; ++j) {
			bit[j] = byte[j-i*8];
		}
	}
}

void BitToStr(bit_64 bit, char str[8]) {
	int i, j;
	bit_8 byte;
	for(i = 0; i < 8; ++i) {
		for(j=i*8; j<i*8+8; ++j) {
			byte[j-i*8] = bit[j];
		}
		str[i] = byte.to_ulong();
	}
}

void GetSubKey(bit_64 key, bit_48 subkey[16]) {
	bit_28 C[17], D[17];
	bit_56 CD[16];
	
	//选择置换PC-1
	for(int i=0; i<28; ++i)
		C[0][i] = key[ PC_1[i-1] ];
	for(int i=0; i<28; ++i)
		D[0][i] = key[ PC_1[i+28-1] ];

	//循环移位
	for(int i=1; i<=16; ++i) {
		if(i==1 || i==2 || i==9 || i==16) {
			C[i] = (C[i-1] << 1) | (C[i-1] >> 27);
			D[i] = (D[i-1] << 1) | (D[i-1] >> 27);
		}
		else {
			C[i] = (C[i-1] << 2) | (C[i-1] >> 26);
			D[i] = (D[i-1] << 2) | (D[i-1] >> 26);
		}

		//组合C[i]D[i]为CD[i-1]
		for(int j=0; j<28; ++j) {
			CD[i-1][j] = C[i][j];
		}
		for(int j=0; j<28; ++j) {
			CD[i-1][j+28] = D[i][j];
		}

		//选择置换PC-2
		for(int j=0; j<56; ++j) {
			subkey[i-1][j] = CD[i-1][ PC_2[j]-1 ] ;
		}
	}
}

void InPermute(bit_64 src) {
	bit_64 result;
	for(int i=0; i<64; ++i) {
		result[i] = src[ IP[i] - 1 ] ;
	}
	for(int i=0; i<32; ++i) {
		L[0][i] = result[i];
	}
	for(int i=32; i<64; ++i) {
		R[0][i-32] = result[i];
	}
}

void InvInPermute(bit_64 &result) {
	bit_64 src;

	for(int i=0; i<32; ++i) {
		src[i] = R[16][i] ;
	}
	for(int i=32; i<64; ++i) {
		src[i] = L[16][i-32] ;
	}
	for(int i=0; i<64; ++i) {
		result[i] =src[ InvIP[i] - 1 ];
	}
}

//迭代
void Iterate(bit_32 left1, bit_32 right1, bit_32 &left2, bit_32 &right2, bit_48 subkey) {
	bit_48 before_S ;
	bit_32 after_S, after_P ;

	//经位选择函数E将右半部分扩展为48bit
	for(int i = 0 ; i < 48 ; ++i)
		before_S[i] = right1[ E[i]-1 ] ;

	before_S = before_S ^ subkey ;	//与子密钥异或

	//S盒
	std::bitset<4> col ;	//S盒的列
	std::bitset<2> row ;	//S盒的行

	for(int i = 0 ; i < 8 ; ++ i)
	{
		row[0] = before_S[6 * i] ;	//获取行标
		row[1] = before_S[6 * i + 5] ;

		col[0] = before_S[6 * i + 1] ;	//获取列标
		col[1] = before_S[6 * i + 2] ;
		col[2] = before_S[6 * i + 3] ;
		col[3] = before_S[6 * i + 4] ;

		//组合8个S盒
		std::bitset<4> temp( S[i][row.to_ulong() * 16 + col.to_ulong()] ) ;
		for(int j = 0 ; j < 4 ; ++j)
			after_S[4 * i + j] = temp[j] ;
	}

	for(int i = 0 ; i < 32 ; ++i)
		after_P[i] = after_S[ P[i]-1 ] ;	//经置换函数P后得到32bit

	left2 = right1 ;		//L[i] = R[i-1]
	right2 = left1 ^ after_P  ;	//异或运算
}

void Encrypt(bit_64 message, bit_64 &cipher, bit_64 key) {
	GetSubKey(key, SubKey);	//计算16个子密钥
	InPermute(message);	//得到L[0],R[0]

	//16轮迭代
	for(int i=1; i<=16; ++i) {
		Iterate(L[i-1], R[i-1], L[i], R[i], SubKey[i-1]);
	}

	InvInPermute(cipher);	//得到密文
}

void Decrypt(bit_64 cipher, bit_64 &message, bit_64 key) {
	GetSubKey(key, SubKey);	//计算16个子密钥
	InPermute(cipher);	//得到L[0],R[0]

	//16轮迭代
	for(int i=1; i<=16; ++i) {
		Iterate(L[i-1], R[i-1], L[i], R[i], SubKey[16-i]);
	}

	InvInPermute(message);	//得到明文	
}

int main() {
//	char c1[8] = {'1','2','3','4','5','6','7','8'};	//明文字符
	char c1[8] = {'8','7','6','5','4','3','2','1'};	
	char c2[8];
	bit_64 message, cipher;	//明文，密文
	bit_64 key;		//密钥
	bit_64 plain;		//解密后明文

	cout << "Before Encrypt: " << c1 << endl;	//打印加密前明文

	StrToBit(c1, message);				//明文二进制串

	StrToBit(c1, key);				//密钥

	Encrypt(message, cipher, key);			//加密
	Decrypt(cipher, plain, key);			//解密

	BitToStr(plain, c2);				//解密后将二进制串转为字符

	cout << "After  Decrypt: " << c2 << endl;	//打印解密后明文

	return 0;
}
