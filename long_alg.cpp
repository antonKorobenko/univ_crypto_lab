#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <time.h>
#include <algorithm>
#include <string>
#include "long_alg.h"

using namespace std;

const int BigInt::BASE_LEN = 3;
const int BigInt::BASE = 1000;

// initialization

BigInt::BigInt() {
	isNegative = false;
	digits.clear();
	digits.resize(1, 0);
}

BigInt::BigInt(string str) {
	digits.clear();
	if (str[0] == '-') {
		isNegative = true;
		str.erase(str.begin());
	}
	else
		isNegative = false;

	for (int i = 0; i < (int)str.size(); i++) {
		if (!isDigit(str[i]))
			throw "Value Error";
	}

	for (int i = (int)str.length(); i > 0; i -= BASE_LEN) {
		if (i < BASE_LEN)
			digits.push_back(atoi(str.substr(0, i).c_str()));
		else
			digits.push_back(atoi(str.substr(i - BASE_LEN, BASE_LEN).c_str()));
	}

	removeLeadingZeros();
}

BigInt::BigInt(int x) {
	digits.clear();
	if (x < 0) {
		isNegative = true;
		x = -x;
	}
	else {
		isNegative = false;
	}

	if (x == 0) {
		digits.push_back(0);
	}
	else {
		while (x != 0) {
			digits.push_back(x % BASE);
			x /= BASE;
		}
	}
}

BigInt::BigInt(vector<int>digits_, bool isNegative_) {
	digits = digits_;
	isNegative = isNegative_;
	removeLeadingZeros();
}

BigInt::BigInt(const BigInt& bigInt) {
	digits = bigInt.digits;
	isNegative = bigInt.isNegative;
}

// functions - helpers

void BigInt::removeLeadingZeros() {
	while (digits.size() > 1 && digits.back() == 0)
		digits.pop_back();
}

void BigInt::clearNumber() {
	removeLeadingZeros();
	if (isZero())
		isNegative = false;
}

BigInt BigInt::abs(){
	BigInt res = BigInt(digits, false);
	return res;
}

string BigInt::formatOutput(int x) {
	string s;
	s.clear();
	if (x == 0) {
		s = "0";
	}
	else {
		while (x != 0) {
			s = (char)('0' + x % 10) + s;
			x /= 10;
		}
	}
	while ((int)s.size() < BASE_LEN) {
		s = '0' + s;
	}
	return s;
}

bool BigInt::isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool BigInt::isZero() {
	return (digits.size() == 1 && digits[0] == 0);
}

bool BigInt::isPositiveOne() {
	return (!isNegative && digits.size() == 1 && digits[0] == 1);
}

bool BigInt::isNegativeOne() {
	return (isNegative && digits.size() == 1 && digits[0] == 1);
}

BigInt BigInt::sqrt() {
	if (*this < 0) {
		throw "ValueError";
	}
	BigInt l(0), r(*this);
	while (r - l > 1) {
		BigInt m = (l + r) / 2;
		if (m * m <= *this) {
			l = m;
		}
		else {
			r = m;
		}
	}
	if (r * r <= *this) {
		return r;
	}
	else {
		return l;
	}
}

BigInt BigInt::pow(BigInt n) {
	if (n < 0) {
		throw "ValueError";
	}
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return *this;
	}
	BigInt tmp = this->pow(n / 2);
	tmp = tmp * tmp;
	if (n % 2 == 1) {
		tmp = tmp * *this;
	}
	return tmp;
}

BigInt BigInt::pow(BigInt n, BigInt mod) {
	if (n < 0) {
		throw "ValueError";
	}
	//cout << *this << "^" << n << endl;
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return *this % mod;
	}
	BigInt tmp = this->pow(n / 2, mod);
	tmp = (tmp * tmp) % mod;
	if (n % 2 == 1) {
		tmp = (tmp * *this) % mod;
	}
	return tmp;
}

BigInt BigInt::mathMod(BigInt mod) {
	BigInt tmp = *this % mod;
	return tmp + (tmp < 0 ? mod : 0);
}

