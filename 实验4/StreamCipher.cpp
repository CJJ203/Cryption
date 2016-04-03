#include <iostream>
#include <fstream>
#include <cstdlib>
#include <bitset>
using namespace std;

#define bit_5 std::bitset<5>

void GenKey(bit_5 initial, int n) {
	bitset<1> newbit;
	for(int i=0; i<n; ++i) {
		cout << initial[0];
		newbit = initial[0] ^ initial[3];
		initial = initial >> 1;
		initial[4] = newbit[0];
	}
}

void EnCrypt(char *file1, char *file2, bitset<5> initial) {
	fstream infile;
	fstream outfile;
	char ch1, ch2;

	infile.open(file1, ios::in | ios::binary);
	if(infile.fail()) {
		cout << "打开文件失败！" << endl;
		exit(0);
	}

	outfile.open(file2, ios::out | ios::binary);

	while(!infile.eof()) {
		infile.read((char*)&ch1, sizeof(ch1));
		if(infile.fail())
			break;
		bitset<8> message(ch1);
		bitset<8> cipher;
		bitset<1> newbit;
		for(int i=0; i<8; ++i) {
			cipher[i] = message[i] ^ initial[0];
			newbit = initial[0] + initial[3];
			initial = initial >> 1;
			initial[4] = newbit[0];
		}
		ch2 = cipher.to_ulong();
		outfile.write((char*)&ch2, sizeof(ch2));
	}

	infile.close();
	outfile.close();
}

int main() {
	char *infile = "message.txt";
	char *outfile = "cipher.txt";
	char *result = "decrypt.txt";
	char str[5];

	cout << "输入5位初始二进制序列：" ;
	cin >> str;
	bitset<5> initial(str);

	cout << "\n密钥序列(部分):" << endl;
	GenKey(initial, 100);

	EnCrypt(infile, outfile, initial);
	cout << "\n\n对文件message.txt加密--结果保存在cipher.txt中" << endl;

	EnCrypt(outfile, result, initial);
	cout << "\n对文件cipher.txt解密--结果保存在decrypt.txt中" << endl;

	return 0;
}