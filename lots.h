#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
using namespace std;

#include <jobs.h>

int process_date1(string target);		//converts a string date of the form MMDDYYYY to an integer that can be numerically compared
string process_date2(int target);		//converts that integer used for comparison back to its original form			

class Lots {
	string lotID;
	vector <int> index_list;
	vector <string> dates;
	vector <double> weights;

	string name;
	string man;
	string own;
	string size;

public:
	Lots() { return; }
	Lots(Materials target);
	Lots(Materials target, vector <Materials> &thelist);

	void build(Materials target, vector <Materials> &thelist);
	void rebuild(vector <Materials> &thelist);
	void print();
	void write(ofstream &writefile);

	void operator= (const Lots&);

	void set_ID(string a) { lotID = a; }
	void set_name(string a) { name = a; }
	void set_man(string a) { man = a; }
	void set_own(string a) { own = a; }
	void set_size(string a) { size = a; }

	void build_list(Materials target, vector <Materials> &thelist);
	void build_list(vector <Materials> &thelist);
	void build_dates(vector <Materials> &thelist);
	void sort_dates();
	void build_weights(vector <Materials> &thelist);				//assumes dates are properly sorted in order
	

	string get_lotID() const { return lotID; }
	string get_name() const { return name; }
	string get_man() const { return man; }
	string get_own() const { return own; }
	string get_size() const { return size; }

	vector <int> get_indices() const { return index_list; }
	vector <string> get_dates() const { return dates; }
	vector <double> get_weights() const { return weights; }

	bool is_used(string date);
	bool is_used(Materials target, vector <Materials> &thelist);
	
	void clear();
	void clear_all();


};

Lots::Lots(Materials target) {
	lotID = target.get_lot();
	name = target.get_name();
	man = target.get_man();
	own = target.get_own();
	size = target.get_size();
}

Lots::Lots(Materials target, vector <Materials> &thelist) {
	lotID = target.get_lot();
	name = target.get_name();
	man = target.get_man();
	own = target.get_own();
	size = target.get_size();
	rebuild(thelist);
}

void Lots::build(Materials target, vector <Materials> &thelist) {
	lotID = target.get_lot();
	name = target.get_name();
	man = target.get_man();
	own = target.get_own();
	size = target.get_size();
	rebuild(thelist);
}

void Lots::rebuild(vector <Materials> &thelist) {
	clear();
	build_list(thelist);
	build_dates(thelist);
	build_weights(thelist);
}

void Lots::operator= (const Lots &target) {
	clear_all();
	
	set_ID(target.get_lotID());
	set_name(target.get_name());
	set_man(target.get_man());
	set_own(target.get_own());
	set_size(target.get_size());
	index_list = target.get_indices();
	dates = target.get_dates();
	weights = target.get_weights();
}

void Lots::build_list(vector <Materials> &thelist) {
	for (int i = 0; i < thelist.size(); i++) {
		if (thelist[i].get_lot() == lotID)
			index_list.push_back(i);
	}
}

void Lots::build_list(Materials target, vector <Materials> &thelist) {
	string lotname = target.get_lot();
	for (int i = 0; i < thelist.size(); i++) {
		if (thelist[i].get_lot() == lotname)
			index_list.push_back(i);
	}
}

void Lots::build_dates(vector <Materials> &thelist) {
	for (int i = 0; i < index_list.size(); i++) {
		for (int j = 0; j < thelist[index_list[i]].h_size(); j++) {
			if (!is_used(thelist[index_list[i]].get_dhist(j)))
				dates.push_back(thelist[index_list[i]].get_dhist(j));
		}
	}
	sort_dates();
}

void Lots::sort_dates() {
	vector <int> temp;
	for (int i = 0; i < dates.size(); i++) {
		temp.push_back(process_date1(dates[i]));
	}
	sort(temp.begin(), temp.end());
	dates.clear();
	for (int i = 0; i < temp.size(); i++) {
		dates.push_back(process_date2(temp[i]));
	}
}

int process_date1(string target) {
	string temp = target;
	for (int i = 0; i < 4; i++) {
		temp[i] = temp[i + 4];
	}
	for (int i = 0; i < 4; i++) {
		temp[i + 4] = target[i];
	}
	return stoi(temp);
}