int BigInt::getLength() {
	return digits.size();
}

vector<int> BigInt::getDigits() {
	return digits;
}

// input, output

ostream& operator << (ostream& os, BigInt bigInt) {
	if (bigInt.isNegative) {
		os << "-";
	}
	os << bigInt.digits.back();
	for (int i = (int)bigInt.digits.size() - 2; i >= 0; i--) {
		os << BigInt::formatOutput(bigInt.digits[i]);
	}
	return os;
}

istream& operator >> (istream& in, BigInt& bigInt) {
	string s;
	in >> s;
	bigInt = BigInt(s);
	return in;
}

//comparing

bool operator == (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		return false;
	}
	if (bigInt1.digits.size() != bigInt2.digits.size()) {
		return false;
	}
	for (int i = 0; i < (int)bigInt1.digits.size(); i++) {
		if (bigInt1.digits[i] != bigInt2.digits[i]) {
			return false;
		}
	}
	return true;
}

bool operator == (BigInt bigInt1, int int2) {
	return bigInt1 == BigInt(int2);
}

bool operator == (int int1, BigInt bigInt2) {
	return BigInt(int1) == bigInt2;
}

bool operator != (BigInt bigInt1, BigInt bigInt2) {
	return !(bigInt1 == bigInt2);
}

bool operator != (BigInt bigInt1, int int2) {
	return bigInt1 != BigInt(int2);
}

bool operator != (int int1, BigInt bigInt2) {
	return BigInt(int1) != bigInt2;
}


bool operator > (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative && !bigInt2.isNegative) {
		return false;
	}
	if (!bigInt1.isNegative && bigInt2.isNegative) {
		return true;
	}

	if (!bigInt1.isNegative) {
		if (bigInt1.digits.size() > bigInt2.digits.size()) {
			return true;
		}
		if (bigInt1.digits.size() < bigInt2.digits.size()) {
			return false;
		}
		for (int i = bigInt1.digits.size() - 1; i >= 0; i--) {
			if (bigInt1.digits[i] > bigInt2.digits[i]) {
				return true;
			}
			if (bigInt1.digits[i] < bigInt2.digits[i]) {
				return false;
			}
		}
		return false;
	}
	else {
		return bigInt2.abs() > bigInt1.abs();
	}
}

bool operator > (BigInt bigInt1, int int2) {
	return bigInt1 > BigInt(int2);
}

bool operator > (int int1, BigInt bigInt2) {
	return BigInt(int1) > bigInt2;
}

bool operator >= (BigInt bigInt1, BigInt bigInt2) {
	return bigInt1 > bigInt2 || bigInt1 == bigInt2;
}

bool operator >= (BigInt bigInt1, int int2) {
	return bigInt1 >= BigInt(int2);
}

bool operator >= (int int1, BigInt bigInt2) {
	return BigInt(int1) >= bigInt2;
}


bool operator < (BigInt bigInt1, BigInt bigInt2) {
	return bigInt2 > bigInt1;
}

bool operator < (BigInt bigInt1, int int2) {
	return bigInt1 < BigInt(int2);
}

bool operator < (int int1, BigInt bigInt2) {
	return BigInt(int1) < bigInt2;
}

bool operator <= (BigInt bigInt1, BigInt bigInt2) {
	return bigInt1 < bigInt2 || bigInt1 == bigInt2;
}

bool operator <= (BigInt bigInt1, int int2) {
	return bigInt1 <= BigInt(int2);
}

bool operator <= (int int1, BigInt bigInt2) {
	return BigInt(int1) <= bigInt2;
}

// +, -, *, /

