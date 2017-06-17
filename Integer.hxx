#ifndef INTEGER
#define INTEGER
#include <string>

#define SIZE 1000

void subString(char buffer[], char num[], int start, int end);

class Integer {
  char num[SIZE];
  long long size;
  long long capacity;
  char sign;
  void initializeStr(int limit) {
    for(int i = 0; i < limit; i++)
      num[i] = '0';
  }
public:
  static Integer ONE, TEN, ZERO;
  Integer() : size(0), capacity(0) {
    capacity = SIZE;
    sign = '+';
    initializeStr(capacity);
  }
  Integer(std::string aStr) : size(aStr.length()), capacity(0) {
    capacity = SIZE;
    if(aStr.find('-') != std::string::npos) {
      sign = '-';
    }
    else
      sign = '+';
    initializeStr(capacity);
    for(int i = capacity-1, j = aStr.length()-1; i >= 0 && j >= 0;  j--, i--) {
      num[i] = aStr[j];
    }
  }
  Integer(const Integer& a) {
    capacity = a.getCapacity();
    for(int index = 0; index < capacity; index++) {
      num[index] = a.getChar(index);
    }
    sign = a.getSign();
    size = a.getSize();
  }
  int getDigit(int index) const  { 
    return num[index] - '0'; 
  }
  char getChar(int index) const  { return num[index]; }
  char getSign() const           { return sign; }
  long long getSize() const      { return size; }
  long long getCapacity() const  { return capacity; }
  void findDigits() {
    int i = 0;
    while(i < SIZE && num[i] == '0') i++;
    size = capacity - i;
    if(size == 0) // In case of zero
      size = 1;
  }
  friend std::ostream& operator<<(std::ostream& tmp, const Integer& a);
  friend std::istream& operator>>(std::istream& aStream, Integer& a);
  friend bool operator>(const Integer& b, const Integer& a);
  friend Integer operator+(const Integer& a, const Integer& b);
  friend Integer operator-(const Integer& b, const Integer& a);
  friend Integer subtractionProcess(Integer a, Integer b);
  friend Integer addOffset(Integer& a, Integer& b, int offset);
  friend Integer multiplyIntermediate(char a, const Integer& b);
  friend Integer operator*(const Integer& a, const Integer& b);
  friend Integer findNearestMultiple(const Integer& a, const Integer& b);
  friend Integer addDigitToRight(Integer temp, char ch);
  friend Integer operator/(const Integer& b, const Integer& a);
  friend bool operator>=(const Integer& b, const Integer& a);
  friend bool operator==(const Integer& a, const Integer& b);
  friend bool operator!=(const Integer& a, const Integer& b);
  friend Integer absolute(const Integer& a);
  friend Integer operator%(const Integer& a, const Integer& b);
  friend Integer operator<<(const Integer& a, const Integer& b);
  friend Integer operator>>(const Integer& a, const Integer& b);
  friend bool operator<(const Integer& b, const Integer& a);
  friend Integer pow(const Integer& a, int exponent);
  friend Integer gcd(const Integer& a, const Integer& b);
  std::string toString();
  std::string toBinaryString();
  long toLongValue() const;
  void set_integer(const char *buffer);
};

#endif
