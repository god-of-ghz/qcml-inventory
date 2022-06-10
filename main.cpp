#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

//#include <materials.h>
#include <lots.h>
#include <time.h>

//reading and writing functions
vector <Materials> file_to_vec(ifstream& reading);
vector <Jobs> file_to_jobs(vector <Materials>& thelist);
void read_history(vector <Materials>& thelist);

void write_to_file(vector <Materials>& thelist, string filename);
void write_jobs(vector <Jobs>& joblist);
void write_history(vector <Materials>& thelist);
void write_ireport(vector <Materials>& thelist);
void write_hreport(vector <Materials>& thelist);
void write_as_report(vector <Materials>& targets);

//command center functions
void cc_search(vector <Materials>& thelist);
void cc_find(vector <Materials>& thelist, vector <int>& recent);
void cc_edit(vector <Materials>& thelist, vector <int>& recent);
void cc_add(vector <Materials>& thelist, vector <int>& recent);
void cc_save(vector <Materials>& thelist, vector <Jobs>& joblist);
void cc_print(vector <Materials>& thelist);
void cc_history(vector <Materials>& thelist);
void cc_report(vector <Materials>& thelist);
void cc_recent(vector <Materials>& thelist, vector <int>& recent);
void cc_job(vector <Materials>& thelist, vector <int>& recent, vector <Jobs>& joblist);
void cc_lots(vector <Materials>& thelist);
void cc_usage(vector <Materials>& thelist);
void cc_help();
void cc_about();
void cc_experimental();

//helper functions
Materials process_material(string * info_set);
Materials * search_material(vector <Materials>& thelist, string a);
int find_index(vector <Materials>& thelist, Materials * target);
int find_index(vector <Materials>& thelist, string code);
int find_index(vector <Jobs>& joblist, string ID);

const string currentDate();

void prep_screen(string menu);

void check_recent(vector <int>& recent);
void add_recent(vector <int>& recent, int index);

bool is_valid_date(string a);
bool is_number(string a);
bool partial_match(string a, string b);		//check if b can be found as a portion of a

void command_center(vector <Materials>& thelist, vector <Jobs>& joblist) {
	string command;
	string temp;
	vector <int> recent;
	while (1) {
		check_recent(recent);
		prep_screen("main");
		cout << "\nChoose from one of the following commands.\n";
		cout << "1. search\n2. find\n3. edit\n4. help\n5. save\n6. print\n7. history\n8. recent\n9. add\n10. report\n11. jobs\n12. lot history\n13. monthly usage\n14. exit\n15. about\n\n";
		cin >> command;
		temp.clear();
		if (command == "lookup" || command == "search" || command == "1") {
			cc_search(thelist);
		}
		else if (command == "find" || command == "2") {
			cc_find(thelist, recent);
		}
		else if (command == "edit" || command == "3") {
			cc_edit(thelist, recent);
		}
		else if (command == "help" || command == "4") {
			cc_help();
		}
		else if (command == "save" || command == "5") {
			cc_save(thelist, joblist);
		}
		else if (command == "print" || command == "list" || command == "6") {
			cc_print(thelist);
		}
		else if (command == "history" || command == "7") {
			cc_history(thelist);
		}
		else if (command == "recent" || command == "8") {
			cc_recent(thelist, recent);
		}
		else if (command == "add" || command == "9") {
			cc_add(thelist, recent);
		}
		else if (command == "reports" || command == "report" || command == "10") {
			cc_report(thelist);
		}
		else if (command == "jobs" || command == "job" || command == "11") {
			cc_job(thelist, recent, joblist);
		}
		else if (command == "lot" || command == "lots" || command == "12") {
			cc_lots(thelist);
		}
		else if (command == "usage" || command == "monthly" || command == "13") {
			cc_usage(thelist);
		}
		else if (command == "about" || command == "15") {
			cc_about();
		}
		else if (command == "beta") {
			cc_experimental();
		}
		else if (command == "exit" || command == "back" || command == "14") {
			prep_screen("exiting");
			cout << "Are you sure you want to exit the program?\n";
			cin >> temp;
			if (temp == "yes" || temp == "y")
				break;
		}
		else if (command == "fix_all") {
			int sum = 0;
			for (int i = 0; i < thelist.size(); i++) {
				if (thelist[i].get_hist().size() > 1) {
					if (!stoi(thelist[i].get_open()))
						sum++;
					thelist[i].set_open("1");
					
				}
			}
			cout << sum << " entries changed.\n";
			system("pause");
		}
		else {
			prep_screen("main");
			cout << "That command is not recognized, please try again. Type 'help' if you need assistance.\n";
			cout << "\nHit any key to leave this menu.\n";
			cin >> temp;
		}
	}
	return;
}

void main() {
	string temp;
	string filename = "data.qcml";
	ifstream readfile;
	prep_screen("main");
	readfile.open(filename);
	while (!readfile.is_open()) {
		cout << "There was a problem loading the default file for the database. Please enter the name of the file.\n\n";
		cin >> filename;
		readfile.open(filename);
		if (readfile.is_open()) {
			cout << "Read was successful.\n\n";
		}
		else
			cout << "Read failed. Please try again. \n";
	}
	cout << "\nLoading, please wait...\n";
	vector <Materials> thelist = file_to_vec(readfile);
	readfile.close();
	read_history(thelist);
	vector <Jobs> joblist = file_to_jobs(thelist);
	cout << "Database loaded!.\n";
	

	command_center(thelist, joblist);

	prep_screen("exiting");
	write_to_file(thelist, filename);
	write_history(thelist);
	write_jobs(joblist);
	
}

vector <Materials> file_to_vec(ifstream& reading) {
	string * info_set = nullptr;
	string temp;
	vector <Materials> thelist;
	while (!reading.eof()) {
		info_set = new string [10];
		for (int i = 0; i < 10; i++) {
			reading >> temp;
			info_set[i] = temp;
		}
		if (reading.eof())
			break;
		thelist.push_back(process_material(info_set));
		delete[] info_set;
	}
	return thelist;
}

Materials process_material(string * info_set) {
	Materials temp_mat;
	temp_mat.set_code(info_set[0]);
	temp_mat.set_name(info_set[1]);
	temp_mat.set_man(info_set[2]);
	temp_mat.set_date(info_set[3]);
	temp_mat.set_own(info_set[4]);
	temp_mat.set_size(info_set[5]);
	temp_mat.set_lot(info_set[6]);
	temp_mat.set_weight(info_set[7]);
	temp_mat.set_open(info_set[8]);
	temp_mat.set_ID_ONLY(info_set[9]);
	return temp_mat;
}

Materials * search_material(vector <Materials>& thelist, string a) {
	Materials * target = nullptr;
	for (int i = 0; i < thelist.size(); i++) {
		if (thelist[i].get_code() == a)
			return target = &thelist[i];
	}
	return target;
}

