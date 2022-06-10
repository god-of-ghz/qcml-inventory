# qcml-inventory
an old inventory system I worked on for the quad city manufacturing laboratory
Quad City Manufacturing Laboratory Inventory Program
User Manual and Log

 

Written and maintained by
Sameer Sajid
 
User Manual
1.	Getting started
a.	Click on the program executable file (.exe) to get started
b.	The program will open, prompting the user to enter the filename of the inventory data if it is not found automatically
c.	Data is loaded and program is ready for use
2.	Main Menu
a.	Main menu opens and prompts user to enter a command, list of commands are as follows

Version History

V1.0 7/22/2014
-	Added reading from file function
-	Added writing to file function
-	Added class containing material objects
-	Added basic command-line user interface
-	Added main menu
-	Added search, lookup, edit, back, help, save, list, print, add and about functions
-	Disabled recent function
-	Disabled experimental function
-	Cleaned up main function
-	Fixed main menu heading
-	Fixed miscellaneous formatting for readability
-	Known bugs:
o	‘recent’ function crashes program after ‘add’ function has been used several times
o	‘add’ does not check to see if a new barcode is already being used
o	‘edit’ does not check to see if an edited barcode is already being used
o	‘back’ cannot be used during ‘add’ if a new material was already started (process cannot be canceled)
-	Known problems
o	‘save’ does not automatically run when database is changed
o	Program still prompts user for file input and output, does not run automatically
o	Database file is unencrypted
o	No method of exporting data
