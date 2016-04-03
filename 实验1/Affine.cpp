#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

//加密字符
void Encrypt(char m, char &c, char k1, char k2) {
	//不是英文字母
	if( ( m <'a' || m > 'z' ) && 
		( m <'A' || m > 'Z' ) ) 
	{
		c = m ;
		return ;
	}
	
	if(m<97)	m = m + 32;		//大写变小写
	int offset = 97 ;

	m = m - offset ;
	k1 = k1 - offset ;
	k2 = k2 - offset ;

	c = ( ( (k1+k2*m) % 26) + offset );
	return ;
}

//解密字符
void Decrypt(char c, char &m, char k1, char k2) {
	if( ( c <'a' || c > 'z' ) && 
		( c <'A' || c > 'Z' ) ) 
	{
		m = c ;
		return ;
	}

	if(c<97)	c = c + 32;		//大写变小写
	int offset = 97 ;

	c = c - offset ;
	k1 = k1 - offset ;
	k2 = k2 - offset ;

	for(int i=0; i<26; ++i) {
		if( (k1+k2*i) % 26 == c )
			m = i + offset ;
	}

	return ;
}

//加密文件
void EncryptFile(char * plain, char * cipher, char k1, char k2) {
	fstream infile;
	fstream outfile;
	char ch1, ch2;

	infile.open(plain,ios::in);
	if( infile.fail() ) {
		cout << "Can't open " << plain << endl;
		exit(0);
	}

	outfile.open(cipher, ios::out);
	if( outfile.fail() ) {
		cout << "Can't open " << cipher << endl;
		exit(0);
	}

	while( !infile.eof() ) {
		infile.get(ch1) ;
		if(infile.fail())
			break;
		Encrypt(ch1, ch2, k1, k2);
		outfile.put(ch2);
	}

	infile.close();
	outfile.close();
}

//解密文件
void DecryptFile(char * cipher, char * plain, char k1, char k2) {
	fstream infile;
	fstream outfile;
	char ch1, ch2;

	infile.open(cipher,ios::in);
	if( infile.fail() ) {
		cout << "Can't open " << cipher << endl;
		exit(0);
	}

	outfile.open(plain, ios::out);
	if( outfile.fail() ) {
		cout << "Can't open " << plain << endl;
		exit(0);
	}

	while( !infile.eof() ) {
		infile.get(ch1) ;
		if(infile.fail())
			break;
		Decrypt(ch1, ch2, k1, k2);
		outfile.put(ch2);
	}

	infile.close();
	outfile.close();
}

void statistic(char * file) {
	fstream infile ;
	char charac;
	int ch[26] = { 0 } ;
	float count = 0.0;
	int offset = 97;
	float percent = 0.0;

	infile.open(file, ios::in);
	if( infile.fail() ) {
		cout << "Can't open " << file << endl;
		exit(0);
	}

	while( !infile.eof() ) {
		infile.get(charac) ;
		if(infile.fail())
			break;

		if( ( charac <'a' || charac > 'z' ) && 
			( charac <'A' || charac > 'Z' ) ) 
			continue;

		if(charac < 97)	
			charac = charac + 32;		//大写变小写
		
		ch[charac - offset] ++ ;
		count ++ ;
	}

	for(int i=0; i<26; ++i) {
		charac = i + 97 ;
		percent = ch[i] / count * 100;
		cout << charac << " : " ;
		cout << setprecision(6) << setiosflags( ios::fixed ) << percent;
		cout << "%" << "\t";
	}

	cout << endl;

	infile.close();
}

int main() {
	char * plain = "src.txt" ;
	char * cipher = "tar.txt" ;
	char * decrypt = "decry.txt" ;
	char k1 = 'c';
	char k2 = 'f';
	char m, c;

	//打印密钥对
	cout << "KEY : ( " << k1 << ", " << k2 << " )" << endl << endl ;

	//打印加解密映射关系
	cout << "Mapping :" << endl;

	for(m='a'; m<='z'; m++) {
		Encrypt(m, c, k1, k2) ;
		cout << m << " <--> " << c << "\t" ;
	}

	cout << endl << endl ;

	EncryptFile(plain, cipher, k1, k2);		//加密文件

	DecryptFile(cipher, decrypt, k1, k2);	//解密文件

	//打印字符出现频率
	cout << "Characters' frequencies in Plaintext: " << endl ;
	statistic(plain);

	cout<< endl;

	cout << "Characters' frequencies in Ciphertext: " << endl ;
	statistic(cipher);

	return 0;
}