void write_to_file(vector <Materials>& thelist, string filename) {
	ofstream writefile(filename);
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	string buildline;
	cout << "Writing the database! DO NOT CLOSE THE PROGRAM!\n";
	for (int i = 0; i < thelist.size(); i++) {
		buildline.append(thelist[i].get_code());
		buildline.append(return_spaces(thelist[i].get_code(), 10));
		buildline.append(thelist[i].get_name());
		buildline.append(return_spaces(thelist[i].get_name(), 20));
		buildline.append(thelist[i].get_man());
		buildline.append(return_spaces(thelist[i].get_man(), 15));
		buildline.append(thelist[i].get_date());
		buildline.append(return_spaces(thelist[i].get_date(), 15));
		buildline.append(thelist[i].get_own());
		buildline.append(return_spaces(thelist[i].get_own(), 20));
		buildline.append(thelist[i].get_size());
		buildline.append(return_spaces(thelist[i].get_size(), 10));
		buildline.append(thelist[i].get_lot());
		buildline.append(return_spaces(thelist[i].get_lot(), 10));
		buildline.append(thelist[i].get_weight());
		buildline.append(return_spaces(thelist[i].get_weight(), 10));
		buildline.append(thelist[i].get_open());
		buildline.append(return_spaces(thelist[i].get_open(), 5));
		buildline.append(thelist[i].get_ID());
		writefile << buildline << endl;
		buildline.clear();
	}
	writefile.close();
}

void prep_screen(string menu) {
	system("cls");
	cout << "+---------------------------------------+\n";
	cout << "|       QCML INVENTORY SYSTEM V1.8.4    |\n";
	cout << "+---------------------------------------+\n\n";

	if (menu == "main")
		cout << "~~~~~~~~~~~~~~~~MAIN MENU~~~~~~~~~~~~~~~\n\n";
	else if (menu == "searching")
		cout << "~~~~~~~~~~~~SEARCH MODE ENABLED~~~~~~~~~\n\n";
	else if (menu == "exiting")
		cout << "~~~~~~~~~~~~~~EXIT PROGRAM?~~~~~~~~~~~~~\n\n";
	else if (menu == "saving")
		cout << "~~~~~~~~~~~~~~SAVE CHANGES?~~~~~~~~~~~~~\n\n";
	else if (menu == "helping")
		cout << "~~~~~~~~~~~~~~~~HELP MENU~~~~~~~~~~~~~~~\n\n";
	else if (menu == "listing")
		cout << "~~~~~~~~~~~~~~INVENTORY LIST~~~~~~~~~~~~\n\n";
	else if (menu == "recent")
		cout << "~~~~~~~~~~~~~RECENTLY EDITED~~~~~~~~~~~~\n\n";
	else if (menu == "editing")
		cout << "~~~~~~~~~~~~~EDIT MODE ENABLED~~~~~~~~~~\n\n";
	else if (menu == "adding")
		cout << "~~~~~~~~~~~~NEW MATERIAL ENTRY~~~~~~~~~~\n\n";
	else if (menu == "about")
		cout << "~~~~~~~~~~~~~~~~~~ABOUT~~~~~~~~~~~~~~~~~\n\n";
	else if (menu == "history")
		cout << "~~~~~~~~~~~~~~~~~HISTORY~~~~~~~~~~~~~~~~\n\n";
	else if (menu == "report")
		cout << "~~~~~~~~~~~~~~~~~REPORTS~~~~~~~~~~~~~~~~\n\n";
	else if (menu == "jobs")
		cout << "~~~~~~~~~~~~~~~~JOB MENU~~~~~~~~~~~~~~~~\n\n";
	else if (menu == "lots")
		cout << "~~~~~~~~~~~~~~~LOT HISTORY~~~~~~~~~~~~~~\n\n";
	else if (menu == "usage")
		cout << "~~~~~~~~~~~~~~MONTHLY USAGE~~~~~~~~~~~~~\n\n";
	else if (menu == "beta")
		cout << "~~~~~~~~~~~~~~~CLASSIFIED~~~~~~~~~~~~~~~\n\n";
	else
		cout << "~~~~~~~~~~~~~~~COMING SOON!~~~~~~~~~~~~~\n\n";

	if (menu == "listing" || menu == "recent" || menu == "searching") {
		cout << "BARCODE    MATERIAL             MANUFACTURER    DATE            OWNER                SIZE(um)   LOT        WEIGHT(kg) OPEN  ID        \n";
		cout << "--------------------------------------------------------------------------------------------------------------------------------------\n";
	}
	else if (menu == "history") {
		cout << "Enter the barcode of the material whose history you would like or type 'back.\n\n";
		cout << "BARCODE    WEIGHT(kg) DATE           \n";
		cout << "-------------------------------------\n";
	}

	return;
}

int find_index(vector <Materials>& thelist, Materials * target) {
	int index = 0;
	for (int i = 0; i < thelist.size(); i++) {
		if (&thelist[i] == target) {
			index = i;
			return index;
		}
	}
	return -1;
}

int find_index(vector <Materials>& thelist, string a) {
	for (int i = 0; i < thelist.size(); i++) {
		if (thelist[i].get_code() == a)
			return i;
	}
	return -1;
}

int find_index(vector <Jobs>& joblist, string ID) {
	for (int i = 0; i < joblist.size(); i++) {
		if (joblist[i].get_ID() == ID)
			return i;
	}
	return -1;
}

void check_recent(vector <int>& recent) {
	if (recent.size() > 500)
		recent.clear();
}

void add_recent(vector <int>& recent, int index) {
	recent.push_back(index);
}

bool is_valid_date(string a) {
	string num = "0123456789";
	string month; month.append(a, 0, 2);
	string day; day.append(a, 2, 2);
	string year; year.append(a, 4, 4);
	//check ????????
	if (a == "????????")
		return true;
	//check size
	if (a.size() != 8)
		return false;
	//check that they're actually numbers
	try {
		stoi(month);
	}
	catch (invalid_argument ia) {
		return false;
	}
	try {
		stoi(day);
	}
	catch (invalid_argument ia) {
		return false;
	}
	try {
		stoi(year);
	}
	catch (invalid_argument ia) {
		return false;
	}
	//check valid month
	if (stoi(month) < 1 || stoi(month) > 12)
		return false;
	//check to make sure its not in the future
	if (process_date1(a) > process_date1(currentDate()))
		return false;
	//check sure its a reasonable year
	if (stoi(year) < 2009)
		return false;
	//check to see if its a valid day for each month (i.e, can't have 35th of May, or 31st of February)
	if (a[0] == num[0]) {
		if (a[1] == num[1] || a[1] == num[3] || a[1] == num[5] || a[1] == num[7] || a[1] == num[8]) {		//Jan, March, May, July, August have 31 days
			if (stoi(day) > 31)
				return false;
		}
		else if (a[1] == num[4] || a[1] == num[6] || a[1] == num[9]) {						//April, June, September have 30 days
			if (stoi(day) > 30)
				return false;
		}
		else if (a[1] == num[2]) {							//February has 28 days, but 29 on a leap year
			if (stoi(year) % 4 == 0) {
				if (stoi(day) > 29)
					return false;
			}
			else {
				if (stoi(day) > 28)
					return false;
			}
		}
	}
	else if (a[0] == num[1]) {																				
		if (a[0] == num[0] || a[0] == num[2]) {				//October and December have 31 days									
			if (stoi(day) > 31)
				return false;
		}
		else if (a[0] == num[1]) {							//November has 30 days
			if (stoi(day) > 30)
				return false;
		}
	}
	return true;
}

