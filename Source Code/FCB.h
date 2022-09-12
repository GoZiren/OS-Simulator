/*
 * File: FCB.h
 * ----------
 * This interface exports the template class file control block (for file and folder)
 */

#pragma once

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <ctime>
# include <cstring>

# include "AccessControl.h"

/*
 * Folder is declared here since FCB would have an attribute as Folder*
 */

class Folder; 

/*
* Abstract Class: FCB
* ----------------------------
* The class FCB (has two derived classes File and Folder)
*/

class FCB {

public:

	//ATTRIBUTES

	std::string name;

	std::string path;

	std::string type;

	bool isFolder;

	int nodeId; 

	int location; // whether is a temporary object from a mount point, and if is, of which one

	Folder* parentNode;

	Access access;

	// METHODS

	/*
	 * Constructor: FCB
	 * Usage: FCB(filename, currentPath, nodeId, userId, location);
	 * ------------------------------------------------------------
	 * Initializes a new file control block
	 */
	FCB(std::string filename, std::string currentPath, int& nodeId, int userId, int location);
	
	/*
	 * Copy Constructor: FCB
	 * Usage: FCB(old, currentPath, nodeId, location);
	 * -----------------------------------------------
	 * Initialises a new file control block from a existing one
	 */
	FCB(const FCB& old, std::string currentPath, int& nodeId, int location);

	/*
	 * Destructor: ~FCB
	 * Usage: (usually implicit) (virtual)
	 * -----------------------------------
	 * Frees any heap storage associated with this file control block.
	 */
	virtual ~FCB() = default;

	/*
	 * Method: setMode
	 * Usage: FCB.setMode(category, newValue);
	 * ---------------------------------------
	 * Sets the access mode of an object;
	 * Categories: 0(RUSR), 1(WUSR), 2(XUSR), 
	 *			   3(RGRP), 4(WGRP), 5(XGRP), 
	 *			   6(ROTH), 7(WOTH), 8(XOTH)
	 */
	bool setMode(int category, char newValue);

	/*
	 * Method: setTime
	 * Usage: FCB.setTime();
	 * ---------------------
	 * Sets the time of last edition / last access
	 */
	void setTime(time_t& lastTime);

	/*
	 * Method: timetostr
	 * Usage: FCB.Time();
	 * ------------------
	 * Converts the time of last edition / last access to string
	 */
	std::string timetostr(time_t& lastTime);

	/*
	 * Method: size
	 * Usage: (pure virtual)
	 * ---------------------
	 * None.
	 */
	virtual int size() = 0;

	/*
	 * Method: find
	 * Usage: (pure virtual)
	 * ---------------------
	 * None. 
	 */
	virtual bool find(std::string name, std::vector<FCB*>& result, bool further = false, bool exact = true, int mode = 0) const = 0;

protected:

	int sz;

private:

};

/*
 * Class: File
 * ----------------------------
 * The class file, inherit from the class FCB
 */

class File : public FCB {

public:

	// ATTRIBUTES

	std::streamoff getptr, putptr;

	// METHODS

	/*
	 * Constructor: File
	 * Usage: File(name, currentPath, nodeId, userId, location);
	 * ---------------------------------------------------------
	 * Initializes a new empty file
	 */
	File(std::string filename, std::string currentPath, int& nodeId, int userId, int location);

	/*
	 * Copy Constructor: File
	 * Usage: File(old, currentPath, nodeId, location);
	 * ------------------------------------------------
	 * Initializes a new file from a existing one
	 */
	File(const File& old, std::string currentPath, int& nodeId, int location);

	/*
	 * Destructor: ~File
	 * Usage: (usually implicit)
	 * -------------------------
	 * Frees any heap storage associated with this file
	 */
	~File() = default;

	/*
	 * Method: size
	 * Usage: size = File.size();
	 * --------------------------
	 * Returns the size of this file
	 */
	int size();

	/*
	 * Method: find
	 * Usage: (virtual)
	 * ----------------
	 * None.
	 */
	bool find(std::string name, std::vector<FCB*>& result, bool further = false, bool exact = true, int mode = 0) const;

	/*
	 * Method: get
	 * Usage: File.get(str, size);
	 * ---------------------------
	 * Gets string of given size from the file
	 */
	bool get(std::string& str, int size, bool end = false);

	/*
	 * Method: put
	 * Usage: File.put(str, size);
	 * ---------------------------
	 * Writes given string into the file
	 */
	bool put(std::string& str, int size, bool end = false, bool replace = true);

	/*
	 * Method: repos
	 * Usage: File.repos(ptr_type, off);
	 * ---------------------------------
	 * Changes the position of get/put pointers: 0(get), 1(put)
	 */
	bool repos(int ptr_type, std::streamoff off, bool end = false);

	/*
	 * Method: trunc
	 * Usage: File.trunc();
	 * ------------------------------
	 * Truncates the file
	 */
	bool trunc();

private:

};

/*
 * Class: Folder
 * ----------------------------
 * The class folder, inherit from the class FCB
 */

class Folder : public FCB {

public:

	// ATTRIBUTES

	std::vector<FCB*> child;

	// METHODS

	/*
	 * Constructor: Folder
	 * Usage: Folder(name, currentPath, nodeId, userId, location);
	 * -----------------------------------------------------------
	 * Initializes a new empty folder
	 */
	Folder(std::string name, std::string currentPath, int& nodeId, int userId, int location);

	/*
	 * Copy Constructor: Folder
	 * Usage: Folder(old, currentPath, nodeId, location);
	 * --------------------------------------------------
	 * Initializes a new Folder from a existing one
	 */
	Folder(const Folder& old, std::string currentPath, int& nodeId, int location);


	/*
	 * Destructor: ~Folder
	 * Usage: (usually implicit)
	 * -------------------------
	 * Frees any heap storage associated with this folder.
	 */
	~Folder();

	/*
	 * Method: size
	 * Usage: int size = Folder.size();
	 * --------------------------------
	 * Returns the amount of sizes of files under this folder.
	 */
	int size();

	/*
	 * Method: count
	 * Usage: int childCount = Folder.count();
	 * ---------------------------------------
	 * Returns the number of children under this folder.
	 */
	int count() const;

	/*
	 * Method: find
	 * Usage: find(std::string name, std::vector<FCB*>& result);
	 * ---------------------------------------------------------
	 * Finds corresponding children under the folder.
	 * Target: mode = 0 (both), mode = 1 (file), mode = 2 (folder)
	 */
	bool find(std::string name, std::vector<FCB*>& result, bool further = false, bool exact = true, int mode = 0) const;


	/*
	 * Method: newChild
	 * Usage: newChild(FCB* newFCB);
	 * -----------------------------
	 * Creates link to a new child.
	 */
	void newChild(FCB* newFCB);

	/*
	 * Method: removeChild
	 * Usage: removeChild(FCB* deletedFCB);
	 * ------------------------------------
	 * Removes link to a child.
	 */
	bool removeChild(FCB* rFCB);

private:

};