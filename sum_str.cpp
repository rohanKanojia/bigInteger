#include <iostream>
#include <cstring>
#include <cstdio>
#include <curses.h>
#include <cstdlib>
#include <unistd.h>

#define SIZE 80
#define STATUS_ROW 25
#define STATUS_COL 35

using namespace std;
int selected = 10;
int gridR = 7, gridC = 32;

class Integer
{
  char str[SIZE], buffer[SIZE];
  char sign;
  int no_of_digits;
public :
  Integer()
  {
    int index;
    for(index = 0; index < SIZE; index++) 
      str[index] = '0';
    str[SIZE-1] = '\0';
    sign = '+';
    no_of_digits = 0;
  }
  Integer(Integer& tmp) 
  {
    int i;
    for(i = 0; i < SIZE; i++)
      str[i] = tmp.str[i];
    sign = tmp.sign;
    no_of_digits = tmp.no_of_digits;
  }
  Integer(char buffer[]) 
  {
    this->set_integer(buffer);
  }
  void set_integer(char buffer[])
  {
    int buffer_index = strlen(buffer), index = SIZE - 1;
    sign = (buffer[0] == '-') ? '-' : '+';
    while(buffer_index >= 0) {
      if(buffer[buffer_index] != '-') {
	str[index] = buffer[buffer_index];
	buffer_index --;
	index --;
      }
      else break;
    }
    no_of_digits = (buffer[0] == '-') ? strlen(buffer) - 1 : strlen(buffer);
  }
  int check(void)
  {
    if(strlen(buffer) > 80) {
      cout << "Exception : length of numbers greater than 80\n";
      cout << "program failure...\n";
      exit(0);
    }
    int index;
    for(index = strlen(buffer) - 1; index >= 1; index--) {
      if(!(buffer[index] > 47 && buffer[index] < 58)) {
	cout << "invalid integer entered ...\n"
	     << "encountered symbol '" << buffer[index] << "' at index : " << index
	     << "program failure ...\n";
	exit(0);
      }
    }				
  }
  void setZero()
  {
    int i;
    for(i = 0; i < SIZE-1; i++)
      str[i] = '0';
    str[SIZE-1] = '\0';
  }
  int findDigits() 
  {
    int i = 0;
    while(str[i] == '0' && i < (SIZE-1))
      i++;
    no_of_digits =  SIZE - (i+1);
    return no_of_digits;
  }
  void CursesInput()
  {
    scanw("%s", buffer);
    check();
    set_integer(buffer);
    findDigits();
  }
  void CursesOutput(int row, int col)
  {
    int i = SIZE - 1 - no_of_digits;  
    attron(COLOR_PAIR(2));
    move(row, col);
    if(no_of_digits == 0) printw("0");
    if(sign == '-')  printw("%c", sign);
    while(i < SIZE)
      printw("%c", str[i++]);       
  }
  void Curses_Output(int row, int col)
  {
    move(row, col);
    attron(COLOR_PAIR(2));
    int j = SIZE - 2;
    for(int i = 0; i < no_of_digits; i++)
      mvprintw(row, col--, "%c", str[j--]);
  }
  friend Integer absolute(Integer a);
  friend Integer add(Integer , Integer );
  friend istream& operator>>(istream &, Integer &);
  friend ostream& operator<<(ostream&, Integer& );
  friend Integer multiplyIntermediate(char a, Integer b);
  friend Integer multiply(Integer a, Integer b);
  friend Integer addOffset(Integer a, Integer b, int offset);
  friend Integer Subtract(Integer b, Integer a);
  friend Integer SubtractionProcess(Integer b, Integer a);
  friend int operator>(Integer b, Integer a);
  friend int operator<(Integer b, Integer a);
  friend Integer operator+(Integer a, Integer b);
  friend Integer operator-(Integer a, Integer b);
  friend Integer operator*(Integer a, Integer b);
  friend Integer operator/(Integer a, Integer b);
  friend Integer operator%(Integer a, Integer b);
  friend Integer findNearestMultiple(Integer a, Integer b);
  friend void subString(char buffer[], char str[], int start, int end);
  friend Integer addDigitToRight(Integer temp, char ch);
  friend int operator>=(Integer a, Integer b);
  friend int operator<=(Integer a, Integer b);
  friend int operator==(Integer a, Integer b);
  friend Integer squareRoot(Integer a);
  friend Integer operator>>(Integer a, Integer b);
  friend Integer operator<<(Integer a, Integer b);
  friend Integer guessRoot(Integer tmp, Integer a);
  friend Integer CursesAdd(Integer a, Integer b);
  friend Integer CursesSubtract(Integer, Integer);
  friend Integer CursesSubtractionProcess(Integer, Integer);
  friend Integer CursesMultiply(Integer, Integer);
  friend Integer CursesDivide(Integer, Integer);
};