BigInt operator + (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		BigInt res;
		if (bigInt1.abs() > bigInt2.abs()) {
			res = BigInt((bigInt1.abs() - bigInt2.abs()).digits, bigInt1.isNegative);
		}
		else {
			res = BigInt((bigInt2.abs() - bigInt1.abs()).digits, bigInt2.isNegative);
		}
		res.clearNumber();
		return res;
	}
	else {
		vector <int> resDigits(1, 0);
		int carry = 0;
		for (int i = 0; i < max((int)bigInt1.digits.size(), (int)bigInt2.digits.size()) || carry; i++) {
			if (i == (int)resDigits.size()) {
				resDigits.push_back(0);
			}
			resDigits[i] = carry + (i < (int)bigInt1.digits.size() ? bigInt1.digits[i] : 0) + (i < (int)bigInt2.digits.size() ? bigInt2.digits[i] : 0);
			carry = resDigits[i] / BigInt::BASE;
			resDigits[i] %= BigInt::BASE;
		}
		BigInt res(resDigits, bigInt1.isNegative);
		res.clearNumber();
		return res;
	}
}

BigInt operator + (BigInt bigInt1, int int2) {
	return bigInt1 + BigInt(int2);
}

BigInt operator + (int int1, BigInt bigInt2) {
	return BigInt(int1) + bigInt2;
}

const BigInt operator ++ (BigInt& a, int) {
	BigInt oldA = a;
	a = a + 1;
	return oldA;
}


BigInt operator - (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		BigInt res((bigInt1.abs() + bigInt2.abs()).digits, bigInt1.isNegative);
		res.clearNumber();
		return res;
	}
	else
		if (bigInt1.isNegative) {
			return bigInt2.abs() - bigInt1.abs();
		}
		else
			if (bigInt1 < bigInt2) {
				return BigInt((bigInt2 - bigInt1).digits, true);
			}
			else {
				vector <int> resDigits = bigInt1.digits;
				int carry = 0;
				for (int i = 0; i < (int)bigInt2.digits.size() || carry; i++) {
					resDigits[i] -= carry + (i < (int)bigInt2.digits.size() ? bigInt2.digits[i] : 0);
					carry = resDigits[i] < 0;
					if (carry) {
						resDigits[i] += BigInt::BASE;
					}
				}
				BigInt res(resDigits, false);
				res.clearNumber();
				return res;
			}
}

BigInt operator - (BigInt bigInt1, int int2) {
	return bigInt1 - BigInt(int2);
}

BigInt operator - (int int1, BigInt bigInt2) {
	return BigInt(int1) - bigInt2;
}

BigInt BigInt::operator - () {
	return BigInt(digits, !isNegative);
}


BigInt operator * (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isZero() || bigInt2.isZero()) {
		return BigInt(0);
	}
	else
		if (bigInt1.isPositiveOne()) {
			return bigInt2;
		}
		else
			if (bigInt1.isNegativeOne()) {
				return -bigInt2;
			}
			else
				if (bigInt2.isPositiveOne()) {
					return bigInt1;
				}
				else
					if (bigInt2.isNegativeOne()) {
						return -bigInt1;
					}
					else {
						vector <int> resDigits(bigInt1.digits.size() + bigInt2.digits.size());
						for (int i = 0; i < (int)bigInt1.digits.size(); i++) {
							for (int j = 0, carry = 0; j < (int)bigInt2.digits.size() || carry; j++) {
								resDigits[i + j] += bigInt1.digits[i] * (j < (int)bigInt2.digits.size() ? bigInt2.digits[j] : 0) + carry;
								carry = resDigits[i + j] / BigInt::BASE;
								resDigits[i + j] %= BigInt::BASE;
							}
						}
						BigInt res(resDigits, bigInt1.isNegative ^ bigInt2.isNegative);
						res.clearNumber();
						return res;
					}
}

BigInt operator * (BigInt bigInt1, int int2) {
	return bigInt1 * BigInt(int2);
}

BigInt operator * (int int1, BigInt bigInt2) {
	return BigInt(int1) * bigInt2;
}


BigInt BigInt::div2() {
	int carry = 0;
	vector <int> resDigits = digits;
	for (int i = (int)resDigits.size() - 1; i >= 0; i--) {
		resDigits[i] = resDigits[i] + carry * BASE;
		carry = resDigits[i] % 2;
		resDigits[i] /= 2;
	}
	BigInt res(resDigits, isNegative);
	res.clearNumber();
	return res;
}

