#include "summertime.hpp"
#include <iostream>

using namespace std;

int main() {
	string content;
	try {
		content = fetchNotificationContent();
	} catch (const char *msg) {
		cerr << msg << endl;
		return 1;
	}

	cout << content << endl;

    return 0;
}