string process_date2(int target) {
	string temp = to_string(target);
	string result = temp;
	for (int i = 0; i < 4; i++) {
		result[i] = result[i + 4];
	}
	for (int i = 0; i < 4; i++) {
		result[i + 4] = temp[i];
	}
	return result;
}

void Lots::build_weights(vector <Materials> &thelist) {
	string current_date;
	int date_index;
	double runsum;

	for (int i = 0; i < dates.size(); i++) {
		date_index = i;
		current_date = dates[i];
		runsum = 0;
		for (int j = 0; j < index_list.size(); j++) {
			/*if (thelist[index_list[j]].h_size() > date_index) {
				if (thelist[index_list[j]].get_dhist(date_index) == current_date) {
					runsum = runsum + stod(thelist[index_list[j]].get_whist(date_index));
				}
				else {
					for (int k = 0; k <= date_index; k++) {
						if (process_date1(thelist[index_list[j]].get_dhist(date_index - k)) < process_date1(current_date)) {
							runsum = runsum + stod(thelist[index_list[j]].get_whist(date_index - k));
							break;
						}
					}
				}
			}*/
			for (int k = thelist[index_list[j]].h_size() - 1; k >= 0; k--) {
				if (process_date1(thelist[index_list[j]].get_dhist(k)) <= process_date1(current_date)) {
					runsum = runsum + stod(thelist[index_list[j]].get_whist(k));
					break;
				}
			}

		}
		weights.push_back(runsum);
	}
}

void Lots::print() {
	string temp;
	cout << endl;
	cout << "LOT        MATERIAL             MANUFACTURER    OWNER                SIZE(um)   \n";
	cout << "--------------------------------------------------------------------------------\n";
	cout << get_lotID();
	calc_spaces(get_lotID(), 10);
	cout << get_name();
	calc_spaces(get_name(), 20);
	cout << get_man();
	calc_spaces(get_man(), 15);
	cout << get_own();
	calc_spaces(get_own(), 20);
	cout << get_size();
	calc_spaces(get_size(), 10);
	cout << endl << endl;
	cout << "WEIGHT(kg) DATE           \n";
	cout << "--------------------------\n";
	for (int i = 0; i < dates.size(); i++) {
		cout << to_string(weights[i]);
		calc_spaces(to_string(weights[i]), 10);
		cout << dates[i];
		calc_spaces(dates[i], 15);
		cout << endl;
	}
	cout << endl;
}

void Lots::write(ofstream &writefile) {
	string temp;
	writefile << "********************************************************************************\n";
	writefile << "LOT        MATERIAL             MANUFACTURER    OWNER                SIZE(um)   \n";
	writefile << "--------------------------------------------------------------------------------\n";
	writefile << get_lotID();
	writefile << return_spaces(get_lotID(), 10);
	writefile << get_name();
	writefile << return_spaces(get_name(), 20);
	writefile << get_man();
	writefile << return_spaces(get_man(), 15);
	writefile << get_own();
	writefile << return_spaces(get_own(), 20);
	writefile << get_size();
	writefile << return_spaces(get_size(), 10);
	writefile << endl << endl;
	writefile << "WEIGHT(kg) DATE           \n";
	writefile << "--------------------------\n";
	for (int i = 0; i < dates.size(); i++) {
		writefile << to_string(weights[i]);
		writefile << return_spaces(to_string(weights[i]), 10);
		writefile << dates[i];
		writefile << return_spaces(dates[i], 15);
		writefile << endl;
	}
	writefile << endl;
}

bool Lots::is_used(string date) {
	for (int i = 0; i < dates.size(); i++) {
		if (dates[i] == date)
			return true;
	}
	return false;
}

bool Lots::is_used(Materials target, vector <Materials> &thelist) {
	int index = -1;
	for (int i = 0; i < thelist.size(); i++) {
		if (target.get_code() == thelist[i].get_code())
			index = i;
	}
	for (int i = 0; i < index_list.size(); i++) {
		if (index_list[i] == index)
			return true;
	}
	return false;
}

void Lots::clear() {
	index_list.clear();
	dates.clear();
	weights.clear();
}

void Lots::clear_all() {
	lotID.clear();
	name.clear();
	man.clear();
	own.clear();
	size.clear();

	clear();
}
