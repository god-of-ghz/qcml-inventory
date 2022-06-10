#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
using namespace std;

#include <materials.h>

class Jobs {
	string ID;
	string start_date;
	string end_date;
	vector <string> materials;
	vector <string> end_materials;
	double weight_in;
	double weight_out;
	bool finished;

public:
	Jobs() { weight_in = 0; weight_out = 0; finished = false; }
	Jobs(string);
	Jobs(string, vector <string>);
	Jobs(string, string, string, vector <string>);

	void set_ID(string a) { ID = a; }
	void set_sdate(string a) { start_date = a; }
	void set_edate(string a) { end_date = a; }
	void add_material(string a) { materials.push_back(a); }
	void add_ematerial(string a) { end_materials.push_back(a); }
	void add_sweight(string a);
	void add_eweight(string a);

	string get_ID() const { return ID; }
	string get_sdate() const { return start_date; }
	string get_edate() const { return end_date; }
	vector <string> get_mats() const { return materials; }
	vector <string> get_emats() const { return end_materials; }
	double get_sweight() const { return weight_in; }
	double get_eweight() const { return weight_out; }

	void start(string a) { start_date = a; }
	void finish() { finished = true; }
	void operator+ (const string& a) { materials.push_back(a); }
	void operator= (const Jobs& a);

	void clear();

	bool is_used(string a);
	bool is_end_used(string a);
	bool is_finished() const { return finished; }

	void print();

};

void Jobs::add_sweight(string a) {
	weight_in += + stod(a);
}

void Jobs::add_eweight(string a) {
	weight_out += stod(a);
}

void Jobs::operator= (const Jobs& a) {
	ID = a.get_ID();
	start_date = a.get_sdate();
	end_date = a.get_edate();
	materials = a.get_mats();
	end_materials = a.get_emats();
	weight_in = a.get_sweight();
	weight_out = a.get_eweight();
	finished = a.is_finished();
}

void Jobs::clear() {
	ID.clear();
	start_date.clear();
	end_date.clear();
	materials.clear();
	end_materials.clear();
	weight_in = 0;
	weight_out = 0;
	finished = false;
}

bool Jobs::is_used(string a) {
	for (int i = 0; i < materials.size(); i++) {
		if (a == materials[i])
			return true;
	}
	return false;
}

bool Jobs::is_end_used(string a) {
	for (int i = 0; i < end_materials.size(); i++) {
		if (a == end_materials[i])
			return true;
	}
	return false;
}

void Jobs::print() {
	string temp;
	cout << ID;
	calc_spaces(ID, 10);
	cout << start_date;
	calc_spaces(start_date, 10);
	cout << end_date;
	calc_spaces(end_date, 10);
	cout << weight_in;
	temp = to_string(weight_in);
	calc_spaces(temp, 20);
	cout << weight_out;
	temp = to_string(weight_out);
	calc_spaces(temp, 20);
	cout << finished << endl;
}