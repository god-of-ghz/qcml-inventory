#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

#include <materials.h>
#include <time.h>

vector <Materials> file_to_vec(ifstream& reading);
void read_history(vector <Materials>& thelist);
void write_to_file(vector <Materials>& thelist, string filename);
void write_history(vector <Materials>& thelist);
void write_ireport(vector <Materials>& thelist);
void write_hreport(vector <Materials>& thelist);

Materials process_material(string * info_set);
Materials * search_material(vector <Materials>& thelist, string a);
int find_index(vector <Materials>& thelist, Materials * target);
int find_index(vector <Materials>& thelist, string code);

const string currentDate();

void experiment(vector <Materials>& thelist);

void prep_screen(string menu);

void check_recent(vector <int>& recent);
//void add_recent(vector <int>& recent, Materials * target);
void add_recent(vector <int>& recent, int index);

void command_center(vector <Materials>& thelist) {
	string command;
	string temp;
	Materials * target = nullptr;
	vector <int> recent;
	vector <Materials*> matching;
	Materials temp_mat;
	string * info_set;
	while (1) {
		check_recent(recent);
		prep_screen("main");
		cout << "\nEnter a command.\n\n";
		cin >> command;
		temp.clear();
		if (command == "lookup" || command == "search") {
			prep_screen("searching");
			while (temp != "back") {
				cout << "\nEnter the barcode for the material or type 'back' to go back. \n";
				cin >> temp;
				if (temp != "back") {
					target = search_material(thelist, temp);
					if (target == nullptr) {
						cout << "The material was not found, please try again.\n";
						continue;
					}
					Materials(*target).print_me();
				}
			}
		}
		else if (command == "find") {
			prep_screen("searching");
			matching.clear();
			while (temp != "back") {
				cout << "\nEnter the field to search by or type 'back' to go back.\n";
				cin >> temp;
				if (temp == "name") {
					cout << "Which material name are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_name() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "manufacturer") {
					cout << "Which manufacturer are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_man() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "owner") {
					cout << "Which owner are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_own() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "date") {
					cout << "Which date are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_date() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "size") {
					cout << "Which size are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_size() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "lot") {
					cout << "Which lot # are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_lot() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				else if (temp == "ID") {
					cout << "Which ID are you looking for?\n";
					cin >> temp;
					for (int i = 0; i < thelist.size(); i++) {
						if (thelist[i].get_ID() == temp) {
							target = &thelist[i];
							matching.push_back(target);
						}
					}
				}
				if (matching.size() != 0) {
					prep_screen("listing");
					for (int i = 0; i < matching.size(); i++) {
						Materials(*matching[i]).print_table();
					}
					cout << "\nThis is a list of the matching materials. Enter any key to go back.\n";
					cin >> temp;
				}
				else if (temp != "back") {
					cout << "\nThere were no matching materials. Enter any key to try again.\n";
					cin >> temp;
				}
			}
		}
		else if (command == "edit") {
			prep_screen("editing");
			int index;
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
				add_recent(recent, index);
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
							break;
						}
						else
							cout << "That name is the wrong length, please enter one less than 15 characters.\n";
					}
				}
				else if (temp == "date") {
					while (1) {
						cout << "Enter the new date, in the format MMDDYYYY.\n";
						cout << "The current value for this field is: " << thelist[index].get_date() << endl;
						cin >> temp;
						if (temp.size() == 8) {
							thelist[index].set_date(temp);
							break;
						}
						else
							cout << "That date is the wrong format, please enter one as MMDDYYYY.\n";
					}
				}
				else if (temp == "owner" || temp == "own") {
					while (1) {
						cout << "Enter the name of the owner, less than 20 characters.\n";
						cout << "The current value for this field is: " << thelist[index].get_own() << endl;
						cin >> temp;
						if (temp.size() < 20) {
							thelist[index].set_own(temp);
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
							break;
						}
						else
							cout << "That number/code, please enter one less than 10 characters.\n";
					}
				}
				else if (temp == "weight") {
					while (1) {
						cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
						cout << "The current value for this field is: " << thelist[index].get_weight() << endl;
						cin >> temp;
						if (temp.size() < 10) {
							thelist[index].set_weight(temp);
							break;
						}
						else
							cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
					}
				}
				else if (temp == "open") {
					cout << "The packaging is now labeled as 'open'.\n";
					thelist[index].set_open("1");
				}
				else if (temp == "closed") {
					cout << "The packaging is now labeled as 'closed'.\n";
					thelist[index].set_open("0");
				}
				else if (temp == "ID") {
					while (1) {
						cout << "Enter the job ID of the material, less than 10 characters.\n";
						cout << "The current value for this field is: " << thelist[index].get_ID() << endl;
						cin >> temp;
						if (temp.size() < 10) {
							thelist[index].set_ID(temp);
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
								cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
								cin >> temp;
								if (temp.size() < 10) {
									break;
								}
								else
									cout << "That number is too precise, enter a number with less than 10 significant digits.\n";
							}
							thelist[index].set_weight(temp);
							break;
						}
						else
							cout << "The material to be cloned was not found, please enter a valid barcode.\n";
					}
				}
				else {
					cout << "That field was not found, please choose one from the following: \n";
					cout << "\n	barcode\n	name\n	manufacturer\n	date\n	owner\n	size\n	lot\n	weight\n	ID\n\n";
					cout << "Or just type 'open' or 'closed' to change the packging condition to the former or latter.\n";
					cout << "\nHit any ket to go back.\n";
					cin >> temp;
				}
			}
		}
		else if (command == "exit" || command == "back") {
			prep_screen("exiting");
			cout << "Are you sure you want to exit the program?\n";
			cin >> temp;
			if (temp == "yes" || temp == "y")
				break;
		}
		else if (command == "help") {
			prep_screen("helping");
			cout << "Use 'search' or 'lookup' to look up info for different materials.\n";
			cout << "Use 'find' to gather a list of materials matching a certain criteria.\n";
			cout << "Use 'edit' to alter or update the info for each material.\n";
			cout << "In the main menu, use 'exit' to exit to the program and save changes to the database.\n";
			cout << "Enter 'back' in any function to return to the main menu.\n";
			cout << "Use 'save' to save the changes to the database without closing the program.\n";
			cout << "To print out a complete list of the inventory, type 'list' or 'print' in the main menu.\n";
			cout << "To add a new material, use 'add'.\n";
			cout << "For information about this program, type 'about'.\n";
			cout << "\nContact Sameer Sajid for help with any bugs or problems during usage.\n";
			cout << "\nssajid@qcml.org\n401-864-5676\n";
			cout << "\nHit any key to leave this menu.\n";
			cin >> temp;
		}
		else if (command == "save") {
			prep_screen("saving");
			cout << "Save changes to file?\n";
			cin >> temp;
			if (temp == "yes" || temp == "y") {
				write_to_file(thelist, "data.qcml");
				write_history(thelist);
			}
			cout << "Changes saved";
		}
		else if (command == "print" || command == "list") {
			prep_screen("listing");
			for (int i = 0; i < thelist.size(); i++) {
				thelist[i].print_table();
			}
			cout << "\nHit any key to go back \n";
			cin >> temp;
		}
		else if (command == "history") {
			prep_screen("history");
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
		}
		else if (command == "recent") {
			prep_screen("recent");
			for (int i = 0; i < recent.size(); i++) {
				thelist[recent[i]].print_table();
			}
			cout << "\nHit any key to leave this menu.\n";
			cin >> temp;
		}
		else if (command == "add") {
			prep_screen("adding");
			temp_mat.clear();
			while (temp != "back") {
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
								cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
								cin >> temp;
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
							cout << "Enter the new date, in the format MMDDYYYY.\n";
							cin >> temp;
							if (temp.size() == 8) {
								info_set[3] = temp;
								break;
							}
							else
								cout << "That date is the wrong format, please enter one as MMDDYYYY.\n";
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
							cout << "Enter the remaining weight of the material, in kg, less than 10 significant digits.\n";
							cin >> temp;
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
		}
		else if (command == "report") {
			prep_screen("report");
			cout << "Would you like to generate a report?\n";
			cin >> temp;
			if (temp == "y" || temp == "yes") {
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
						break;
					else {
						cout << "That is not a valid report, please choose from the given options.\n";
						continue;
					}
					prep_screen("report");
				}
			}
		}
		else if (command == "about") {
			prep_screen("about");
			cout << "This program was written and developed by Sameer Sajid for the Quad City Manufacturing Laboratory.\n";
			cout << "\nAll rights reserved.\n";
			cout << "\nHit any key to leave this menu.\n";
			cin >> temp;
		}
		/*else if (command == "experiment") {
			prep_screen("testing");
			experiment(thelist);
			cin >> temp;
		}*/
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
	cout << "Database loaded!.\n";
	

	command_center(thelist);

	prep_screen("exiting");
	write_to_file(thelist, filename);
	write_history(thelist);
	
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
	temp_mat.set_ID(info_set[9]);
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
	cout << "+-------------------------------------+\n";
	cout << "|       QCML INVENTORY SYSTEM V1.4    |\n";
	cout << "+-------------------------------------+\n\n";

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
	else
		cout << "~~~~~~~~~~~~~~~COMING SOON!~~~~~~~~~~~~~\n\n";

	if (menu == "listing" || menu == "recent") {
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
}