void rectangle(int r_start, int r_end, int C_START, int C_END ,char ch)
{
    int r, c;
    for(r = r_start; r < r_end; r++)
      for(c = C_START; c < C_END ; c++)
         mvprintw(r, c, "%c",ch);
}

Integer operator%(Integer a, Integer b)
{
  Integer q, r;
  q = a / b;
  q = q * b;
  r = a - q;
  return r;
}

Integer operator>>(Integer a, Integer b)
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

Integer operator<<(Integer a, Integer b)
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

istream& operator>>(istream& tmp, Integer& a)
{
  fgets(a.buffer, 80, stdin);
  a.buffer[strlen(a.buffer) - 1] = '\0';
  a.check();
  a.set_integer(a.buffer); 
  return tmp;
}

ostream& operator<<(ostream& tmp, Integer& a)
{
  int i = SIZE - 1 - a.no_of_digits; 
  if(a.no_of_digits == 0) cout << "0";
  if(a.sign == '-') cout << a.sign;
  while(i < SIZE)
    cout << a.str[i++];
  return tmp;
}

Integer add(Integer a, Integer b)
{
  int index, temp = 0, carry = 0;
  Integer C;
  if(a.sign == b.sign) {
    C.str[SIZE - 1] = '\0';
    for(index = 78; index >= 0; index--) {
      temp = (C.str[index] - 48 + a.str[index] - 48 + b.str[index] - 48) % 10;
      if(index != 0) 
        carry = (C.str[index] - 48 + a.str[index] -48 + b.str[index] - 48) / 10;
      C.str[index] = temp + 48;
      C.str[index - 1] = carry + 48;
    }
  }
  else
    C = a - b;
  C.sign = (a.sign == '+') ? '+' : '-';  
  C.findDigits();
  return C;
}

Integer CursesAdd(Integer a, Integer b)
{
  int index, temp = 0, carry = 0, col = 155;
  Integer C;
  mvprintw(17, 73, "a : %s", a.str);
  mvprintw(18, 73, "b : %s", b.str);
  rectangle(19, 20, 73, 158, '-'); 
  if(a.sign == b.sign) {
    C.str[SIZE - 1] = '\0';
    for(index = 78; index >= 0; index--) {
      move(20, col);
      temp = (C.str[index] - 48 + a.str[index] - 48 + b.str[index] - 48) % 10;
      if(index != 0) 
        carry = (C.str[index] - 48 + a.str[index] -48 + b.str[index] - 48) / 10;
      C.str[index] = temp + 48;
      C.str[index - 1] = carry + 48;
      printw("%c", C.str[index]);
      col--;
      refresh();
      usleep(100*1000);
    }
  }
  else
    C = a - b;
  C.sign = (a.sign == '+') ? '+' : '-';  
  C.findDigits();
  refresh();
  return C;
}