bool is_number(string a) {
	bool decimal_used = false;			// did we use up the decimal point? if so, can't use another
	try {
		stod(a);
	}
	catch (invalid_argument ia) {		// make sure its not a weird string or something
		return false;
	}
	for (int i = 0; i < a.size(); i++) {
		if (a[i] == 45 || a[i] == 46 || (a[i] <= 57 && a[i] >= 48)) {}		// check to see that every character is either a decimal point, a negative sign or a number
		else
			return false;
		if (a[i] == 45 && i != 0)		// make sure the negative sign is up front, not in the middle somewhere
			return false;
		if (a[i] == 46 && !decimal_used)
			decimal_used = true;
		else if (a[i] == 46 && decimal_used)	// if we get a decimal point, but have already used one, its wrong
			return false;
	}
	return true;
}

bool partial_match(string a, string b) {
	bool match = false;
	if (a.size() > b.size()) {
		for (int i = 0; i < a.size(); i++) {
			for (int j = 0; j < b.size(); j++) {
				if (i + j < a.size()) {					// make sure we dont exceed the outer loop's limits
					if (a[i + j] == b[j]) {
						match = true;
					}
					else {
						match = false;
						break;
					}
				}
				else
					match = false;
			}
			if (match)									// to make sure the function ends after its done finding the match
				return match;
		}
	}
	else if (b.size() > a.size()) {
		for (int i = 0; i < b.size(); i++) {
			for (int j = 0; j < a.size(); j++) {
				if (i + j < b.size()) {					// make sure we dont exceed the outer loop's limits
					if (b[i + j] == a[j]) {
						match = true;
					}
					else {
						match = false;
						break;
					}
				}
				else
					match = false;
			}
			if (match)
				return match;
		}
	}
	else if (a.size() == b.size())
		return (a == b);
	return match;
}

void read_history(vector <Materials>& thelist) {
	ifstream readfile("history.qcml");
	string temp;
	string tempc, tempw, tempd;
	int index;
	if (!readfile.is_open()) {
		while (!readfile.is_open()) {
			cout << "There was a problem loading the default directory for the material history. Please enter the filename.\n";
			cin >> temp;
			readfile.open(temp);
		}
	}
	while (!readfile.eof()) {
		readfile >> tempc;
		index = find_index(thelist, tempc);
		readfile >> tempw;
		readfile >> tempd;
		if (!readfile.eof())
			thelist[index].add_history(tempw, tempd);
	}
	readfile.close();
}

void write_history(vector <Materials>& thelist) {
	ofstream writefile("history.qcml");
	string tempc, tempw, tempd, buildline;
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	for (int i = 0; i < thelist.size(); i++) {
		for (int j = 0; j < thelist[i].h_size(); j++) {
			buildline.append(thelist[i].get_code());
			buildline.append("	");
			buildline.append(thelist[i].get_whist(j));
			buildline.append("	");
			buildline.append(thelist[i].get_dhist(j));
			writefile << buildline << endl;
			buildline.clear();
		}
	}
	writefile.close();
}

void write_ireport(vector <Materials>& thelist) {
	string filename = "reports/";
	filename.append(currentDate());
	filename.append("_inventory_report.txt");
	ofstream writefile(filename);
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	writefile << "BARCODE    MATERIAL             MANUFACTURER    DATE            OWNER                SIZE(um)   LOT        WEIGHT(kg) OPEN  ID       \n";
	writefile << "-------------------------------------------------------------------------------------------------------------------------------------\n";
	string buildline;
	cout << "Writing your report! DO NOT CLOSE THE PROGRAM!\n";
	for (int i = 0; i < thelist.size(); i++) {
		buildline.append(thelist[i].get_code());
		buildline.append(return_spaces(thelist[i].get_code(), 10));
		buildline.append(thelist[i].get_name());
		buildline.append(return_spaces(thelist[i].get_name(), 20));
		buildline.append(thelist[i].get_man());
		buildline.append(return_spaces(thelist[i].get_man(), 15));
		buildline.append(thelist[i].get_date());
		buildline.append(return_spaces(thelist[i].get_date(), 15));
		buildline.append(thelist[i].get_own());
		buildline.append(return_spaces(thelist[i].get_own(), 20));
		buildline.append(thelist[i].get_size());
		buildline.append(return_spaces(thelist[i].get_size(), 10));
		buildline.append(thelist[i].get_lot());
		buildline.append(return_spaces(thelist[i].get_lot(), 10));
		buildline.append(thelist[i].get_weight());
		buildline.append(return_spaces(thelist[i].get_weight(), 10));
		buildline.append(thelist[i].get_open());
		buildline.append(return_spaces(thelist[i].get_open(), 5));
		buildline.append(thelist[i].get_ID());
		writefile << buildline << endl;
		buildline.clear();
	}
	writefile.close();
	string tempcommand = "START WINWORD.exe ";
	tempcommand.append(filename);
	const char * command = tempcommand.c_str();
	system(command);
}

void write_as_report(vector <Materials>& targets) {
	double total = 0;
	string filename = "reports/";
	filename.append(currentDate());
	filename.append("_custom_report.txt");
	ofstream writefile(filename);
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	writefile << "BARCODE    MATERIAL             MANUFACTURER    DATE            OWNER                SIZE(um)   LOT        WEIGHT(kg) OPEN  ID       \n";
	writefile << "-------------------------------------------------------------------------------------------------------------------------------------\n";
	string buildline;
	cout << "Writing your report! DO NOT CLOSE THE PROGRAM!\n";
	for (int i = 0; i < targets.size(); i++) {
		buildline.append(targets[i].get_code());
		buildline.append(return_spaces(targets[i].get_code(), 10));
		buildline.append(targets[i].get_name());
		buildline.append(return_spaces(targets[i].get_name(), 20));
		buildline.append(targets[i].get_man());
		buildline.append(return_spaces(targets[i].get_man(), 15));
		buildline.append(targets[i].get_date());
		buildline.append(return_spaces(targets[i].get_date(), 15));
		buildline.append(targets[i].get_own());
		buildline.append(return_spaces(targets[i].get_own(), 20));
		buildline.append(targets[i].get_size());
		buildline.append(return_spaces(targets[i].get_size(), 10));
		buildline.append(targets[i].get_lot());
		buildline.append(return_spaces(targets[i].get_lot(), 10));
		buildline.append(targets[i].get_weight());
		total += stod(targets[i].get_weight());
		buildline.append(return_spaces(targets[i].get_weight(), 10));
		buildline.append(targets[i].get_open());
		buildline.append(return_spaces(targets[i].get_open(), 5));
		buildline.append(targets[i].get_ID());
		writefile << buildline << endl;
		buildline.clear();
	}
	writefile << "TOTAL WEIGHT: " << total << "kg\n";

	writefile.close();
	string tempcommand = "START WINWORD.exe ";
	tempcommand.append(filename);
	const char * command = tempcommand.c_str();
	system(command);
}

