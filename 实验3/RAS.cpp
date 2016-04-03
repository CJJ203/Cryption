#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

using namespace std;

//求u模n的逆
int Euclid(int u, int n) {
	int n1, n2, b1, b2, q, r, t;

	n1 = n;
	n2 = u;
	b1 = 0;
	b2 = 1;

repeat:
	q = n1 / n2;
	r = n1 - q * n2;
	
	if(r != 0) {
		n1 = n2;
		n2 = r;
		t = b2;
		b2 = b1 - q * b2;
		b1 = t;
		goto repeat;
	}

	if(n2 != 1) 
		return -1;
	else {
		if(b2 % n < 0)
			return n + b2 % n;
		else
			return b2 % n;
	}
}

//x^r mod n
int power_mod(int x, int r, int n) {
	int a, b, c;

	a = x;
	b = r;
	c = 1;

	while(b != 0) {
		while(b % 2 == 0) {
			b = b / 2;
			a = (a * a) % n;
		}
		b = b - 1;
		c = (c * a) % n;
	}

	return c;
}

//Miller-Rabin素性测试法
bool Miller_Rabin(int n, int k) {
	if(n < 2)
		return false;
	if(n == 2) 
		return false;
	if((n & 0x1) != 1)
		return false;

	int r, b, m, s = 0, z;
	int range = n / k;
	for(m = n - 1; (m & 0x1) != 1;  m = m >> 1, ++s);

	for(b = rand() % (n-1) + 1; n - b < range; b = b + range) {
		r = 0;
		z = power_mod(b, m, n);

		if(z == 1 || z == n - 1) {
			return true;
		}  
loop:		
		if (r == s - 1) {
			return false;
		}

		r = r + 1;
		z = z * z % n;

		if(z == n - 1) {
			return true;
		}
		else
			goto loop;
	}
}

int CharToNum(char ch1, char ch2) {
	int n1, n2;
	
	if(ch1 == ' ') {
		n1 = 0;
	} else if(ch1 >='A' && ch1 <= 'Z') {
		n1 = ch1 - 'A' + 1;
	}

	if(ch2 == ' ') {
		n2 = 0;
	} else if(ch2 >='A' && ch2 <= 'Z') {
		n2 = ch2 - 'A' + 1;
	}

	return n1 * 27 + n2;
}

void NumToChar(int n, char &ch1, char &ch2) {
	int n1, n2;

	n1 = n / 27;
	n2 = n % 27;

	if(n1 == 0) {
		ch1 = ' ';
	} else {
		ch1 = (char)(n1 + 64);
	}

	if(n2 == 0) {
		ch2 = ' ';
	} else {
		ch2 = (char)(n2 + 64);
	}
}

void EnCrypt(int m, int &c, int e, int n) {
	c = power_mod(m, e, n);
}

void DeCrypt(int c, int &m, int d, int n) {
	m = power_mod(c, d, n);
}

int main() {
//	cout << "7 ^ 563 % 561 = " << power_mod(7, 563, 561) << endl;

	cout << "素数不宜太大，否则可能溢出导致程序运行出错" << endl;
	int p = 107, q = 137;
	if(!(Miller_Rabin(p, 6) && Miller_Rabin(q, 6))) {
		cout << "p,q不全为素数！"<< endl;
		exit(0);
	}

	cout << "两个素数:" << p << "  " << q << endl;
	int n = p * q;
	int fn = (p - 1) * (q - 1);
	int e, d;

	e = 47;
	cout << "提前设定的加密密钥:" << e << endl;

	d = Euclid(e, fn);
	cout << "相应的解密密钥:" << d << endl;

	fstream infile;
	char ch1, ch2;
	int m, c;
	int Number[100], i=0;

	infile.open("message.txt", ios::in);
	if(infile.fail()) {
		cout << "文件打开失败！" << endl;
		exit(0);
	}

	cout << "\n字符\t对应的数字\t加密结果\n" ;
	while(!infile.eof()) {
		infile.get(ch1);
		if(infile.fail()) {
			break;
		}
		infile.get(ch2);
		if(infile.fail()) {
			break;
		}
		m = CharToNum(ch1, ch2);
		EnCrypt(m, c, e, n);
		Number[i] = c;
		cout << ch1 << ch2 << "\t" << m << "\t" << c << endl;
		i ++;
	}
	infile.close();
	cout << "加密结果保存在Number数组中" << endl;

	fstream outfile;
	outfile.open("decrypt.txt", ios::out);
	if(outfile.fail()) {
		cout << "文件打开失败！" << endl;
		exit(0);
	}

	cout << "\n密文\t明文\t对应的字符\n";
	for(int k=0; k<i; k++) {
		DeCrypt(Number[k], m, d, n);
		NumToChar(m, ch1, ch2);
		outfile.put(ch1);
		outfile.put(ch2);
		cout << Number[k] << "\t" << m << "\t" << ch1 << ch2 << endl;
	}
	outfile.close();
	cout << "解密结果保存在文件decrypt.txt中" << endl;

	return 0;
}