Integer multiplyIntermediate(char a, Integer b)
{
  int i = SIZE - 2, product = 0, carry = 0;
  Integer temp ;
  for(i = SIZE - 2; i >= 0; i--) {
    product = (temp.str[i] - 48 + (a - 48)*(b.str[i] - 48)) % 10;
    if(i != 0) 
      carry = (temp.str[i] - 48+ (a - 48)*(b.str[i] - 48)) / 10;
    temp.str[i] =  product + 48;
    temp.str[i - 1] =  carry  + temp.str[i - 1];
  }
  temp.findDigits();
  return temp;
}

Integer multiply(Integer a, Integer b)
{
  int offset = 2, i;
  Integer temp, res;
  for(i = SIZE - 2; i >= 0; i--) {
    temp = multiplyIntermediate(a.str[i], b);
    res = addOffset(temp, res, offset);
    offset++;
  }
  res.no_of_digits = res.findDigits();
  res.sign = a.sign == b.sign ? '+' : '-';
  return res;
}

Integer CursesMultiply(Integer a, Integer b)
{
  int offset = 2, i, row = 20, col = 155, x, no_x;
  Integer temp, res, zero;
  zero.set_integer("0");
  mvprintw(17, 73, "a : ");
  b.Curses_Output(17, 155);
  mvprintw(18, 73, "b : ");
  a.Curses_Output(18, 155);
  rectangle(19, 20, 73, 158, '-'); 
  for(i = SIZE - 2; i >= 0; i--) {
    col = 155;
    move(row, col);
    temp = multiplyIntermediate(a.str[i], b);
    if(temp.no_of_digits == 0)
      break; 
    no_x = 0;
    attron(A_BOLD);
    for(x = i; x < SIZE-2; x++) 
      mvprintw(row, col--, "x");
    attroff(A_BOLD);
    temp.Curses_Output(row, col);
      res = addOffset(temp, res, offset);
    row++; 
    refresh();
    usleep(100*1000);
    offset++;
  }
  rectangle(row, row+1, 73, 158, '-');
  res.findDigits();
  res.Curses_Output(row + 1, col);
  res.sign = a.sign == b.sign ? '+' : '-';
  return res;
}

Integer addOffset(Integer a, Integer b, int offset)
{
  int temp, carry, index, a_index = SIZE - 2;
  Integer C = b;
  for(index = SIZE - offset; a_index >= 0; index--, a_index--) {
    temp = (C.str[index] - 48 + a.str[a_index] - 48 ) % 10;
    if(index != 0) 
      carry = (C.str[index] - 48 + a.str[a_index] -48 ) / 10;
    C.str[index] = temp + 48;
    C.str[index - 1] = C.str[index - 1] + carry ;
  }
  return C;
}

Integer Subtract(Integer b, Integer a)
{
  Integer c;
  if(a ==  b) {
    c.set_integer("0");
    c.sign = '+';
    c.no_of_digits = 1;
  }
  else if(b > a) {
    c = SubtractionProcess(b, a);
    c.sign = '+';
    c.no_of_digits = c.findDigits();
  }
  else if(b < a) {
    c = SubtractionProcess(a, b);
    c.sign = '-';
    c.no_of_digits = c.findDigits();
  }

  return c;
}


Integer CursesSubtract(Integer b, Integer a)
{
  Integer c;
  if(a ==  b) {
    c.set_integer("0");
    c.sign = '+';
    c.no_of_digits = 1;
  }
  else if(b > a) {
    c = CursesSubtractionProcess(b, a);
    c.sign = '+';
    c.no_of_digits = c.findDigits();
  }
  else if(b < a) {
    c = CursesSubtractionProcess(a, b);
    c.sign = '-';
    c.no_of_digits = c.findDigits();
  }

  return c;
}

