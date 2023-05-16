#include "head.h"

int main(int argv, char* argc[]) {
	string str;
	cin >> str;
	int len = str.length();
	int x;
	int i, j;
	int num, max;
	for(x = 0; x < len; ++x) {
		num = 0;
		i = x;
		j = x + 1;
		while(str[x] == str[j] && i >= 0 && j < len) {
			--i;
			++j;
			num += 2;
		}
		max = max > num ? max : num;
	}
	for(x = 1; x < len; ++x) {
		num = 1;
		i = x - 1;
		j = x + 1;
		while(str[x] == str[j] && i >= 0 && j < len) {
			--i;
			++j;
			num += 2;
		}
		max = max > num ? max : num;
	}
	cout << str << "'s substr max length: " << max << endl;
	return 0;
}