BigInt BigInt::mod2() {
	return (digits[0] % 2) * (isNegative ? -1 : 1);
}


BigInt operator / (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt2 == 0) {
		throw "DivisionByZero";
	}
	else
		if (bigInt1 == 0) {
			return 0;
		}
		else
			if (bigInt2.abs() == 1) {
				return bigInt1 * bigInt2;
			}
			else
				if (bigInt2 == 2) {
					return bigInt1.div2();
				}
				else
					if (bigInt1 < 0 || bigInt2 < 0) {
						return (bigInt1.abs() / bigInt2.abs()) * (bigInt1.isNegative ^ bigInt2.isNegative ? -1 : 1);
					}
					else {
						BigInt bigInt2Power2 = bigInt2;
						BigInt power2(1);
						while (bigInt2Power2 < bigInt1) {
							bigInt2Power2 = bigInt2Power2 * 2;
							power2 = power2 * 2;
						}
						BigInt tmp;
						BigInt res;
						while (power2 > 0) {
							if (tmp + bigInt2Power2 <= bigInt1) {
								tmp = tmp + bigInt2Power2;
								res = res + power2;
							}
							bigInt2Power2 = bigInt2Power2 / 2;
							power2 = power2 / 2;
						}
						return res;
					}


}

BigInt operator / (BigInt bigInt1, int int2) {
	return bigInt1 / BigInt(int2);
}

BigInt operator / (int int1, BigInt bigInt2) {
	return BigInt(int1) / bigInt2;
}


BigInt operator % (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt2 == 0) {
		throw "DivisionByZero";
	}
	else
		if (bigInt1 == 0 || bigInt2.abs() == 1) {
			return 0;
		}
		else
			if (bigInt2 == 2) {
				return bigInt1.mod2();
			}
			else
				if (bigInt1 < 0 || bigInt2 < 0) {
					return (bigInt1.abs() % bigInt2.abs()) * (bigInt1.isNegative ? -1 : 1);
				}
				else {
					BigInt bigInt2Power2 = bigInt2;
					BigInt power2(1);
					while (bigInt2Power2 < bigInt1) {
						bigInt2Power2 = bigInt2Power2 * 2;
						power2 = power2 * 2;
					}
					BigInt tmp;
					BigInt res;
					while (power2 > 0) {
						if (tmp + bigInt2Power2 <= bigInt1) {
							tmp = tmp + bigInt2Power2;
							res = res + power2;
						}
						bigInt2Power2 = bigInt2Power2 / 2;
						power2 = power2 / 2;
					}
					return bigInt1 - tmp;
				}


}

BigInt operator % (BigInt bigInt1, int int2) {
	return bigInt1 % BigInt(int2);
}

BigInt operator % (int int1, BigInt bigInt2) {
	return BigInt(int1) % bigInt2;
}



BigInt test_FastPow_Mod(BigInt b, BigInt n, BigInt m) {
	return(b.pow(n) % m);
}

BigInt karatsuba(BigInt first, BigInt second) {
	if (first.getLength() % 2 != 0 || second.getLength() % 2 != 0)
		throw "Both numbers must have even number of digits";
	if (first.getLength() != second.getLength())
		throw "Nubmers have different size!";
	
	int x = first.getLength() / 2;
	BigInt a = first / x;
	BigInt b = first % x;
	BigInt c = second / x;
	BigInt d = second % x;

	BigInt a_m_c = a * c;
	BigInt b_m_d = b * d;
	
	return a_m_c * x * x + ((a + b) * (c + d) - a_m_c - b_m_d) * x + b_m_d;
}

BigInt randBigInt(BigInt p) {
	int len = p.getLength();
	vector<int> digits(len);
	for (int i = 0; i + 1 < len; i++) {
		digits[i] = rand() % BigInt::BASE;
	}
	auto pDigits = p.getDigits();
	digits[len - 1] = rand() % pDigits[len - 1];
	return BigInt(digits, false);
}