Integer SubtractionProcess(Integer b, Integer a)
{
  Integer c;
  int i, temp, neighbour, j;
  for(i = SIZE - 2; i >= 0; i--) {
    if(b.str[i] >= a.str[i]) 
      temp = b.str[i] - a.str[i];
    else {
      neighbour = i - 1;
      while(b.str[neighbour] == '0' ) {
	neighbour--;
	if(neighbour <= 0) break;
      }
      for(j = neighbour; j < i; j++) {
	b.str[j] = b.str[j] - 1;
	b.str[j+1] = b.str[j+1] + 10;
      }
      temp = b.str[i] - a.str[i];
    }
    c.str[i] = temp + 48;
  }
  return c;
} 

Integer CursesSubtractionProcess(Integer b, Integer a)
{
  Integer c;
  int i, temp, neighbour, j, col = 155;
  mvprintw(17, 73, "a : %s", b.str);
  mvprintw(18, 73, "b : %s", a.str);
  rectangle(19, 20, 73, 158, '-'); 
  for(i = SIZE - 2; i >= 0; i--) {
    move(20, col--);
    if(b.str[i] >= a.str[i]) 
      temp = b.str[i] - a.str[i];
    else {
      neighbour = i - 1;
      while(b.str[neighbour] == '0' ) {
	neighbour--;
	if(neighbour <= 0) break;
      }
      for(j = neighbour; j < i; j++) {
	b.str[j] = b.str[j] - 1;
	b.str[j+1] = b.str[j+1] + 10;
      }
      temp = b.str[i] - a.str[i];
    }
    c.str[i] = temp + 48;
    printw("%c", c.str[i]);
    refresh();
    usleep(100*1000);
  }
  return c;
}

Integer absolute(Integer a)
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

int operator>(Integer b, Integer a)
{
  if(b.sign == '-' && a.sign == '+')
    return 0;
  else if(a.sign == '-' && b.sign == '+')
    return 1;
  else if(b.no_of_digits > a.no_of_digits)
    return 1;
  else if(b.no_of_digits < a.no_of_digits)
    return 0;
  else if(b.no_of_digits == a.no_of_digits) {
    int i = SIZE - (a.no_of_digits + 1);
    while(i != SIZE-1) {
      if(b.str[i]  >  a.str[i])
        return 1;
      else if(b.str[i] < a.str[i])
        return 0; 
      i++;
    }
    return 0;
  }
  else 
    return 0;
}

int operator<(Integer b, Integer a)
{
  if(a == b)
    return 0;
  else
    return !(operator>(b, a));
}

Integer operator+(Integer a, Integer b)
{
  Integer c;
  c =  add(a, b);
  return c;
}

int operator>=(Integer b, Integer a)
{
  if(b == a)
    return 1;
  else 
    return (b > a);
}

int operator<=(Integer b, Integer a)
{
  if(a == b)
    return 1;
  else
    return (b < a);
}

int operator==(Integer a, Integer b)
{
  if(a.no_of_digits == b.no_of_digits   &&   a.sign == b.sign) {
    int i = SIZE - (a.no_of_digits + 1);
    while(i != SIZE-1) {
      if(b.str[i] != a.str[i])
        return 0;
      else 
        i++;
    }
    return 1;
  }
  else
    return 0;
}

Integer operator-(Integer b, Integer a)
{
  Integer c ;
  c = Subtract(b, a);
  return c;
}

Integer operator*(Integer b, Integer a)
{
  Integer c ;
  c = multiply(b, a);
  return c;
}

void subString(char buffer[], char str[], int start, int end)
{
  int i;
  for(i = 0; start < end; start++, i++) 
    buffer[i] = str[start]; 
  buffer[i] ='\0';
}

