#include <string>
#include <iostream>
#include <vector>
#include <limits>


using namespace std;

struct hasValue {
	string value;
	bool empty = true;
};


class CuckooTable {
public:
	double maxFill;
	double resizeFactor;
	int maxLoops;

	vector<hasValue> T1;
	vector<hasValue> T2;

	CuckooTable(size_t tableSize, double mf, double rf, int ml) {
		T1.resize(tableSize);
		T2.resize(tableSize + 1);
		maxFill = mf;
		resizeFactor = rf;
		maxLoops = ml;
	}

	int hashFunction1(string key) {
		return hash<string>{}(key) % T1.size();
	}

	int hashFunction2(string key) {
		return hash<string>{}(key) % T2.size();
	}

	int NumberOfItems() {
		int result = 0;
		for (size_t i = 0; i < T1.size();i++) {
			if (!T1[i].empty) {
				result++;
			}
		}
		for (size_t i = 0; i < T2.size();i++) {
			if (!T2[i].empty) {
				result++;
			}
		}
		return result;
	}

	bool Lookup(string key) {
		
		int index = hashFunction1(key);
		if (T1[index].value == key)
		{
			return true;
		}
		index = hashFunction2(key);
		if (T2[index].value == key)
		{
			return true;
		}
		return false;
		

		/*
		int index;
		string temp = key;
		string temp2;

		for (int i = 0; i < maxLoops; i++) {
			index = hashFunction1(temp);
			if (T1[index].value == key) {
				return true;
			}

			if (!T1[index].empty) {
				temp = T1[index].value;
			}

			index = hashFunction2(temp);
			if (T2[index].value == key) {
				return true;
			}
			if (!T2[index].empty) {
				temp = T2[index].value;
			}
		}
		*/

		/*
		for (int i = 0; i < T1.size(); i++) {
			if (T1[i].value == key) {
				return true;
			}
		}
		for (int i = 0; i < T2.size(); i++) {
			if (T2[i].value == key) {
				return true;
			}
		}
		*/

		return false;
	}

	void AllEmpty() {
		for (size_t i = 0; i < T1.size(); i++) {
			T1[i].empty = true;
		}
		for (size_t i = 0; i < T2.size(); i++) {
			T2[i].empty = true;
		}
	}

	void Resize() {

		vector<hasValue> temp = T1;
		vector<hasValue> temp2 = T2;
		/*
		T1.resize(T1.size() * resizeFactor);
		T2.resize(T2.size() * resizeFactor);
		AllEmpty();
		*/
		vector<hasValue> new1;
		new1.resize(T1.size() * resizeFactor);
		T1 = new1;
		vector<hasValue> new2;
		new2.resize(T2.size() * resizeFactor);
		T2 = new2;
		
		for (size_t i = 0; i < temp.size(); i++) {
			if (!temp[i].empty) {
				Insert(temp[i].value);
			}
		}
		for (size_t i = 0; i < temp2.size(); i++) {
			if (!temp2[i].empty) {
				Insert(temp2[i].value);
			}
		}
	}

	void Insert(string key) {

		if (NumberOfItems() / static_cast<double>(T1.size() + T2.size()) > maxFill) {
			Resize();
		}

		if (Lookup(key)) {
			return;
		}

		int index;
		string temp;
		for (int i = 0; i < maxLoops; i++) {
			index = hashFunction1(key);
			if (T1[index].empty) {
				T1[index].value = key;
				T1[index].empty = false;
				return;
			}

			temp = T1[index].value;
			T1[index].value = key;
			key = temp;

			index = hashFunction2(key);
			if (T2[index].empty) {
				T2[index].value = key;
				T2[index].empty = false;
				return;
			}

			temp = T2[index].value;
			T2[index].value = key;
			key = temp;
		}

		Resize();
		Insert(key);

	}

	void Print() {
		cout << "Table1:" << endl;
		for (size_t i = 0; i < T1.size(); i++) {
			if (!T1[i].empty) {
				cout << i << ": " << T1[i].value << " ";
			}
		}
		cout << endl;
		cout << "Table2:" << endl;
		for (size_t i = 0; i < T2.size(); i++) {
			if (!T2[i].empty) {
				cout << i << ": " << T2[i].value << " ";
			}
		}
		cout << endl;
	}

};


int main() {


	CuckooTable cuckoo(10, 0.5, 1.5, 5);

	vector<string> inputs = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	vector<string> inputs2 = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
"seventeen", "eighteen", "nineteen", "twenty",
"twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five",
"twenty-six", "twenty-seven", "twenty-eight", "twenty-nine", };

	for (size_t i = 0; i < inputs.size(); i++) {
		cuckoo.Insert(inputs[i]);
	}

	cuckoo.Print();
	
	for (size_t i = 0; i < inputs2.size(); i++) {
		cuckoo.Insert(inputs2[i]);
	}
	
	cout << endl;
	cuckoo.Print();

	return 0;
}