bool MillerRabinTest(BigInt n, int k) {
	if (n == 2 || n == 3)
		return true;
	if (n < 2 || n % 2 == 0)
		return false;

	BigInt t = n - 1;
	int s = 0;
	while (t % 2 == 0) {
		t = t.div2();
		s++;
	}
	for (int i = 0; i < k; i++) {
		BigInt rand = randBigInt(n - 2) + 2;
		BigInt x = rand.pow(t, n);

		if (x == 1 || x == n - 1) 
			continue;

		for (int j = 0; j < s - 1; j++) {
			x = x.pow(2, n);
			if (x == 1)
				return false;
			if (x == n - 1)
				break;
		}

		if (x != n - 1)
			return false;
	}
	return true;
}

bool MillerRabinTest_Base(BigInt n, int base) {
	if (n < 2 || n % 2 == 0)
		return false;

	BigInt t = n - 1;
	int s = 0;
	while (t % 2 == 0) {
		t = t.div2();
		s++;
	}

	BigInt a = BigInt(base);
	BigInt k = a.pow(t, n);

	if (k == 1 || k == -1)
		return true;

	for (int i = 0; i < s - 1; i++) {
		k = k.pow(2, n);
		if (k == 1)
			return false;
		if (k == -1)
			return true;
	}
	return false;
}

BigInt gcd(BigInt a, BigInt b) {
	if (a == 0)
		return b;

	else
		return gcd(b % a, a);
	
}

BigInt gcd(int a, BigInt b) {
	if (a == 0)
		return b;

	else
		return gcd(b % a, a);

}

BigInt jacobi(BigInt n, BigInt m) {
	n = n % m;
	if (n == 0 && m == 1) {
		return 1;
	}
	if (n == 0 && m > 1) {
		return 0;
	}

	BigInt ans = 1;
	while (n % 2 == 0) {
		n = n / 2;
		BigInt r = m % 8;
		if (r == 3 || r == 5) {
			ans = -ans;
		}
	}
	if (n % 4 == 3 && m % 4 == 3) {
		ans = -ans;
	}
	return ans * jacobi(m, n);
}

int jacobi_(BigInt a, BigInt b) {
	if (gcd(a, b) != 1)
		return 0;
	int r = 1;
	if (a.isNegativeOne()){
		a.abs();
		if (b % 4 == 3)
			r = -r;
	}
	while (true) {
		int t = 0;
		while (a % 2 == 0) {
			t++;
			a = a / 2;
		}
		if (t % 2 == 1) {
			if (b % 8 == 3 || b % 8 == 5)
				r = -r;
		}

		if (a % 4 == b % 4 == 3)
			r = -r;
		BigInt c = a;
		a = b % c;
		b = c;

		if (a != 0)
			continue;
		else
			return r;
	}
}

int jacobi_(int a_int, BigInt b) {
	
	BigInt a = BigInt(a_int);

	if (gcd(a, b) != 1)
		return 0;
	int r = 1;
	if (a.isNegativeOne()) {
		a.abs();
		if (b % 4 == 3)
			r = -r;
	}
	while (true) {
		int t = 0;
		while (a % 2 == 0) {
			t++;
			a = a / 2;
		}
		if (t % 2 == 1) {
			if (b % 8 == 3 || b % 8 == 5)
				r = -r;
		}

		if (a % 4 == b % 4 == 3)
			r = -r;
		BigInt c = a;
		a = b % c;
		b = c;

		if (a != 0)
			continue;
		else
			return r;
	}
}

int LucasSelfridgeTest(BigInt n) {
	int d = 5;
	int i = 2;
	while (true) {
		if (jacobi_(d, n) == -1)
			return d;
		i++;
		d = (abs(d) + 2) * pow(-1, i);
	}
}

bool BailliePSWTest(BigInt n) {
	bool res_1 = MillerRabinTest_Base(n, 2);

	int d = LucasSelfridgeTest(n);
	int p = 1;
	int q = (1 - d) / 4;
	BigInt a = BigInt((p + sqrt(d)) / 2);
	BigInt b = BigInt((p - sqrt(d)) / 2);
	BigInt v = a.pow(n) + b.pow(b);
	
	int res_2;

	if (v % n == n)
		res_2 = 1;
	else
		res_2 = 0;
	if (res_1 == res_2 == 1)
		return true;
	else 
		return false;
}

