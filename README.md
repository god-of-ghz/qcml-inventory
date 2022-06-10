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
b.	‘search’ or ‘lookup’
i.	User types in a barcode or scans one into the program with a barcode scanner
ii.	Program will search for the material matching that barcode
iii.	If the barcode is found, a list of that material’s properties is shown on screen
iv.	If the barcode is not found, program will say the material was unable to be found
c.	‘find’
i.	User types in the field to search by, and then the value for that field
ii.	Returns a list of all materials that match this criteria
1.	If no exact matches are found, ‘find’ will search for partial matches instead, but only on the initial run
2.	Also reports the total weight of all the results
iii.	Use can then choose from one of several options
1.	‘refine’ or ‘narrow’ down the results to search within the entries on-screen
2.	‘clear’ or ‘restart’ the function and begin a new search
3.	Export the results as a ‘report’ 
d.	‘print’ or ‘list’
i.	With this command, the program shows a complete list onscreen of every item in the inventory system
e.	‘edit’
i.	User types in a barcode or scans one into the program with a barcode scanner
ii.	Program will search for the material matching that barcode
iii.	If the barcode is found, program will prompt user to input what field to edit and its new value
1.	‘barcode’
a.	The new barcode for this material
2.	‘name’
a.	The new name for the material
3.	‘manufacturer’
a.	The new manufacturer name for the material
4.	‘date’
a.	The date the material was received
5.	‘owner’
a.	The owner of the material
6.	‘size’
a.	The particle size of the material in microns
7.	‘lot’
a.	The lot the material is a part of
8.	‘weight
a.	The remaining weight of the material
9.	‘open’
a.	Sets the condition of the packaging as ‘open’
10.	‘closed’
a.	Sets the condition of the packaging as ‘closed’
11.	‘ID’
a.	The job ID number of the material
12.	‘clone’
a.	Copies all properties (except barcode and history) of another material
iv.	If the barcode was not found, the program will prompt the user to enter a valid barcode.
f.	‘add’
i.	User types in a barcode or scans one into the program with a barcode scanner
ii.	Program will ask if this material is completely new, or is like another (meaning it has the exact same other properties, but a different barcode)
iii.	Type in ‘yes’ to clone the material, or any other key to enter data manually.
g.	‘save’
i.	Data in this program is not automatically written to the file and saved over the old loaded data. Program memory changes during use, but the file is manually written during saving and when the program is exited using the ‘exit’ command
ii.	Program will ask user if data should be saved, saving only if the user enters ‘yes’ or a ‘y’
h.	‘help’
i.	The help menu brings up a list of available commands for the program, as well as contact information for the main programmer
i.	‘recent’
i.	This command brings up a list of recently accessed materials (e.g. materials accessed from the search, edit or add functions, disabled as of V1.0, re-enabled as of V1.4
ii.	Results can be exported as a report
j.	‘history’
i.	User enters a barcode for a material and the function returns a list of its history
ii.	History consists of all past ‘weight’ information for that material as well as the date it was changed.
k.	‘jobs’
i.	Prompts the user if a new job is to be created, or an old one to be loaded
ii.	‘new’
1.	User enters the new job ID, the date it was started, then a list
l.	‘lot history’ or ‘lots’
i.	User is prompted to enter a barcode or a lot number
ii.	Based on the entry, program generates a combined total history for that lot of powder
iii.	Use can also enter a hidden command ‘report_all’ to generate lot histories for all powders in inventory and write them to a file
m.	‘monthly usage’, ‘monthly’ or ‘usage’
i.	Function runs automatically when called, no user input is required
ii.	‘usage’ generates a report of all the lot histories of powders that have had usage within the current month
1.	i.e. powders inventoried in February of the current year will show a usage history if the function is used during that time
n.	‘about’
i.	Brings up basic information about the program
o.	‘back’
i.	Returns from one of the sub-menus to the main menu
p.	‘exit’
i.	Exits from the main menu and asks if data should be saved

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

V1.1 7/23/2014
-	Program automatically reads and writes data upon starting and exiting
-	No longer requires user input unless there is a problem
-	Update menu heading
-	Adjusted ‘size’ field to accommodate 10 characters, instead of 5
-	Adjusted ‘print’ header to accommodate new ‘size’ length
-	Known bugs:
o	Program will still default to writing to ‘data.qcml’ using ‘save’ even if user specified a different source file during program start up
-	Bug fixes:
o	‘add’ and ‘edit’ will now appropriately check to make sure a barcode is or isn’t being used
o	‘recent’ has been temporarily disabled to prevent program crashing
o	Fixed potential bugs that could have resulted in runtime errors


V1.2 7/25/2014
-	Updated menu heading
-	Updated help menu
-	New functionality!
o	The program now keeps a history of all the past weights for a material, as well as the date they were changed
-	New function!
o	‘history’
o	Enter the barcode of the material whose history you would like
-	New function!
o	‘edit’  ‘clone’
o	Asks for the barcode of the material to be edited, then the barcode of another material
o	Copies all the properties (except barcode) from the 2nd material to the 1st
-	New function!
o	‘find’
o	Prints out a list of all the materials matching a single criteria
-	Updated ‘search’ function
o	Prevented possible usage bugs
o	Improved performance
-	Update ‘edit’ function
o	Will now show you the current value of the field you are about to change

V1.3 8/1/2014
-	Bug fixes:
o	Fixed ‘add’ to correctly initialize the history for new entries
o	Fixed a bug where the program would add extra entry at the end when reading in history data
o	Fixed a bug where current weight of all previously cloned materials would also be added to the history of a new material being added and cloned
-	Updated menu heading
-	Changed units to ‘kg’ instead of ‘lbs’
-	Known bugs:
o	When a new material is added and it is copied from another, the weight is copied as well, creating a single entry of a false history

V1.4 8/18/2014
-	New function!
o	‘report’
o	Asks the user if he or she would like to generate a report and of what
o	Writes that data to a .txt file and opens it in Microsoft Office Word for printing
-	Functional change: when the properties of one material are copied to another, weight will not be included
o	‘Add  (is this material like any other?) yes’ will now prompt the user to input the correct weight right away
o	‘Edit  clone’ will now prompt the user to enter the correct weight
-	Update ‘recent’ function
o	‘recent’ is back!
o	Changed functionality: recent now only shows recently changed materials
	Only materials edited in ‘add’ or ‘edit’ will be added
-	Updated menu heading
-	Slight performance improvements to:
o	‘edit’
o	‘recent’
-	Program now writes ‘data.qcml’ to intelligently include spaces for proper formatting
o	Easier to create a report
-	Bug fixes:
o	Removed false history that is created when a new material is added and copied
o	‘Add  (is this material like any other?)’ will now only proceed if an answer is given as ‘y’, ‘yes’, ‘n’, or ‘no’

V1.5 8/28/2014
-	‘report’ doesn’t ask for a yes or no answer anymore before giving the option to generate a report
-	The main menu now gives a list of commands before the user is prompted to enter one
-	Updated ‘help’ menu
-	Update menu heading
-	All functions have been removed from ‘command_center’ and now have their own proper separate function names titled ‘cc_<function name>’
o	Ex. Instead of create a loop when ‘edit’ is entered, command center now calls a function called ‘cc_edit’ and passes the list of materials to it.
o	No functional changes otherwise
o	In the future, will allow these functions to talk to each other directly
-	New function!
o	‘jobs’
o	User has the choice to either create a new job or view existing ones
o	In a new job, the job’s starting date is entered, as well as a list of the associated materials. A preview of the weight and list of materials is shown during entry.
o	When viewing or editing an existing job, user has a choice to either finish the job, add more materials or simply view the info for the job
o	Adding more materials continues the job from where it was started, finishing it marks the end date and calculates the ending weight based on the amount of material removed
-	Bug fixes
o	‘find’ now properly resets after each search, before it accumulated all the previous search results as well
o	When typing in ‘back’ during a new material addition in ‘add,’ the new material addition is properly canceled
-	Functional change: whenever a date is asked for, entering ‘today’ will put in the current date

V1.6 11/12/2014
-	Added functionality: ‘find’ can now generate a report of the results after searching
-	Added functionality: ‘find’ can now refine search results or start a brand new search and exit directly to main menu while in the middle of a search
-	Updated history under ‘reports’ to provide clearer, more usable reports
o	Now gives the material name, manufacturer and owner, in addition to the barcode and weight/date history
-	Updated menu heading
-	Bug fixes:
o	‘find’ did not have the option to search by weight correctly
o	Updated ‘search’ to use ‘kg’ instead of displaying ‘lbs’

 V1.7 12/3/2014
-	New function! ‘lot history’ or ‘lots’ provides a weight history for an entire lot of powder
o	User enters either the barcode of a member of a lot or the lot# directly to generate the report
o	Lot histories are not saved and are rebuilt each time one is desired
-	Added functionality: ‘lot history’ can also generate a full report of all lots in memory with a hidden command ‘report_all’
-	Added functionality: ‘find’ can now edit a group of materials after the desired list has been generated
o	Essentially allows for a ‘mass edit’ feature
-	Updated menu heading

 V1.8 1/7/2015
-	New function! ‘usage’ or ‘monthly’ provides a powder usage history for the current month
o	User enters ‘usage’ at the main menu and usage is automatically calculated and exported as a report, similar to ‘lot history’
o	Currently only works for the current, cannot be done for a month in the past
-	Changed the ‘recent’ function to only clear if 500 entries are made
-	Added functionality: editing a group of materials in ‘find’ will now add them to ‘recent’
-	Added functionality: listing results in ‘find’ also gives the total weight without having to write a report every time
-	Added functionality: searching for materials in ‘find’ will automatically search for partial matches if no complete matches are found
o	Partial matches are only searched for during new searches
o	Refining a search will not seek partial matches
o	A partial match is one where the user input is found as fragment of the actual term, or vice versa.
o	Eg. Searching for ‘steel’ would return partial matches for ‘stainless_steel’ ‘maraging_steel’ and ‘chrome_steel’
o	Eg. Searching for ‘nickel_iron’ would return partial matches for ‘nickel’ and ‘iron,’ but NOT ‘nickel_steel’ or ‘nickel_chrome’
o	One of the terms must appear COMPLETELY in the other to return match
-	Updated help menu
-	Updated menu heading
-	Re-enabled hidden experimental function, used for debugging only
o	Accessed via command ‘beta’
-	Bug fixes:
o	Whenever a date is entered by the user, the program will thoroughly check to be sure it is a valid date in the proper format
o	The program will not incorrectly add a material to ‘recent’ if the edit never went through
V1.8.1 2/6/2015
-	‘search’ now displays its results in a neater table format
-	Fixed a bug where inventorying a new item and using it in the same month would report no usage
V1.8.2 2/17/2015
-	Updated menu heading
-	Added functionality: ‘monthly usage’ now prints out the report on the screen as well as writing it to the file
-	Added functionality: ‘recent’ now gives a preview of the total weight, instead of having the print the report to see it
-	Added functionality: ‘monthly usage’ now tallies up power gained and powder lost in the monthly report, instead of giving just net usage
-	Added functionality: whenever a number is entered for a weight, the program checks to ensure it is an actual number before continuing, preventing commands or nonsensical entries accidentally being entered as weights
-	Writing any report now puts the written text report in a folder called ‘reports,’ instead of putting them directly in the program’s files
V1.8.3 3/4/2015
-	Added functionality: ‘search’ now also adds up the total weight of the powder containers being scanned
o	Now only prompts the user to enter a barcode once at the start, instead of each time.
-	Updated menu heading

V1.8.4 4/16/2015
-	When a contained of powder is edited for weight (meaning the weight has changed) the program will now also automatically mark it as being opened
-	Added a hidden command at the main menu “fix_all”
o	Runs through the list of powder in inventory
o	Any entry with more than 1 instance of history (i.e the weight has ever changed) will be set to ‘1’ or be marked as opened
o	Displays the number of entries changed
-	Updated menu heading
