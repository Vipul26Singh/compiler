#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

int main() {
    using namespace std;
    	string sentence = "And I feel fine...";
    	istringstream iss(sentence);
	vector<string> tokens;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
	
	for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    		std::cout << ' ' << *it;
  	std::cout << '\n';
}
