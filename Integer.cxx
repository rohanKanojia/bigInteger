#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cstring>
#include "Integer.hxx"

std::ostream& operator<<(std::ostream& tmp, const Integer& a) {
  int i = 0;
  while(i < SIZE && a.num[i] == '0') i++;
  if(i == SIZE)
    tmp << 0;
  else {
    while(i < SIZE) {
      tmp << a.num[i++];
    }
  }
  return tmp;
}

bool operator>(const Integer& b, const Integer& a) {
  if(b.sign == '-' && a.sign == '+') {
    return false;
  }
  else if(a.sign == '-' && b.sign == '+') {
    return true;
  }
  else if(b.getSize() > a.getSize()) {
    return true;
  }
  else if(b.getSize() < a.getSize()) {
    return false;
  }
  else if(b.getSize() == a.getSize()) {
    int i = b.getCapacity() - a.getSize();
    while(i != b.getCapacity()) {
      if(b.getDigit(i) > a.getDigit(i)) {
        return true;
      }
      else if(b.getDigit(i) < a.getDigit(i)) {
        return false;
      }
      i++;
    }
    return false;
  }
  else
    return false;
}

Integer operator+(const Integer& a, const Integer& b) {
  int index, tmp = 0, carry = 0;
  Integer C;

  for(int i = a.getCapacity()-1; i >= 0; i--) {
    tmp = a.getDigit(i) + b.getDigit(i) + C.getDigit(i);
    carry = tmp / 10;
    tmp = tmp % 10;
    C.num[i] = tmp + '0';
    if(i > 0) {
      C.num[i-1] = C.num[i-1] + carry;
    }
  }
  C.findDigits();
  return C;
}

Integer operator-(const Integer& b, const Integer& a) {
  Integer res;
  if(b > a) {
    res = subtractionProcess(b, a);
  }
  else {
    res = subtractionProcess(a, b);
    res.sign = '-';
  }
  return res;
}

Integer subtractionProcess(Integer b, Integer a) {
  Integer C;
  int i, tmp, neighbour, j;

  for(i = a.getCapacity()-1; i >= 0; i--) {
    if(b.getDigit(i) >= a.getDigit(i)) {
      tmp = b.getDigit(i) - a.getDigit(i);
    }
    else {
      neighbour = i - 1;
      while(neighbour > 0 && b.num[neighbour] == '0') { neighbour--; }

      for(j = neighbour; j < i; j++) {
        b.num[j] = b.num[j]-1;
        b.num[j+1] = b.num[j+1] + 10;
      }
      tmp = b.getDigit(i) - a.getDigit(i);
    }
    C.num[i] = tmp + '0';
  }
  C.findDigits();

  return C;
}

Integer addOffset(Integer& a, Integer& b, int offset) {
  int temp, carry, index, a_index = a.getCapacity()-1;
  Integer C = a;
  for(index = a.getCapacity()-offset; a_index >= 0; index--, a_index--) {
    temp = C.getDigit(index) + b.getDigit(a_index);
    carry = temp / 10;
    temp %= 10;
    C.num[index] = temp + '0';
    if(index > 0)
      C.num[index-1] = C.num[index-1] + carry;
  }
  C.findDigits();
  return C;
}

Integer multiplyIntermediate(char a, const Integer& b) {
  int i, product = 0, carry = 0;
  Integer temp;

  for(i = b.getCapacity()-1; i >= 0; i--) {
    product = temp.getDigit(i) + (a-'0')*(b.getDigit(i));
    carry = product / 10;
    product %= 10;
    temp.num[i] = product + '0';
    if(i > 0)
      temp.num[i-1] = carry + temp.num[i-1];
  }
  temp.findDigits();
  return temp;
}

Integer operator*(const Integer& a, const Integer& b) {
  int offset = 1, i;
  Integer temp, res;
  for(int i = a.getCapacity()-1; i > a.getCapacity()-1-b.getSize(); i--) {
    temp = multiplyIntermediate(b.num[i], a);
    res = addOffset(res, temp, offset);
    offset++;
  }
  res.findDigits();
  res.sign = a.sign == b.sign ? '+' : '-';
  return res;
}