void write_hreport(vector <Materials>& thelist) {
	string filename = "reports/";
	filename.append(currentDate());
	filename.append("_history_report.txt");
	ofstream writefile(filename);
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	writefile << "BARCODE    MATERIAL             MANUFACTURER    OWNER                WEIGHT(kg) DATE           \n";
	writefile << "-----------------------------------------------------------------------------------------------\n";
	string buildline;
	cout << "Writing your report! DO NOT CLOSE THE PROGRAM!\n";
	for (int i = 0; i < thelist.size(); i++) {
		for (int j = 0; j < thelist[i].h_size(); j++) {
			buildline.append(thelist[i].get_code());
			buildline.append(return_spaces(thelist[i].get_code(), 10));
			buildline.append(thelist[i].get_name());
			buildline.append(return_spaces(thelist[i].get_name(), 20));
			buildline.append(thelist[i].get_man());
			buildline.append(return_spaces(thelist[i].get_man(), 15));
			buildline.append(thelist[i].get_own());
			buildline.append(return_spaces(thelist[i].get_own(), 20));
			buildline.append(thelist[i].get_whist(j));
			buildline.append(return_spaces(thelist[i].get_whist(j), 10));
			buildline.append(thelist[i].get_dhist(j));
			writefile << buildline << endl;
			buildline.clear();
		}
	}
	writefile.close();
	string tempcommand = "START WINWORD.exe ";
	tempcommand.append(filename);
	const char * command = tempcommand.c_str();
	system(command);
}

vector <Jobs> file_to_jobs(vector <Materials>& thelist) {
	vector <Jobs> joblist;
	ifstream readfile("jobs.qcml");
	if (!readfile.is_open()) {
		cout << "There was a problem reading the jobs file.\n";
		return joblist;
	}
	Jobs tempjob;
	string temp;
	int num_mats;
	int index;
	while (!readfile.eof()) {
		tempjob.clear();					// clear temporary variables
		temp.clear();
		index = -1;
		num_mats = 0;
		readfile >> temp;					// read in ID
		if (readfile.eof())					// extra little check for the end of the file
			break;				
		tempjob.set_ID(temp);				
		readfile >> temp;					// read in start date
		tempjob.set_sdate(temp);
		readfile >> temp;					// read in end date, empty string if (blank)
		if (temp == "(blank)")	
			temp.clear();
		tempjob.set_edate(temp);			
		readfile >> temp;					// read in start weight
		tempjob.add_sweight(temp);
		readfile >> temp;					// read in end weight, leave as 0 if (blank)
		if (temp != "(blank)")
			tempjob.add_eweight(temp);
		else
			tempjob.add_eweight("0");
		readfile >> temp;					// read in finished/unfinished state, set to true if 1
		if (temp == "1")
			tempjob.finish();	
		if (readfile.eof())
			break;
		readfile >> temp;					// read in # of materials
		num_mats = stoi(temp);
		for (int i = 0; i < num_mats; i++) {	// read in the next few lines as materials, depending on number read above
			readfile >> temp;
			tempjob.add_material(temp);
			index = find_index(thelist, temp);
			thelist[index].add_job(tempjob.get_ID());
		}
		if (tempjob.is_finished()) {
			for (int i = 0; i < tempjob.get_mats().size(); i++) {
				tempjob.add_ematerial(tempjob.get_mats()[i]);
			}
		}
		joblist.push_back(tempjob);			// add the completed job to the list
	}
	readfile.close();
	return joblist;
}

void write_jobs(vector <Jobs>& joblist) {
	ofstream writefile("jobs.qcml");
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	string buildline;
	string temp;
	for (int i = 0; i < joblist.size(); i++) {
		buildline.clear();

		buildline.append(joblist[i].get_ID());
		buildline.append(" ");

		buildline.append(joblist[i].get_sdate());
		buildline.append(" ");

		temp = joblist[i].get_edate();
		if (joblist[i].get_edate().size() == 0)
			temp = "(blank)";
		buildline.append(temp);
		buildline.append(" ");

		buildline.append(to_string(joblist[i].get_sweight()));
		buildline.append(" ");

		temp = to_string(joblist[i].get_eweight());
		if (joblist[i].get_eweight() == 0)
			temp = "(blank)";
		buildline.append(temp);
		buildline.append(" ");

		if (joblist[i].is_finished())
			temp = "1";
		else
			temp = "0";
		buildline.append(temp);
		buildline.append(" ");

		buildline.append(to_string(joblist[i].get_mats().size()));
		
		writefile << buildline << endl;

		for (int j = 0; j < joblist[i].get_mats().size(); j++) {
			writefile << joblist[i].get_mats()[j] << endl;
		}
	}
	writefile.close();
}

void cc_search(vector <Materials>& thelist) {
	prep_screen("searching");
	string temp;
	Materials * target = nullptr;
	double runsum = 0;
	cout << "\nEnter the barcode for the material or type 'back' to go back. \n";
	while (temp != "back") {
		cin >> temp;
		if (temp != "back") {
			target = search_material(thelist, temp);
			if (target == nullptr) {
				cout << "The material was not found, please try again.\n";
				continue;
			}
			Materials(*target).print_table();
			runsum += stod(Materials(*target).get_weight());
			cout << "Total weight: " << runsum << "kg\n\n";
		}
	}
	return;
}

