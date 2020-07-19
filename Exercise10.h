#pragma once
#include <string>
#include <vector>
using namespace std;

void radixsort(vector<string> &words, int len);

void radixsort(vector<string> &words, int len) {
	char power = 'a';
	vector<vector<string>> buckets(26);
	int size = words.size();
	for (int i =len-1 ;i>=0 ;i--) {
		int a = 0;
		//distribute into different buckets
		for (int j = 0;j < size;++j) {
			string temp = words[j];
			buckets[temp[i]%power].push_back(words[j]);

		}
		//order by buckets
		for (int k = 0;k < 26;++k) {
			while (!buckets[k].empty()) {
				int bucketSize = buckets[k].size();
				vector<string> bucket = buckets[k];
				for (int j = 0;j<bucketSize; j++) {
					words[a] = bucket[j];
					a++;
				}
				buckets[k].clear();
				k++;
			
			}
		}
	}

}
