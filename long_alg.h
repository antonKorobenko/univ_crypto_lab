#include <iostream>
#include <vector>

using namespace std;

class BigInt {
private:
	bool isNegative;
	vector<int> digits;
public:
	static const int BASE_LEN;
	static const int BASE;


	BigInt();
	BigInt(string s);
	BigInt(int x);
	BigInt(vector<int> digits_, bool isNegative_);
	BigInt(const BigInt& bigInt);

	void removeLeadingZeros();
	void clearNumber();
	BigInt abs();
	static string formatOutput(int x);
	static bool isDigit(char ch);
	bool isZero();
	bool isPositiveOne();
	bool isNegativeOne();
	BigInt sqrt();
	BigInt pow(BigInt n);
	BigInt pow(BigInt n, BigInt mod);
	BigInt reversedBySimpleMod(BigInt mod);
	BigInt mathMod(BigInt mod);
	int getLength();
	vector<int> getDigits();

	friend ostream& operator << (ostream& os, BigInt bigInt);
	friend istream& operator >> (istream& is, BigInt& bigInt);

	friend bool operator == (BigInt bigInt1, BigInt bigInt2);
	friend bool operator == (BigInt bigInt1, int int2);
	friend bool operator == (int int1, BigInt bigInt2);

	friend bool operator != (BigInt bigInt1, BigInt bigInt2);
	friend bool operator != (BigInt bigInt1, int int2);
	friend bool operator != (int int1, BigInt bigInt2);

	friend bool operator > (BigInt bigInt1, BigInt bigInt2);
	friend bool operator > (BigInt bigInt1, int int2);
	friend bool operator > (int int1, BigInt bigInt2);

	friend bool operator < (BigInt bigInt1, BigInt bigInt2);
	friend bool operator < (BigInt bigInt1, int int2);
	friend bool operator < (int int1, BigInt bigInt2);

	friend bool operator >= (BigInt bigInt1, BigInt bigInt2);
	friend bool operator >= (BigInt bigInt1, int int2);
	friend bool operator >= (int int1, BigInt bigInt2);

	friend bool operator <= (BigInt bigInt1, BigInt bigInt2);
	friend bool operator <= (BigInt bigInt1, int int2);
	friend bool operator <= (int int1, BigInt bigInt2);

	friend BigInt operator + (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator + (BigInt bigInt1, int int2);
	friend BigInt operator + (int int1, BigInt bigInt2);
	friend const BigInt operator ++ (BigInt& a, int);

	friend BigInt operator - (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator - (BigInt bigInt1, int int2);
	friend BigInt operator - (int int1, BigInt bigInt2);
	BigInt operator - ();

	friend BigInt operator * (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator * (BigInt bigInt1, int int2);
	friend BigInt operator * (int int1, BigInt bigInt2);

	BigInt div2();
	BigInt mod2();

	friend BigInt operator / (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator / (BigInt bigInt1, int int2);
	friend BigInt operator / (int int1, BigInt bigInt2);

	friend BigInt operator % (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator % (BigInt bigInt1, int int2);
	friend BigInt operator % (int int1, BigInt bigInt2);

	BigInt addBySimpleMod(BigInt other, BigInt mod);
	BigInt subBySimpleMod(BigInt other, BigInt mod);
	BigInt mulBySimpleMod(BigInt other, BigInt mod);
	BigInt divBySimpleMod(BigInt other, BigInt mod);
	BigInt powBySimpleMod(BigInt n, BigInt mod);

};

BigInt karatsuba(BigInt a, BigInt b);