void cc_find(vector <Materials>& thelist, vector <int> &recent) {
	prep_screen("searching");
	string temp;
	Materials * target = nullptr;
	vector <Materials*> matching;
	vector <Materials> targets;
	double runsum = 0;
	bool partial;
	while (temp != "back") {
		partial = false;
		cout << "\nEnter the field to search by or type 'back' to go back.\n";
		cin >> temp;
		if (temp == "name") {
			cout << "Which material name are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_name() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_name() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_name(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "manufacturer") {
			cout << "Which manufacturer are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_man() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_man() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_man(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "owner") {
			cout << "Which owner are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_own() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_own() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_own(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "date") {
			cout << "What date are you looking for? Entering 'today' will put in today's date.\n";
			cin >> temp;
			if (temp == "today")
				temp = currentDate();
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_date() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_date() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_date(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "size") {
			cout << "What size are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_size() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_size() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_size(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "lot") {
			cout << "Which lot # are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_lot() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_lot() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_lot(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "ID") {
			cout << "Which ID are you looking for?\n";
			cin >> temp;
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_ID() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_ID() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_ID(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		else if (temp == "weight") {
			while (1) {
				cout << "How much weight are you looking for?\n";
				cin >> temp;
				if (is_number(temp))
					break;
				cout << "That is not a valid number, please try again.\n";
			}
			if (matching.size() > 0) {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						if (Materials(*matching[i]).get_weight() != temp) {
							matching[i] = nullptr;
						}
					}
				}
			}
			else {
				for (int i = 0; i < thelist.size(); i++) {
					if (thelist[i].get_weight() == temp) {
						target = &thelist[i];
						matching.push_back(target);
					}
				}
				if (matching.size() == 0) {
					partial = true;
					for (int i = 0; i < thelist.size(); i++) {
						if (partial_match(thelist[i].get_weight(), temp)) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
			}
		}
		if (matching.size() > 0) {
			prep_screen("listing");
			runsum = 0;
			for (int i = 0; i < matching.size(); i++) {
				if (matching[i] != nullptr) {
					Materials(*matching[i]).print_table();
					runsum += stod(Materials(*matching[i]).get_weight());
				}
			}
			cout << "\nTotal weight: " << runsum << "kg\n";
			if (!partial) {
				cout << "\nThis is a list of the matching materials.\n";
				cout << "You can generate a report of the list, edit the materials in it, narrow it down/refine it, go back, or clear/restart the search.\n";
			}
			else {
				cout << "\nThis is a list of only partially matching materials- no perfect matches were found.\n";
				cout << "You can generate a report of the list, edit the materials in it, narrow it down/refine it, go back, or clear/restart the search.\n";
			}
			cin >> temp;
			if (temp == "r" || temp == "report") {
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr) {
						targets.push_back(Materials(*matching[i]));
					}
				}
				write_as_report(targets);
			}
			else if (temp == "edit") {
				cout << "Which field to edit?\n";
				cin >> temp;
				if (temp == "back")
					break;
				else if (temp == "name") {
					while (1) {
						cout << "Enter the new material name, less than 20 characters.\n";
						cin >> temp;
						if (temp.size() < 20) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_name(temp);
							}
							break;
						}
						else
							cout << "That name is the wrong length, please enter one less than 20 characters.\n";
					}
				}
				else if (temp == "manufacturer" || temp == "manufac" || temp == "man") {
					while (1) {
						cout << "Enter the new manufacturer name, less than 15 characters.\n";
						cin >> temp;
						if (temp.size() < 15) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_man(temp);
							}
							break;
						}
						else
							cout << "That name is the wrong length, please enter one less than 15 characters.\n";
					}
				}
				else if (temp == "date") {
					while (1) {
						cout << "Enter the new date, in the format MMDDYYYY. Entering 'today' will enter today's date.\n";
						cin >> temp;
						if (temp == "today")
							temp = currentDate();
						if (is_valid_date(temp)) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_date(temp);
							}
							break;
						}
						else
							cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
					}
				}
				else if (temp == "owner" || temp == "own") {
					while (1) {
						cout << "Enter the name of the owner, less than 20 characters.\n";
						cin >> temp;
						if (temp.size() < 20) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_own(temp);
							}
							break;
						}
						else
							cout << "That name is too long, please enter one less than 20 characters.\n";
					}
				}
				else if (temp == "size") {
					while (1) {
						cout << "Enter the size of the material's particles.\n";
						cin >> temp;
						if (temp.size() < 10) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_size(temp);
							}
							break;
						}
						else
							cout << "That number is too large, please enter the particle size.\n";
					}
				}
				else if (temp == "lot") {
					while (1) {
						cout << "Enter the lot number, less than 10 characters.\n";
						cin >> temp;
						if (temp.size() < 10) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_lot(temp);
							}
							break;
						}
						else
							cout << "That number/code, please enter one less than 10 characters.\n";
					}
				}
				else if (temp == "weight") {
					while (1) {
						while (1) {
							cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
							cin >> temp;
							if (is_number(temp))
								break;
							cout << "That is not a valid number, please try again.\n";
						}
						if (temp.size() < 10) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr) {
									thelist[find_index(thelist, matching[i])].set_weight(temp);
									thelist[find_index(thelist, matching[i])].set_open("1");
								}
							}
							break;
						}
						else
							cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
					}
				}
				else if (temp == "open") {
					cout << "The packaging is now labeled as 'open'.\n";
					for (int i = 0; i < matching.size(); i++) {
						if (matching[i] != nullptr)
							thelist[find_index(thelist, matching[i])].set_open("1");
					}
				}
				else if (temp == "closed") {
					cout << "The packaging is now labeled as 'closed'.\n";
					for (int i = 0; i < matching.size(); i++) {
						if (matching[i] != nullptr)
							thelist[find_index(thelist, matching[i])].set_open("0");
					}
				}
				else if (temp == "ID") {
					while (1) {
						cout << "Enter the job ID of the material, less than 10 characters.\n";
						cin >> temp;
						if (temp.size() < 10) {
							for (int i = 0; i < matching.size(); i++) {
								if (matching[i] != nullptr)
									thelist[find_index(thelist, matching[i])].set_ID(temp);
							}
							break;
						}
						else
							cout << "That ID is too long, please enter one less than 10 characters.\n";
					}
				}
				for (int i = 0; i < matching.size(); i++) {
					if (matching[i] != nullptr)
						add_recent(recent, find_index(thelist, matching[i]));
				}
			}
			else if (temp == "narrow" || temp == "refine") {
				continue;
			}
			else if  (temp == "back") {
				matching.clear();
				targets.clear();
				return;
			}
			matching.clear();
			targets.clear();
		}
		else if (temp != "back") {
			cout << "\nThere were no matching materials. Enter any key to try again.\n";
			cin >> temp;
		}
	}
	return;
}