int find_index(vector <Materials>& thelist, string a) {
	for (int i = 0; i < thelist.size(); i++) {
		if (thelist[i].get_code() == a)
			return i;
	}
}

void check_recent(vector <int>& recent) {
	if (recent.size() > 100)
		recent.clear();
}

void add_recent(vector <int>& recent, int index) {
	recent.push_back(index);
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
	string filename;
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

void write_hreport(vector <Materials>& thelist) {
	string filename;
	filename.append(currentDate());
	filename.append("_history_report.txt");
	ofstream writefile(filename);
	if (!writefile.good()) {
		cout << "There was a problem writing the file.\n";
		return;
	}
	writefile << "BARCODE    WEIGHT(kg) DATE           \n";
	writefile << "-------------------------------------\n";
	string buildline;
	cout << "Writing your report! DO NOT CLOSE THE PROGRAM!\n";
	for (int i = 0; i < thelist.size(); i++) {
		for (int j = 0; j < thelist[i].h_size(); j++) {
			buildline.append(thelist[i].get_code());
			buildline.append(return_spaces(thelist[i].get_code(), 10));
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

void experiment(vector <Materials>& thelist) {
	Materials * target1;
	Materials * target2;
	target1 = &thelist[5];
	target2 = &thelist[5];
	//thelist[5].set_code("421341");
	cout << Materials(*target1).get_code() << endl;
	cout << Materials(*target2).get_code() << endl;

}