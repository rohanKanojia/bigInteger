#include <iostream>
#include "Integer.hxx"
using namespace std;

int main() {
  string s1, s2;
  cin >> s1 >> s2;
  Integer a1(s1), a2(s2), res;


  cout << "a1 : " << a1 << endl;
  cout << "a2 : " << a2 << endl;

  res = a1 + a2;
  cout << a1 << " + " << a2 << " : " << res  << endl;
  res = a1 - a2;
  cout << a1 << " - " << a2 << " : " << res << endl;
  res = a1 * a2;
  cout << a1 << " * " << a2 << " : " << res << endl;
  res = a1 / a2;
  cout << a1 << " / " << a2 << " : " << res << endl;

  return 0;
}