void cc_edit(vector <Materials>& thelist, vector <int>& recent) {
	prep_screen("editing");
	int index;
	string temp;
	Materials * target = nullptr;
	while (temp != "back") {
		cout << "\nEnter the barcode for the material to be edited or type 'back' to go back. \n";
		cin >> temp;
		target = search_material(thelist, temp);
		if (target == nullptr) {
			if (temp != "back")
				cout << "The material was not found, please try again.\n";
			continue;
		}
		index = find_index(thelist, target);
		cout << "Which field to edit?\n";
		cin >> temp;
		if (temp == "back")
			break;
		else if (temp == "code" || temp == "barcode") {
			while (1) {
				cout << "Enter the new barcode, please no more than 9 digits or less than 3.\n";
				cout << "The current value for this field is: " << thelist[index].get_code() << endl;
				cin >> temp;
				target = search_material(thelist, temp);
				if (target != nullptr) {
					cout << "That barcode is already in use. Please enter an unused barcode.\n";
					continue;
				}
				if (temp.size() < 10 && temp.size() > 3) {
					thelist[index].set_code(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That barcode is the wrong length, please enter one between 9 and 3 digits.\n";
			}
		}
		else if (temp == "name") {
			while (1) {
				cout << "Enter the new material name, less than 20 characters.\n";
				cout << "The current value for this field is: " << thelist[index].get_name() << endl;
				cin >> temp;
				if (temp.size() < 20) {
					thelist[index].set_name(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That name is the wrong length, please enter one less than 20 characters.\n";
			}
		}
		else if (temp == "manufacturer" || temp == "manufac" || temp == "man") {
			while (1) {
				cout << "Enter the new manufacturer name, less than 15 characters.\n";
				cout << "The current value for this field is: " << thelist[index].get_man() << endl;
				cin >> temp;
				if (temp.size() < 15) {
					thelist[index].set_man(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That name is the wrong length, please enter one less than 15 characters.\n";
			}
		}
		else if (temp == "date") {
			while (1) {
				cout << "Enter the new date, in the format MMDDYYYY. Entering 'today' will enter today's date.\n";
				cout << "The current value for this field is: " << thelist[index].get_date() << endl;
				cin >> temp;
				if (temp == "today")
					temp = currentDate();
				if (is_valid_date(temp)) {
					thelist[index].set_date(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
			}
		}
		else if (temp == "owner" || temp == "own") {
			while (1) {
				cout << "Enter the name of the owner, less than 20 characters.\n";
				cout << "The current value for this field is: " << thelist[index].get_own() << endl;
				cin >> temp;
				if (temp.size() < 20) {
					thelist[index].set_own(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That name is too long, please enter one less than 20 characters.\n";
			}
		}
		else if (temp == "size") {
			while (1) {
				cout << "Enter the size of the material's particles.\n";
				cout << "The current value for this field is: " << thelist[index].get_size() << endl;
				cin >> temp;
				if (temp.size() < 10) {
					thelist[index].set_size(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That number is too large, please enter the particle size.\n";
			}
		}
		else if (temp == "lot") {
			while (1) {
				cout << "Enter the lot number, less than 10 characters.\n";
				cout << "The current value for this field is: " << thelist[index].get_lot() << endl;
				cin >> temp;
				if (temp.size() < 10) {
					thelist[index].set_lot(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That number/code, please enter one less than 10 characters.\n";
			}
		}
		else if (temp == "weight") {
			while (1) {
				while (1) {
					cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
					cout << "The current value for this field is: " << thelist[index].get_weight() << endl;
					cin >> temp;
					if (is_number(temp))
						break;
					cout << "That is not a valid number, please try again.\n";
				}
				if (temp.size() < 10) {
					thelist[index].set_weight(temp);
					thelist[index].set_open("1");
					add_recent(recent, index);
					break;
				}
				else
					cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
			}
		}
		else if (temp == "open") {
			cout << "The packaging is now labeled as 'open'.\n";
			thelist[index].set_open("1");
			add_recent(recent, index);
		}
		else if (temp == "closed") {
			cout << "The packaging is now labeled as 'closed'.\n";
			thelist[index].set_open("0");
			add_recent(recent, index);
		}
		else if (temp == "ID") {
			while (1) {
				cout << "Enter the job ID of the material, less than 10 characters.\n";
				cout << "The current value for this field is: " << thelist[index].get_ID() << endl;
				cin >> temp;
				if (temp.size() < 10) {
					thelist[index].set_ID(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "That ID is too long, please enter one less than 10 characters.\n";
			}
		}
		else if (temp == "clone") {
			while (1) {
				cout << "Enter the barcode of the material whose properties you want to copy to this material.\n";
				cin >> temp;
				target = search_material(thelist, temp);
				if (target != nullptr) {
					thelist[index] = Materials(*target);
					while (1) {
						while (1) {
							cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
							cin >> temp;
							if (is_number(temp))
								break;
							cout << "That is not a valid number, please try again.\n";
						}
						if (temp.size() < 10) {
							break;
						}
						else
							cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
					}
					thelist[index].set_weight(temp);
					add_recent(recent, index);
					break;
				}
				else
					cout << "The material to be cloned was not found, please enter a valid barcode.\n";
			}
		}
		else {
			cout << "That field was not found, please choose one from the following: \n";
			cout << "\n	barcode\n	name\n	manufacturer\n	date\n	owner\n	size\n	lot\n	weight\n	ID\n\n";
			cout << "Or just type 'open' or 'closed' to change the packaging condition to the former or latter.\n";
		}
	}
	return;
}

void cc_add(vector <Materials>& thelist, vector <int>& recent) {
	prep_screen("adding");
	Materials temp_mat;
	string temp;
	Materials * target = nullptr;
	string * info_set;
	while (temp != "back") {
		temp_mat.clear();
		while (1) {
			cout << "\nPlease enter the barcode for the new material/package.\n";
			cin >> temp;
			if (temp != "back") {
				target = search_material(thelist, temp);
				if (target == nullptr)
					break;
				else
					cout << "That barcode is already in use. Please enter an unused code.\n";
			}
			else
				break;
		}
		if (temp != "back") {
			temp_mat.set_code(temp);
			cout << "Is this material like any other?\n";
			cin >> temp;
			if (temp == "y" || temp == "yes") {
				while (1) {
					cout << "Enter the barcode of the other material.\n";
					cin >> temp;
					target = search_material(thelist, temp);
					if (target == nullptr) {
						cout << "That material was not found, please try again.\n";
						continue;
					}
					temp_mat = *target;
					temp_mat.clear_history();
					while (1) {
						while (1) {
							cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
							cin >> temp;
							if (is_number(temp))
								break;
							cout << "That is not a valid number, please try again.\n";
						}
						if (temp.size() < 10) {
							break;
						}
						else
							cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
					}
					temp_mat.set_weight(temp);
					temp_mat.add_history(temp_mat.get_weight(), currentDate());
					thelist.push_back(temp_mat);
					add_recent(recent, thelist.size() - 1);
					break;
				}
			}
			else if (temp == "back") {}
			else if (temp == "n" || temp == "no") {
				info_set = new string[10];
				info_set[0] = temp_mat.get_code();
				while (1) {
					cout << "Enter the new material name, less than 20 characters.\n";
					cin >> temp;
					if (temp.size() < 20) {
						info_set[1] = temp;
						break;
					}
					else
						cout << "That name is the wrong length, please enter one less than 20 characters.\n";
				}
				while (1) {
					cout << "Enter the new manufacturer name, less than 15 characters.\n";
					cin >> temp;
					if (temp.size() < 15) {
						info_set[2] = temp;
						break;
					}
					else
						cout << "That name is the wrong length, please enter one less than 15 characters.\n";
				}
				while (1) {
					cout << "Enter the new date, in the format MMDDYYYY. Entering 'today' will put in today's date.\n";
					cin >> temp;
					if (temp == "today")
						temp = currentDate();
					if (is_valid_date(temp)) {
						info_set[3] = temp;
						break;
					}
					else
						cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
				}
				while (1) {
					cout << "Enter the name of the owner, less than 20 characters.\n";
					cin >> temp;
					if (temp.size() < 20) {
						info_set[4] = temp;
						break;
					}
					else
						cout << "That name is too long, please enter one less than 20 characters.\n";
				}
				while (1) {
					cout << "Enter the size of the material's particles.\n";
					cin >> temp;
					if (temp.size() < 10) {
						info_set[5] = temp;
						break;
					}
					else
						cout << "That number is too large, please enter the particle size.\n";
				}
				while (1) {
					cout << "Enter the lot number, less than 10 characters.\n";
					cin >> temp;
					if (temp.size() < 10) {
						info_set[6] = temp;
						break;
					}
					else
						cout << "That number/code, please enter one less than 10 characters.\n";
				}
				while (1) {
					while (1) {
						cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
						cin >> temp;
						if (is_number(temp))
							break;
						cout << "That is not a valid number, please try again.\n";
					}
					if (temp.size() < 10) {
						info_set[7] = temp;
						break;
					}
					else
						cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
				}
				while (1) {
					cout << "Is this package open or closed?\n";
					cin >> temp;
					if (temp == "o" || temp == "open") {
						info_set[8] = "1";
						break;
					}
					else if (temp == "c" || temp == "closed") {
						info_set[8] = "0";
						break;
					}
					else
						cout << "Please enter 'open' or 'closed.'\n";
				}
				while (1) {
					cout << "Enter the job ID of the material, less than 10 characters.\n";
					cin >> temp;
					if (temp.size() < 10) {
						info_set[9] = temp;
						break;
					}
					else
						cout << "That ID is too long, please enter one less than 10 characters.\n";
				}
				thelist.push_back(process_material(info_set));
				thelist[thelist.size() - 1].add_history(info_set[7], currentDate());
				add_recent(recent, thelist.size() - 1);
				delete[] info_set;
			}
		}
	}
	return;
}

void cc_save(vector <Materials>& thelist, vector <Jobs>& joblist) {
	prep_screen("saving");
	cout << "Save changes to file?\n";
	string temp;
	cin >> temp;
	if (temp == "yes" || temp == "y") {
		write_to_file(thelist, "data.qcml");
		write_history(thelist);
		write_jobs(joblist);
	}
	cout << "Changes saved";
	return;
}

void cc_print(vector <Materials>& thelist) {
	string temp;
	prep_screen("listing");
	for (int i = 0; i < thelist.size(); i++) {
		thelist[i].print_table();
	}
	cout << "\nHit any key to go back \n";
	cin >> temp;
	return;
}

void cc_history(vector <Materials>& thelist) {
	prep_screen("history");
	string temp;
	Materials * target = nullptr;
	while (temp != "back") {
		cin >> temp;
		if (temp != "back") {
			target = search_material(thelist, temp);
			if (target == nullptr) {
				cout << "That material could not be found, please try again.\n";
				continue;
			}
			else {
				Materials(*target).print_history();
			}
		}
	}
	return;
}

void cc_report(vector <Materials>& thelist) {
	prep_screen("report");
	string temp;
	while (temp != "back") {
		cout << "What kind of report would you like? Please choose from 'history' or 'inventory' or type 'back' to go back.\n";
		cin >> temp;
		if (temp == "i" || temp == "inventory") {
			write_ireport(thelist);
		}
		else if (temp == "h" || temp == "history") {
			write_hreport(thelist);
		}
		else if (temp == "back")
			return;
		else {
			cout << "That is not a valid report, please choose from the given options.\n";
			continue;
		}
		prep_screen("report");
	}
	return;
}

void cc_recent(vector <Materials>& thelist, vector <int>& recent) {
	prep_screen("recent");
	string temp;
	double runsum = 0;
	for (int i = 0; i < recent.size(); i++) {
		thelist[recent[i]].print_table();
		runsum += stod(thelist[recent[i]].get_weight());
	}
	cout << "\nTotal weight: " << runsum << "kg\n\n";
	cout << "Would you like to generate a report of this list?\n";
	cout << "\nOtherwise, hit any key or type 'back' to leave this menu.\n";
	cin >> temp;
	if (temp == "y" || temp == "yes") {
		vector <Materials> targets;
		for (int i = 0; i < recent.size(); i++) {
			targets.push_back(thelist[recent[i]]);
		}
		write_as_report(targets);
	}

	return;
}

void cc_job(vector <Materials>& thelist, vector <int>& recent, vector <Jobs>& joblist) {
	string temp;
	Jobs tempjob;
	Materials * target = nullptr;
	int index;
	int jindex;
	while (1) {
		tempjob.clear();
		target = nullptr;
		index = -1;
		jindex = -1;
		prep_screen("jobs");
		cout << "Would you like to create a new job, or view/edit an existing one?\n\n";
		cin >> temp;
		if (temp == "back" || temp == "done")
			return;
		else if (temp == "new") {
			cout << "What is the new job ID?\n";
			cin >> temp;
			tempjob.set_ID(temp);
			while (1) {
				cout << "What is the start date? Entering 'today' will put in the current date.\n";
				cin >> temp;
				if (temp == "today") {
					temp = currentDate();
				}
				if (is_valid_date(temp))
					break;
				cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
			}
			tempjob.set_sdate(temp);
			cout << "Please enter the barcode(s) of the materials to be used. Enter 'done' when you are finished.\n";
			while (temp != "done" && temp != "back") {
				prep_screen("jobs");
				cout << "Job ID: " << tempjob.get_ID() << endl;
				cout << "Current starting weight: " << tempjob.get_sweight() << endl;
				cout << "List of associated materials: \n";
				for (int i = 0; i < tempjob.get_mats().size(); i++) {
					cout << tempjob.get_mats()[i] << endl;
				}
				cout << "\nEnter a barcode.\n";
				cin >> temp;
				if (temp == "done" || temp == "back")
					continue;
				target = search_material(thelist, temp);
				if (target == nullptr) {
					cout << "That material was not found, would you like to enter it?\n";
					cin >> temp;
					if (temp == "yes" || temp == "y") {
						cc_add(thelist, recent);
						prep_screen("jobs");
					}
					continue;
				}
				if (tempjob.is_used(Materials(*target).get_code())) {
					cout << "That material is already being used in this job- please enter another.\n";
					continue;
				}
				index = find_index(thelist, target);
				tempjob.add_material(thelist[index].get_code());
				tempjob.add_sweight(thelist[index].get_weight());
				thelist[index].set_ID(tempjob.get_ID());
				thelist[index].set_open("1");
			}
			joblist.push_back(tempjob);
		}
		else if (temp == "view" || temp == "edit" || temp == "existing") {
			while (1) {
				prep_screen("jobs");
				cout << "Here is a list of existing jobs:\n\n";
				cout << "ID         START      END        STARTING (kg) ENDING (kg)   FINISHED\n";
				cout << "---------------------------------------------------------------------\n";
				for (int i = 0; i < joblist.size(); i++) {
					joblist[i].print();
				}
				cout << endl << endl;
				jindex = -1;
				cout << "Enter the job ID to load, or type 'back' to go back.\n";
				cin >> temp;
				if (temp == "back")
					break;
				jindex = find_index(joblist, temp);
				if (jindex == -1) {
					cout << "That job ID was not found. Please try again, press any key to go back.\n";
					cin >> temp;
					continue;
				}
				while (1) {
					prep_screen("jobs");
					cout << "Job ID: " << joblist[jindex].get_ID() << " loaded.\n";
					cout << "What would you like to do with the job? Choose from the following options:\n\n";
					cout << "1. info\n2. finish\n3. add\n4. set end weight manually\n5. done\n\n";
					cin >> temp;
					if (temp == "done" || temp == "back")
						break;
					else if (temp == "info" || temp == "information" || temp == "i") {
						cout << "ID         START      END        STARTING (kg) ENDING (kg)   FINISHED\n";
						cout << "---------------------------------------------------------------------\n";
						joblist[jindex].print();
						cout << "\nList of associated materials: \n\n";
						for (int i = 0; i < joblist[jindex].get_mats().size(); i++) {
							cout << joblist[jindex].get_mats()[i] << endl;
						}
						cout << endl;
						cout << "Press any key to go back.\n";
						cin >> temp;
					}
					else if (temp == "set" || temp == "manually" || temp == "manual") {
						cout << "ID         START      END        STARTING (kg) ENDING (kg)   FINISHED\n";
						cout << "---------------------------------------------------------------------\n";
						joblist[jindex].print();
						cout << "\nList of associated materials: \n\n";
						for (int i = 0; i < joblist[jindex].get_mats().size(); i++) {
							cout << joblist[jindex].get_mats()[i] << endl;
						}
						while (1) {
							cout << "What was the finishing date? Entering 'today' will put in the current date.\n";
							cin >> temp;
							if (temp == "today") {
								temp = currentDate();
							}
							if (is_valid_date(temp))
								break;
							cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
						}
						joblist[jindex].set_edate(temp);
						while (1) {
							cout << "\nPlease manually enter the desired ending weight.\n";
							cin >> temp;
							if (is_number(temp))
								break;
							cout << "That is not a valid number, please try again.\n";
						}
						joblist[jindex].add_eweight(temp);
						joblist[jindex].finish();
					}
					else if (temp == "add") {
						cout << "Please enter the barcode(s) of the materials to be used. Enter 'done' when you are finished.\n";
						while (temp != "done" && temp != "back") {
							cout << "Enter a barcode.\n";
							cin >> temp;
							if (temp == "done" || temp == "back")
								continue;
							target = search_material(thelist, temp);
							if (target == nullptr) {
								cout << "That material was not found, would you like to enter it?\n";
								cin >> temp;
								if (temp == "yes" || temp == "y") {
									cc_add(thelist, recent);
									prep_screen("jobs");
								}
								continue;
							}
							if (joblist[jindex].is_used(Materials(*target).get_code())) {
								cout << "That material is already being used in this job- please enter another.\n";
								continue;
							}
							index = find_index(thelist, target);
							joblist[jindex].add_material(thelist[index].get_code());
							joblist[jindex].add_sweight(thelist[index].get_weight());
							thelist[index].set_ID(joblist[jindex].get_ID());
							thelist[index].set_open("1");
						}
					}
					else if (temp == "finish" || temp == "f") {
						while (1) {
							cout << "What was the finishing date? Entering 'today' will put in the current date.\n";
							cin >> temp;
							if (temp == "today") {
								temp = currentDate();
							}
							if (is_valid_date(temp))
								break;
							cout << "That is not a valid date, please enter one as MMDDYYYY or enter ???????? if a date is not available.\n";
						}
						joblist[jindex].set_edate(temp);
						cout << "Please enter the barcode(s) of the materials that were used. Enter 'done' when you are finished.\n";
						cout << "Be sure to enter all the materials you used, even empty packages.\n";
						while (temp != "done" && temp != "back") {
							cout << "Enter a barcode.\n";
							cin >> temp;
							if (temp == "done" || temp == "back")
								continue;
							target = search_material(thelist, temp);
							if (target == nullptr) {
								cout << "That material was not found, please enter a valid barcode.\n";
								continue;
							}
							if (!joblist[jindex].is_used(Materials(*target).get_code())) {
								cout << "That material was not found in this job- please enter another.\n";
								continue;
							}
							if (joblist[jindex].is_end_used(Materials(*target).get_code())) {
								cout << "That material has already been entered to finish the job- please enter another.\n";
								continue;
							}
							while (1) {
								cout << "Please enter the material's current weight.\n";
								cin >> temp;
								if (is_number(temp))
									break;
								cout << "That is not a valid number, please try again.\n";
							}
							index = find_index(thelist, target);
							thelist[index].set_weight(temp);
							joblist[jindex].add_eweight(thelist[index].get_weight());
							joblist[jindex].add_ematerial(thelist[index].get_code());
						}
						joblist[jindex].finish();
					}
				}
			}
		}
	}
}

void cc_lots(vector <Materials> &thelist) {
	vector <Lots> lotlist;
	vector <Materials> usedlots;
	string temp;
	Lots templot;
	bool used;
	prep_screen("lots");
	while (temp != "back") {
		cout << "Please enter the lot# or barcode of the material whose lot history you would like to generate.\n";
		cin >> temp;
		if (temp == "report_all") {
			for (int i = 0; i < thelist.size(); i++) {
				used = false;
				for (int j = 0; j < usedlots.size(); j++) {
					if (usedlots[j].get_lot() == thelist[i].get_lot())
						used = true;
				}
				if (!used)
					usedlots.push_back(thelist[i]);
			}
			string filename = "reports/";
			filename.append(currentDate());
			filename.append("_lot_report.txt");
			ofstream writefile(filename);
			if (!writefile.is_open()) {
				cout << "There was a problem writing the file.\n";
				continue;
			}
			for (int i = 0; i < usedlots.size(); i++) {
				templot.build(usedlots[i], thelist);
				templot.write(writefile);
			}
			cout << "Report written.\n\n";
		}
		for (int i = 0; i < thelist.size(); i++) {
			if (thelist[i].get_lot() == temp) {
				templot.build(thelist[i], thelist);
				templot.print();
				break;
			}
			else if (thelist[i].get_code() == temp) {
				templot.build(thelist[i], thelist);
				templot.print();
				break;
			}
		}
	}
}

void cc_usage(vector <Materials> &thelist) {
	prep_screen("usage");
	vector <Lots> lotlist;
	vector <Materials> usedlots;
	bool used;
	Lots templot;
	for (int i = 0; i < thelist.size(); i++) {
		used = false;
		for (int j = 0; j < usedlots.size(); j++) {
			if (usedlots[j].get_lot() == thelist[i].get_lot())
				used = true;
		}
		if (!used)
			usedlots.push_back(thelist[i]);
	}
	for (int i = 0; i < usedlots.size(); i++) {
		templot.build(usedlots[i], thelist);
		if (used_recently(templot))
			lotlist.push_back(templot);
	}
	string filename = "reports/";
	filename.append(currentDate());
	filename.append("_usage_report.txt");
	ofstream writefile(filename);
	if (writefile.is_open()) {
		for (int i = 0; i < lotlist.size(); i++) {
			lotlist[i].write(writefile);
			writefile << "Total usage for " << currentMonth_word() << " " << currentYear() << ": " << endl;
			writefile << "	Used:   " << lotlist[i].get_usage() << "kg\n";
			writefile << "	Gained: " << lotlist[i].get_gained() << "kg\n\n";
			lotlist[i].print();
			cout << "Total usage for " << currentMonth_word() << " " << currentYear() << ": " << endl;
			cout << "	Used:   " << lotlist[i].get_usage() << "kg\n";
			cout << "	Gained: " << lotlist[i].get_gained() << "kg\n\n";
		}
		cout << "Done. Press any key to go back.\n";
		writefile.close();
	}
	else {
		cout << "There was a problem writing your file. Press any key to go back.\n";
	}
	string temp;
	cin >> temp;
}

void cc_help() {
	prep_screen("helping");
	string temp;
	cout << "Use 'search' or 'lookup' to look up info for different materials.\n";
	cout << "Use 'find' to gather a list of materials matching a certain criteria.\n";
	cout << "Use 'edit' to alter or update the info for each material.\n";
	cout << "To add a new material, use 'add'.\n";
	cout << "In the main menu, use 'exit' to exit to the program and save changes to the database.\n";
	cout << "Enter 'back' in any function to return to the main menu.\n";
	cout << "Use 'save' to save the changes to the database without closing the program.\n";
	cout << "To print out a complete list of the inventory, type 'list' or 'print' in the main menu.\n";
	cout << "To generate a report on the inventory or material weight history, type 'report'.\n";
	cout << "To add a job to memory, type 'jobs'.\n";
	cout << "The create a history for a group of powder by lot, type 'lots'.\n";
	cout << "To create a usage history of all the powder used in the current month, type 'usage'.\n";
	cout << "For information about this program, type 'about'.\n";
	cout << "\nContact Sameer Sajid for help with any bugs or problems during usage.\n";
	cout << "\nssajid@qcml.org\n401-864-5676\n";
	cout << "\nHit any key to leave this menu.\n";
	cin >> temp;
	return;
}

void cc_about() {
	prep_screen("about");
	string temp;
	cout << "This program was written and developed by Sameer Sajid for the Quad City Manufacturing Laboratory.\n";
	cout << "\nAll rights reserved.\n";
	cout << "\nHit any key to leave this menu.\n";
	cin >> temp;
	return;
}

void cc_experimental() {
	string temp1, temp2;
	while (temp1 != "back" && temp2 != "back") {
		cout << "Enter two words.\n";
		cin >> temp1;
		cin >> temp2;
		if (temp1 == "back" || temp2 == "back")
			continue;
		cout << partial_match(temp1, temp2) << endl;
	}
}