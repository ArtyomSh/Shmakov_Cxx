#include "NumberToWords.hpp"
#include <string>
#include <vector>

using namespace std;

string convertToWordsRec(int n, vector<int> &values, vector<string> &words) {
  string res = "";

  for (int i = 0; i < values.size(); i++) {
    int value = values[i];
    string word = words[i];

    if (n >= value) {
      if (n >= 100)
        res += convertToWordsRec(n / value, values, words) + " ";

      res += word;

      if (n % value > 0)
        res += " " + convertToWordsRec(n % value, values, words);

      return res;
    }
  }

  return res;
}

string convertToWords(int n) {
  if (n == 0)
    return "Zero";

  vector<int> values = {1000000000, 1000000, 1000, 100, 90, 80, 70, 60,
                        50,         40,      30,   20,  19, 18, 17, 16,
                        15,         14,      13,   12,  11, 10, 9,  8,
                        7,          6,       5,    4,   3,  2,  1};

  vector<string> words = {
      "Billion",  "Million",  "Thousand",  "Hundred", "Ninety",  "Eighty",
      "Seventy",  "Sixty",    "Fifty",     "Forty",   "Thirty",  "Twenty",
      "Nineteen", "Eighteen", "Seventeen", "Sixteen", "Fifteen", "Fourteen",
      "Thirteen", "Twelve",   "Eleven",    "Ten",     "Nine",    "Eight",
      "Seven",    "Six",      "Five",      "Four",    "Three",   "Two",
      "One"};

  return convertToWordsRec(n, values, words);
}