Integer operator/(Integer b, Integer a)
{
  //cout << "In operator/(" << b << ", " << a << ") \n";
  Integer divisor, dividend, tmp, multiple, quotient_digit, q;
  char buffer[20], quotient_str[80];
  int j = 0, k;
  divisor = absolute(a);
  dividend = absolute(b);
  if(divisor > dividend) {
    tmp.set_integer("0");
    return tmp;
  }
  k = SIZE - (b.no_of_digits + 1);
  subString(buffer, dividend.str, k, k + a.no_of_digits);
  k = k + a.no_of_digits - 1;
  tmp.set_integer(buffer);
  //cout << "tmp : " << tmp << endl;
  while(k < (SIZE-1)) {
    //cout << "\tk : " << k << " ============================\n";
    while(divisor > tmp  &&  k < (SIZE-1)) {
      quotient_str[j++] ='0';
      //cout << "\t\tquotient_str[" << j-1 << "] : " << '0' << endl;
      tmp = addDigitToRight(tmp, dividend.str[++k]);
      //cout << "\t\ttmp : " << tmp << endl;
    }
    if(k == SIZE-1) break;
    quotient_digit = findNearestMultiple(tmp, divisor);
    //cout << "\tquotient_digit = " << quotient_digit << endl;
    multiple = quotient_digit * divisor;
    //cout << "\tmultiple = " << quotient_digit << " * " << divisor << endl;
    //cout << "\tmultiple : " << multiple << endl;
    //cout << "\tquotient_str[" << j << "] : " << quotient_digit.str[SIZE-2] << endl;
    quotient_str[j++] = quotient_digit.str[SIZE - 2];
    //cout << "\ttmp = " << tmp << " - " << multiple << endl;
    tmp = tmp - multiple;
    //cout << "\ttmp = " << tmp << endl;
    tmp = addDigitToRight(tmp, dividend.str[++k]);
    tmp.findDigits();
    //cout << "\t After addDigitToRight(...)   tmp : " << tmp << endl;
    //cout << "\ttmp.findDigits() called ... tmp.no_of_digits : " << tmp.no_of_digits << "\n";
    //cout << "\ttmp : " << tmp << endl;
  }
  quotient_str[j] = '\0';
  q.set_integer(quotient_str);
  q.sign = (a.sign == b.sign) ? '+' : '-';
  return q;
}
  
