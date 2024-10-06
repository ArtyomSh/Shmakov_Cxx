#include <iostream>
#include <string>
using namespace std;

string f(void) {
   short int var1 = 1;
   int var2 = 1;
   long int var3 = 1;
   long long int var4 = 1;
   float var5 = 2.0;
   double var6 = 2.0;
   long double var7 = 2.0;
   string var8 = "test";

   return to_string(var5) + to_string(var7) + to_string(var4) + to_string(var1) + to_string(var6) + to_string(var3) + var8 + to_string(var2);
}