Integer findNearestMultiple(const Integer& a, const Integer& b)
{
  Integer  product = b, one;
  Integer i;
  one.set_integer("1");
  i = one;
  while(a >= product) {
    i = i + one;
    product = b * i;
  }
  i = i - one;

  return i;
} 

Integer addDigitToRight(Integer temp, char ch)
{
  Integer a, ten;
  char digit[2];
  sprintf(digit, "%c", ch); 
  a.set_integer(digit);
  ten.set_integer("10");
  temp = temp * ten;
  temp = temp + a;
  return temp;
}

Integer operator/(const Integer& b, const Integer& a)
{
  Integer divisor, dividend, tmp, multiple, quotient_digit, q;
  char buffer[SIZE], quotient_str[SIZE];
  memset(buffer, '\0', SIZE);
  memset(quotient_str, '\0', SIZE);
  int j = 0, k;
  divisor = absolute(a);
  dividend = absolute(b);
  if(divisor > dividend) {
    tmp.set_integer("0");
    return tmp;
  }
  k = SIZE - (b.size);
  subString(buffer, dividend.num, k, k + a.size);
  k = k + a.size - 1;
  tmp.set_integer(buffer);
  while(k < SIZE) {
    while(divisor > tmp  &&  k < SIZE) {
      quotient_str[j++] ='0';
      tmp = addDigitToRight(tmp, dividend.num[++k]);
    }
    if(k == SIZE) break;
    quotient_digit = findNearestMultiple(tmp, divisor);
    multiple = quotient_digit * divisor;
    quotient_str[j++] = quotient_digit.num[SIZE - 1];
    tmp = tmp - multiple;
    tmp = addDigitToRight(tmp, dividend.num[++k]);
    tmp.findDigits();
  }
  q.set_integer(quotient_str);
  q.sign = (a.sign == b.sign) ? '+' : '-';
  return q;
}

void Integer::set_integer(const char buffer[])
{
  int buffer_index = strlen(buffer)-1, index = SIZE - 1;
  sign = (buffer[0] == '-') ? '-' : '+';
  while(buffer_index >= 0) {
    if(buffer[buffer_index] != '-') {
      num[index] = buffer[buffer_index];
      buffer_index --;
      index --;
    }
    else break;
  }
  size = (buffer[0] == '-') ? strlen(buffer) - 1 : strlen(buffer);
}

bool operator>=(const Integer& b, const Integer& a)
{
  if(b == a)
    return 1;
  else 
    return (b > a);
}

bool operator==(const Integer& a, const Integer& b)
{
  if(a.size == b.size   &&   a.sign == b.sign) {
    int i = SIZE - (a.size);
    while(i != SIZE) {
      if(b.num[i] != a.num[i])
        return 0;
      else 
        i++;
    }
    return 1;
  }
  else
    return 0;
}

Integer absolute(const Integer& a)
{
  Integer minusOne, zero, temp;
  minusOne.set_integer("-1");
  zero.set_integer("0");
  if(a > zero) 
    return a;
  else {
    temp = a * minusOne;
    return temp; 
  }
}

void subString(char buffer[], char num[], int start, int end)
{
  int i;
  for(i = 0; start < end; start++, i++) {
    buffer[i] =num[start]; 
  }
  buffer[i] = '\0';
}

Integer operator%(const Integer& a, const Integer& b)
{
  Integer q, r;
  q = a / b;
  q = q * b;
  r = a - q;
  return r;
}

Integer operator>>(const Integer& a, const Integer& b)
{
  Integer i;
  Integer res = a, two, one;
  i.set_integer("0");
  two.set_integer("2");
  one.set_integer("1");
  res = a;
  for(; i < b; i = i + one) {
    res = res / two;
  }
  return res;
}

Integer operator<<(const Integer& a, const Integer& b)
{
  Integer i, one;
  i.set_integer("0");
  one.set_integer("1");
  Integer res = a, two;
  two.set_integer("2");
  while(i < b) {
    res = res * two;
    i = i + one;
  }
  return res;
}

bool operator<(const Integer& b, const Integer& a)
{
  if(a == b)
    return false;
  else
    return !(operator>(b, a));
}