Integer findNearestMultiple(Integer a, Integer b)
{
  Integer  product = b, one;
  Integer i;
  //cout << "\t\tIn findNearestMultiple(" << a << ", " << b << endl;
  one.set_integer("1");
  i = one;
  while(a >= product) {
    i = i + one;
    //cout << "product : " << b << " *  " << i << endl;
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

Integer squareRoot(Integer a)
{
  //cout << "squareRoot(" << a << ") \n";
  Integer quotient, divisor, multiple, temp, digit, ten;
  divisor.set_integer("0");
  ten.set_integer("10");
  char buffer[80], quotient_str[80];
  int j = 0, k = SIZE - (a.no_of_digits+1);
  if(a.no_of_digits % 2   !=   0) {
    subString(buffer, a.str, k, k+1);
    k++;
  }
  else { 
    subString(buffer, a.str, k, k+2);
    k += 2;
  }
  temp.set_integer(buffer);
  while(k < (SIZE)) {
    //cout << "\ttemp : " << temp << endl;     
    digit = guessRoot(divisor, temp);
    //cout << "\tdigit : " << digit << endl;
    divisor = (divisor * ten);
    divisor = divisor + digit;
    //cout << "\tmultiple = " << divisor << " * " << digit << endl;
    multiple = divisor * digit;
    //cout << "temp = " << temp << " - " << multiple << endl;
    temp = temp - multiple;
    //cout << "quotient_str[" << j << "] = " << digit.str[SIZE-2] << endl;
    quotient_str[j++] = digit.str[SIZE-2];
    temp = addDigitToRight(temp, a.str[k++]);
    temp = addDigitToRight(temp, a.str[k++]);
    divisor = divisor + digit;
  }
  quotient_str[j] = '\0';
  quotient.set_integer(quotient_str);
  return quotient;
}


Integer guessRoot(Integer a, Integer b)
{
  Integer tmp, ten, i, one;
  one.set_integer("1");
  i.set_integer("0");
  ten.set_integer("10");
  while(tmp <= b) {
    i = i + one;
    tmp = a * ten;
    tmp = tmp + i;
    tmp = tmp * i;
  }
  i = i - one;
  return i;
}

Integer CursesDivide(Integer b, Integer a)
{
  Integer divisor, dividend, tmp, multiple, quotient_digit, q;
  char buffer[20], quotient_str[80];
  int j = 0, k, r, c, i, spacing = 0;
  attron(COLOR_PAIR(2));
  rectangle(10, 11, 93, 155, '_');
  move(11, 92);
  addch(251 | A_ALTCHARSET);
  a.Curses_Output(11, 90);
  b.CursesOutput(11, 94);
  divisor = absolute(a);
  dividend = absolute(b);
  if(divisor > dividend) {
    tmp.set_integer("0");
    return tmp;
  }
  k = SIZE - (b.no_of_digits + 1);
  subString(buffer, dividend.str, k, k + a.no_of_digits);
  k = k + a.no_of_digits - 1;
  tmp.set_integer(buffer);
  r = 11, c = 94 ;
  while(k < (SIZE-1)) {
    while(divisor > tmp  &&  k < (SIZE-1)) {
      quotient_str[j++] ='0';
      mvprintw(9, 94 + j, "%c", quotient_str[j - 1]);
      tmp = addDigitToRight(tmp, dividend.str[++k]);
    }
    tmp.CursesOutput(r, c);
    r++;
    if(k == SIZE-1) break;
    quotient_digit = findNearestMultiple(tmp, divisor);
    multiple = quotient_digit * divisor;
    multiple.CursesOutput(r, c);
    r++;
    move(r, c);
    for(i = 0; i < a.no_of_digits; i++)
      printw("-");
    r++;
    quotient_str[j++] = quotient_digit.str[SIZE - 2];
    mvprintw(9, 94 + j, "%c", quotient_str[j - 1]);
    tmp = tmp - multiple;
    spacing = tmp.findDigits();
    tmp = addDigitToRight(tmp, dividend.str[++k]);
    tmp.findDigits();
    c = c + ( tmp.no_of_digits  - (spacing));
  }
  tmp.CursesOutput(r, c);
  mvprintw(r, c+tmp.no_of_digits - 1, " ");
  quotient_str[j] = '\0';
  q.set_integer(quotient_str);
  q.sign = (a.sign == b.sign) ? '+' : '-';
  return q;
}

void curses_Initializations()
{
  initscr();
  clear();
  start_color();
  curs_set(0);
  keypad(stdscr, true);
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  init_pair(3, COLOR_RED, COLOR_WHITE);
  refresh();
}

 
void frame(int start_row, int max_row, int start_col, int max_col)
{
  int row, col;
  for(row = start_row ; row < max_row; row++) {
    if(row == start_row || row == max_row-1) {
      move(row, start_col);
      if(row == start_row)    addch(ACS_ULCORNER);
      else                    addch(ACS_LLCORNER);
      for(col = start_col + 1 ; col < max_col-1; col++) {
	move(row, col);
	addch(ACS_HLINE);
      }
      move(row, max_col-1);
      if(row == start_row)    addch(ACS_URCORNER);
      else                    addch(ACS_LRCORNER);
    }
    else {
      move(row, start_col);
      addch(ACS_VLINE);
      move(row, max_col-1);
      addch(ACS_VLINE);
    }
  }
}




void drawHolder(int row, int col)
{
  move(row, col);
  addch(ACS_ULCORNER);
  move(row, col + 5);
  addch(ACS_URCORNER);
  move(row + 5, col);
  addch(ACS_LLCORNER);
  move(row + 5, col + 5);
  addch(ACS_LRCORNER);
}
  
void drawGrid(int startRow, int startCol)
{
  int r, c;
  r = startRow, c = startCol;
  move(r, c - 10);
  printw("Choose any of the operators to test :");
  r += 5;
  move(r, c);
  printw("+      -      *      /");
  r += 5;
  move(r, c);
  printw("%%     ");
  addch(251 | A_ALTCHARSET);
  printw("       <      >");
  r += 5 ;
  move(r, c);
  printw("<=     >=     <<     >>");
} 

void keypress()
{
   mvprintw(STATUS_ROW + 6, STATUS_COL, "Press any to continue ...");
   refresh();
   getch();
}

int giveMainOption(int MAXROW, int MAXCOL)
{
   int key, IsSelected = 0;
   attron(COLOR_PAIR(2));
   drawGrid(5, 34);
   frame(41, 47, STATUS_COL - 10, STATUS_COL + 75);
   switch(selected) {
   case 10:
     mvprintw(42, STATUS_COL - 8, "Addition operator :                                                           ");
     mvprintw(43, STATUS_COL - 8, "Returns the arithmetic sum of two integers atmost 80 characters long.          ");
     break;
   case 11:
     mvprintw(42, STATUS_COL - 8, "Subtraction operator :                                                         ");
     mvprintw(43, STATUS_COL - 8, "Returns the arithmetic difference of two integers atmost 80 characters long.    ");
     break;
   case 12:
     mvprintw(42, STATUS_COL - 8, "Multiplication operator :                                                     ");
     mvprintw(43, STATUS_COL - 8, "Returns the arithmetic product of two integers atmost 80 characters long.      ");
     break;
   case 13:
     mvprintw(42, STATUS_COL - 8, "Division operator :                                                           ");
     mvprintw(43, STATUS_COL - 8, "Returns the quotient of Division process between two integers.                 ");
     break;
   case 20:
     mvprintw(42, STATUS_COL - 8, "Modulus operator :                                                             ");
     mvprintw(43, STATUS_COL - 8, "Returns the remainder formed due to division between two integers.              ");
     break;
   case 21:
     mvprintw(42, STATUS_COL - 8, "Square Root operator :                                                          ");
     mvprintw(43, STATUS_COL - 8, "Returns the Square root rounded off to nearest  integer.                         ");
     break;
   case 22:
     mvprintw(42, STATUS_COL - 8, "Less than operator :                                                             ");
     mvprintw(43, STATUS_COL - 8, "Returns a boolean value indicating whether a is less than b.                      ");
     break;
   case 23:
     mvprintw(42, STATUS_COL - 8, "Greater than operator :                                                          ");
     mvprintw(43, STATUS_COL - 8, "Returns a boolean value indicating whether a is greater than b.                   ");
     break;
   case 30:
     mvprintw(42, STATUS_COL - 8, "Less than equal to operator :                                                     ");
     mvprintw(43, STATUS_COL - 8, "Returns a boolean value whether a is less than or equal to b.                      ");
     break;
   case 31:
     mvprintw(42, STATUS_COL - 8, "Greater than equal to operator :                                                   ");
     mvprintw(43, STATUS_COL - 8, "Returns a boolean value whether a is greater than or equal to b.                    ");
     break;
   case 32:
     mvprintw(42, STATUS_COL - 8, "Bitwise Left shift operator :                                                      ");
     mvprintw(43, STATUS_COL - 8, "Returns an integer value if its bits are shifted to left.                          ");
     break;
   case 33:
     mvprintw(42, STATUS_COL - 8, "Bitwise Right shift operator :                                                     ");
     mvprintw(43, STATUS_COL - 8, "Returns an integer value if its bits are shifted to right.                         ");
     break;
   }
   attroff(COLOR_PAIR(2));
   attron(COLOR_PAIR(3));
   drawHolder(gridR, gridC);
   attroff(COLOR_PAIR(3));
   refresh();
   key = getch();
   switch(key) {
   case KEY_UP :
     if(gridR == 7) {
        gridR = 17;
        selected += 20;
     }
     else {
        gridR -= 5;
        selected -= 10;
     }
     break;
   
   case KEY_DOWN:
     if(gridR == 17) {
       gridR = 7;
       selected -= 20;
     }
     else {
       gridR += 5;
       selected += 10;
     }
     break;
   
   case KEY_RIGHT:
     if(gridC == 53) {
       gridC = 32;
       selected -= 3;
     }
     else {
       gridC += 7;
       selected += 1;
     }
     break;

   case KEY_LEFT:
     if(gridC == 32) {
       gridC = 53;
       selected += 3;
     }
     else {
       gridC -= 7;
       selected -= 1;
     }
     break;
   
   case 10:
     IsSelected = selected;
     break;
   }
   
   return IsSelected;
}
          

int main()
{
  int option, i, row;
  Integer a, b, res;
  curses_Initializations();
  const int MAXROW = tigetnum("lines");
  const int MAXCOL = tigetnum("cols");
  while(1) {   

    res.setZero(); 
    a.setZero();
    b.setZero();
    attron(COLOR_PAIR(2));
    frame(0, MAXROW, 0, MAXCOL);
    rectangle(1, MAXROW-1, 1, MAXCOL-1, ' ');
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    rectangle(1, 2, 1, MAXCOL-1, ' ');
    attron(A_UNDERLINE | A_BOLD);
    mvprintw(1, (MAXCOL/2)- 10, "Big Numbers Class Demo Program");
    attroff(A_UNDERLINE | COLOR_PAIR(1) | A_BOLD);   
    row = STATUS_ROW;
    switch(option = giveMainOption(MAXROW, MAXCOL)) { 
    case 10: 
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = CursesAdd(a, b);
      mvprintw(row, STATUS_COL, "num1 + num2   : ");
      attroff(COLOR_PAIR(2));
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 

    case 11:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = CursesSubtract(a, b);;
      mvprintw(row, STATUS_COL, "num1 - num2   : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 
      
    case 12: 
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = a > b ? CursesMultiply(b, a) : CursesMultiply(a, b);
      cout << res;
      mvprintw(row, STATUS_COL, "num1 * num2   : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 

    case 13: 
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = CursesDivide(a, b);
      mvprintw(row, STATUS_COL, "num1 / num2   : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 

    case 20: 
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = a % b;
      mvprintw(row, STATUS_COL, "num1 %% num2   : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 

    case 21: 
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      res = squareRoot(a);
      move(row, STATUS_COL);
      addch(251 | A_ALTCHARSET);
      a.CursesOutput(row, STATUS_COL + 1);
      printw(" : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break; 

    case 22:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      i = a < b;
      mvprintw(row, STATUS_COL, "num1 < num2   : ");
      printw("%d", i);
      keypress();
      break; 

    case 23:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      i = a > b;
      mvprintw(row, STATUS_COL, "num1 > num2   : ");
      printw("%d", i);
      keypress();
      break; 
     
    case 30:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      i = (a <= b);
      mvprintw(row, STATUS_COL, "num1 <= num2   : ");
      printw("%d", i);
      keypress();
      break;
    
    case 31:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      i = (a >= b);
      mvprintw(row, STATUS_COL, "num1 >= num2   : ");
      printw("%d", i);
      keypress();
      break;      
     
    case 32:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1    : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2    : ");
      b.CursesInput();
      res = (a << b);
      mvprintw(row, STATUS_COL, "num1 << num2   : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break;

    case 33:
      attron(COLOR_PAIR(2));
      mvprintw(row++, STATUS_COL, "Enter num1 : ");
      a.CursesInput();
      mvprintw(row++, STATUS_COL, "Enter num2 : ");
      b.CursesInput();
      res = (a >> b);
      mvprintw(row, STATUS_COL, "num1 >> num2 : ");
      res.CursesOutput(row, STATUS_COL + 16);
      keypress();
      break;
    }
  } 
  refresh(); 
  getch();
  endwin(); 
  /*
  cin >> a;
  res = squareRoot(a);
  cout << "res : " << res << endl; 
  */
  return 0;
}	
