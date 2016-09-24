#include <iostream>
#include <cstdlib>
#include "bimap.h"
using namespace std;
char buf[100];
string to_string(int w) {
	string res;
	int t = 0;
	do {
		buf[t] = w % 10;
		t++;
		w /= 10;
	} while (w > 0);
	for (w = t - 1; w >= 0; w--) {
		res += ('0' + buf[w]);
	}
	return res;
}

bimap c;

void print() {
	bimap::left_iterator it = c.begin_left();
	while (it != c.end_left()) {
		cout << *it << " " << *it.flip() << "  ";
		++it;
	}
	cout << "\n";
	
	
	while (it != c.begin_left()) {
		--it;
		cout << *it << " " << *it.flip() << "  ";
	}
	cout << "\n\n";
	
	
	bimap::right_iterator it2 = c.begin_right();
	
	while (it2 != c.end_right()) {
		cout << *it2 << " " << *it2.flip() << "  ";
		++it2;
	}
	cout << "\n";
	
	
	while (it2 != c.begin_right()) {
		--it2;
		cout << *it2 << " " << *it2.flip() << "  ";
	}
	cout << "\n";
}

int main() {
	
	bimap c10;
	
	int w, e, r;
	
	string c1, c2;
	for (w = 1; w < 10; w++) {
		c1 = ::to_string(w);
		c2 = ::to_string(100 / w);
		c.insert(c1, c2);
	}
	
	bimap::left_iterator it;
	bimap::right_iterator it2;
	
	print();
	
	it = c.find_left("4");
	
	cout << *it << " " << *it.flip() << "\n";
	
	c.erase(it);
	
	for (w = 0; w < 1000; w++) {
		it = c.find_left("-1");
	}
	
	for (w = 0; w < 100; w++) {
		bimap::left_iterator  it1 =  c.find_left(to_string(rand() % 15));
		bimap::right_iterator it2 = c.find_right(to_string(rand() % 15));
	}
	
	
	print();
    
    return 0;
}
