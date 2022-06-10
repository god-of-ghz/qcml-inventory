#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
using namespace std;

void calc_spaces(string target, int num);

class Materials{
	string code;
	string name;
	string manufac;
	string date;
	string owner;
	string size;
	string lot;
	string weight;
	string open;
	string ID;
public:
	Materials() { ; }
	Materials(string);
	Materials(string, string);
	Materials(string, string, string, string, string, string, string, string, string, string);

	//const Materials& get_me() { return *this; }

	void print_me() const;
	void print_table() const;

	Materials(string, const Materials&);

	void operator= (const Materials&);

	void set_info(string, string, string, string, string, string, string, string, string, string);
	void clear();

	string get_code() const { return code; }
	string get_name() const { return name; }
	string get_man() const { return manufac; }
	string get_date() const { return date; }
	string get_own() const { return owner; }
	string get_size() const { return size; }
	string get_lot() const { return lot; }
	string get_weight() const { return weight; }
	string get_open() const { return open; }
	string open_word() const;
	bool isopen() const;
	string get_ID() const { return ID; }

	void set_code(string a) { code = a; }
	void set_name(string a) { name = a; }
	void set_man(string a) { manufac = a; }
	void set_date(string a) { date = a; }
	void set_own(string a) { owner = a; }
	void set_size(string a) { size = a; }
	void set_lot(string a) { lot = a; }
	void set_weight(string a) { weight = a; }
	void set_open(string a) { open = a; }
	void set_ID(string a) { ID = a; }

};

void Materials::set_info(string a, string b, string c, string d, string e, string f, string g, string i, string j, string k) {
	code = a; name = b; manufac = c; date = d; owner = e; size = f, lot = g, weight = i, open = j, ID = k;
}

Materials::Materials(string a, const Materials& target) {
	set_info(a, target.get_name(), target.get_man(), target.get_date(), target.get_own(), target.get_size(), target.get_lot(), target.get_weight(), target.get_open(), target.get_ID());
}

Materials::Materials(string a) {
	clear();
	code = a;
}

Materials::Materials(string a, string b) {
	clear();
	code = a;
	name = b;
}

Materials::Materials(string a, string b, string c, string d, string e, string f, string g, string i, string j, string k) {
	set_info(a, b, c, d, e, f, g, i, j, k);
}

void Materials::clear() {
	code.clear();
	name.clear();
	manufac.clear();
	date.clear();
	owner.clear();
	size.clear();
	lot.clear();
	weight.clear();
	open = "no";
	ID.clear();
}

void Materials::operator= (const Materials& target) {
	set_info(code, target.get_name(), target.get_man(), target.get_date(), target.get_own(), target.get_size(), target.get_lot(), target.get_weight(), target.get_open(), target.get_ID());
}

void Materials::print_me() const {
	cout << "The code for this material is " << get_code() << endl;
	cout << "The name of this material is " << get_name() << endl;
	cout << "The manufacturer of this material is " << get_man() << endl;
	cout << "The material was received on " << get_date() << endl;
	cout << "The material's owner is " << get_own() << endl;
	cout << "The material is " << get_size() << " microns in size.\n";
	cout << "This material is from lot " << get_lot() << endl;
	cout << "This package has " << get_weight() << " lbs left.\n";
	cout << "Has this material been opened? " << open_word() << endl;
	cout << "The material's ID is " << get_ID() << endl;
}

void Materials::print_table() const {
	cout << get_code();
	calc_spaces(get_code(), 10);
	cout << get_name();
	calc_spaces(get_name(), 20);
	cout << get_man();
	calc_spaces(get_man(), 15);
	cout << get_date();
	calc_spaces(get_date(), 15);
	cout << get_own();
	calc_spaces(get_own(), 20);
	cout << get_size();
	calc_spaces(get_size(), 10);
	cout << get_lot();
	calc_spaces(get_lot(), 10);
	cout << get_weight();
	calc_spaces(get_weight(), 10);
	cout << get_open();
	calc_spaces(get_open(), 5);
	cout << get_ID();
	calc_spaces(get_ID(), 10);
	cout << endl;
}

void calc_spaces(string target, int num) {
	for (int i = target.size(); i <= num; i++) {
		cout << " ";
	}
}


bool Materials::isopen() const {
	if (open == "1")
		return true;
	else
		return false;
}

string Materials::open_word() const {
	if (open == "1")
		return "yes";
	else
		return "no";
}