void print_base_2(BigInt n) {
	int s = 0;
	BigInt t = n;
	vector<int> res;
	res.push_back(0);
	while (t / 2 != 0) {
		t = t / 2;
		s++;
		res.push_back(0);
	}
	res[s] = 1;
	while (true)
	{
		n = n - pow(2, s);
		if (n == 0)
			break;
		t = n;
		s = 0;
		while (t / 2 != 0) {
			t = t / 2;
			s++;
		}
		res[s] = 1;
	}

	for (int i = res.size() - 1; i >= 0; i--) {
		cout << res[i];
	}
}

int* char_base2(char a) {
	int int_a = int(a);
	int t = int_a;
	int* byte_a = new int[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
	int s = 0;
	
	while (true)
	{
		if (int_a % 2 == 1) {
			byte_a[7] = 1;
			int_a--;
		}

		if (int_a == 0)
			break;
		while (t / 2 != 0) {
			t = t / 2;
			s++;
		}
		t = int_a - pow(2, s);
		int_a = t;
		byte_a[7 - s] = 1;
		s = 0;
		
	}
	return byte_a;
}

int calc_int_from_base2(vector<int> arr) {
	int s = 5;
	int res = 0;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == 1) {
			res += pow(2, s);
		}
		s--;
	}
	return res;
}

int* get_indexes(char* arr, int status_) {
	vector<int> bits;
	int* indexes = new int[4];
	//initialize bits
	for (int i = 0; i < 3; i++) {
		int* bits_8 = char_base2(arr[i]);
		for (int j = 0; j < 8; j++) {
			bits.push_back(bits_8[j]);
		}
	}
	// handle situations
	if (status_ == 1) {
		for (int i = 16; i < bits.size(); i++)
			bits[i] = 0;
	}
	if (status_ == 2) {
		for (int i = 8; i < bits.size(); i++)
			bits[i] = 0;
	}
	
	// calculate indexes
	vector<int> bits_6;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			bits_6.push_back(bits[j + (i * 6)]);
		}
		indexes[i] = calc_int_from_base2(bits_6);
		bits_6.clear();
	}

	return indexes;
}

string n_to_str(BigInt n) {
	string res = "";
	vector<int> dig = n.getDigits();
	for (int i = dig.size()-1; i >= 0 ; i--) {
		res = res + to_string(dig[i]);
	}
	dig.clear();
	/*string reverse_res = "";
	for (int i = res.size() - 1; i >= 0; i--) {
		reverse_res += res[i];
	}
	return reverse_res;
	*/
	return res;
}

void print_base_64(BigInt n) {
	string BASE_64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string res = "";
	int status_ = 0;
	string digits = n_to_str(n);
	int length = digits.length();

	char* chars = new char[3];
	int* indexes = new int[3];
	for (int i = 0; i  < length; i += 3) {
		if (i + 1 == length - 1) {
			chars[0] = char(digits[i]);
			chars[1] = char(digits[i + 1]);
			chars[2] = '0';
			status_ = 1;
		}
		else if (i == length - 1) {
			chars[0] = char(digits[i]);
			chars[1] = '0';
			chars[2] = '0';
			status_ = 2;
		}
		else {
			chars[0] = char(digits[i]);
			chars[1] = char(digits[i + 1]);
			chars[2] = char(digits[i + 2]);
			status_ = 0;
		}

		indexes = get_indexes(chars, status_);
		for (int i = 0; i < 4; i++) {
			res = res + BASE_64[indexes[i]];
		}
		if (status_ != 0)
			break;
	}
	if (status_ == 1) 
		res[int(res.size()) - 1] = '=';
	
	if (status_ == 2) 
		res[int(res.size()) - 1] = res[int(res.size()) - 2] = '=';

	cout << "\nEncoded \n" << n << "\nas \n" << res;
}

int main() {
	print_base_64(BigInt("11111233"));
	
}
