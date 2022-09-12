/*
 * File: Directory.h
 * ----------
 * This interface exports several commands
 */

#pragma once

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <queue>
# include <set>
# include <stack>
# include <string>
# include <vector>

# ifdef _WIN32
# include <direct.h>
# include <io.h>
# else
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <memory.h>
# include <dirent.h>
# endif

# include "FCB.h"

 /*
 * Class: Directory
 * ----------------------------
 * The class Directory
 */

class Directory {

public:

	// VARIABLES

	// METHODS

	/*
	 * Constructor: Directory
	 * Usage: Directory(std::string rootPath);
	 * ---------------------------------------
	 * Initialises a new directory tree
	 */
	Directory(std::string rootPath);

	/*
	 * Destructor: ~Directory
	 * Usage: (usually implicit)
	 * -------------------------
	 * Frees any heap storage associated with this directory tree
	 */
	~Directory() = default;

	/*
	 * Method: cd
	 * Usage: Directory.cd(path);
	 * --------------------------
	 * Changes to directory; input should be in the form of path (both absolute and relative are acceptible)
	 */
	bool cd(std::string name, bool print = true, bool warning = true, bool mount = false); 

	/*
	 * Method: pwd
	 * Usage: Directory.pwd();
	 * -----------------------
	 * Prints the pathname for the working directory
	 */
	void pwd() const;

	/*
	 * Method: rwd
	 * Usage: Directory.rwd();
	 * -----------------------
	 * Returns the pathname for the working directory
	 */
	std::string rwd(bool withRoot = false);

	/*
	 * Method: ls
	 * Usage: Directory.ls();
	 * ----------------------
	 * Lists contents of current directory; change the second argument for more details
	 */
	void ls(bool detail = false) const;

	/*
	 * Method: Mkdir
	 * Usage: Directory.Mkdir(name);
	 * -----------------------------
	 * Creates a new directory.
	 */
	bool Mkdir(std::string name, bool warning = true);
	
	/*
	 * Method: Rmdir
	 * Usage: Directory.Rmdir(name);
	 * -----------------------------
	 * Removes a directory, which could be either empty or not
	 */
	bool Rmdir(std::string name, bool warning = true);
	
	/*
	 * Method: rm
	 * Usage: Directory.rm(name);
	 * --------------------------
	 * Deletes a file
	 */
	bool rm(std::string name);

	/*
	 * Method: mv
	 * Usage: Directory.mv(src, dest, dest_loc);
	 * -----------------------------------------
	 * Renames or moves (or copys) file(s) or directories. As for copying, state cp as true
	 */
	bool mv(std::string src, std::string dest, int dest_loc, bool cp = false, bool secondary = false);

	/*
	 * Method: find
	 * Usage: Directory.find(name);
	 * ----------------------------
	 * Finds for file(s) or directories matching a provided pattern.
	 */
	bool find(std::string name) const;

	/*
	 * Method: serialise
	 * Usage: Directory.serialise(path);
	 * ---------------------------------
	 * Serialises the directory tree from text files
	 */
	void serialise(std::string path);

	/*
	 * Method: deserialise
	 * Usage: Directory.deserialise(path);
	 * -----------------------------------
	 * Deserialise the directory tree and save it as text files
	 */
	void deserialise(std::string path);

	/*
	 * Method: mount
	 * Usage: Directory.mount(path);
	 * -----------------------------
	 * Mounts an outside directory
	 */
	bool mount(std::string path, bool secondary = false);
	
	/*
	 * Method: umount
	 * Usage: Directory.umount(mId);
	 * -----------------------------
	 * Inverse operation of mounting
	 */
	bool umount(int mId);
	
	/*
	 * Method: lsMount
	 * Usage: Directory.lsMount();
	 * ---------------------------
	 * Lists all mount points
	 */
	void lsMount() const;

	/*
	 * Method: setMount
	 * Usage: Directory.setMount(mId);
	 * -------------------------------
	 * Sets working directory as a mounted point (equivalent to cd(..., mount = true))
	 */
	bool setMount(int mId, bool print = true);

	/*
	 * Method: searchM
	 * Usage: Directory.searchM(path, ...);
	 * ------------------------------------
	 * Searches for mounted points that potentially has an higher order than the path 
	 * input (must be absolute path), and provide a string and an integer which could
	 * be used as the second and the third argument of method mv()
	 */
	bool searchM(std::string path, std::string& dest, int& result_loc);

	/*
	 * Method: touch
	 * Usage: Directory.touch(filename);
	 * ---------------------------------
	 * Creates new empty files
	 */

	bool touch(std::string name);

	/*
	 * Method: cat
	 * Usage: Directory.cat(filename);
	 * -------------------------------
	 * Displays file¡¯s contents
	 */
	bool cat(std::string name) const;

	/*
	 * Method: open
	 * Usage: Directory.open(filename);
	 * -------------------------------
	 * Opens a file for some possible operations
	 */
	bool open(std::string name);

	/*
	 * Method: close
	 * Usage: Directory.close();
	 * -------------------------
	 * Closes an opened file that is currently focused on
	 */
	bool close();

	/*
	 * Method: switchObj
	 * Usage: Directory.switchObj(ID);
	 * -------------------------------
	 * Switches to another open file, use this method togethor with lsOpen
	 */
	bool switchObj(int ID);

	/*
	 * Method: lsOpen
	 * Usage: Directory.lsOpen();
	 * --------------------------
	 * Lists all open files and their ids; provides information for switchObj()
	 */
	void lsOpen() const;

	/*
	 * Method: get
	 * Usage: Directory.get(size);
	 * ---------------------------
	 * Gets string of given size from currently focused file; use this method togethor 
	 * with repos(); change the argument end to true to count the off from end (off 
	 * should be negative), this is also true for the next two methods (put() and repos())
	 */
	bool get(int size, bool end = false);

	/*
	 * Method: put
	 * Usage: Directory.put(str);
	 * --------------------------
	 * Writes given string into currently focused file; input "replace = false" to insert
	 */
	bool put(std::string str, bool end = false, bool replace = true);

	/*
	 * Method: repos
	 * Usage: Directory.repos(ptr_type, off);
	 * --------------------------------------
	 * Changes the position of get/put pointers: 0(get), 1(put)
	 */
	bool repos(int ptr_type, std::streamoff off, bool end = false);

	/*
	 * Method: trunc
	 * Usage: Directory.trunc();
	 * -------------------------
	 * Truncates current file
	 */
	bool trunc();

	/*
	 * Method: newUser
	 * Usage: Directory.newUser(userId, name);
	 * ---------------------------------------
	 * Updates user table
	 */
	void newUser(int userId, std::string name);

	/*
	 * Method: renameUser
	 * Usage: Directory.cat(userId, name);
	 * -----------------------------------
	 * Updates user table
	 */
	bool renameUser(int userId, std::string newName);

	/*
	 * Method: switchUser
	 * Usage: Directory.cat(userId);
	 * -----------------------------
	 * Updates user table
	 */
	bool switchUser(int userId);

	/*
	 * Method: deleteUser
	 * Usage: Directory.cat(userId);
	 * -----------------------------
	 * Updates user table
	 */
	bool deleteUser(int userId);

	/*
	 * Method: setMode
	 * Usage: Directory.setMode(name, category, newValue);
	 * ---------------------------------------------------
	 * Sets the access mode of an object;
	 * Categories: 0(RUSR), 1(WUSR), 2(XUSR),
	 *			   3(RGRP), 4(WGRP), 5(XGRP),
	 *			   6(ROTH), 7(WOTH), 8(XOTH)
	 */
	bool setMode(std::string name, int category, char newValue);

	/*
	 * Method: defragment
	 * Usage: Directory.defragment();
	 * ------------------------------
	 * Defragments the storage
	 */
	void defragment(bool print = true);

private:

	// PRIVATE VARIABLES

	std::string rootPath;

	Folder* root;

	Folder* current;

	std::string WD;

	int userId;

	std::stack<Folder*> history;

	std::pair<int, std::set<int>> nodeIdPool;

	std::map<int, Folder*> mountPoints;

	std::pair<int, int> MnodeIdPool; // first for all affiliated objects; second for mount points

	bool mountmode;

	int current_mountpoint;

	std::map<int, std::string> userTable;

	std::vector<File*> openFiles;

	File* currentObj;

	std::pair<std::map<int, File*>, std::set<int>> storageMap;

	unsigned int unit_block_size; // would be initialised as 20 bytes

	unsigned int storage_limit; // would be initialised as 100 Mb divided by 20 byte

	// PRIVATE METHODS

	/*
	 * Private Method: wdfR
	 * --------------------
	 * Returns working directory without the part before the root
	 */
	std::string wdfR() const; 

	std::string wdfR(std::string wd) const;

	/*
	 * Private Method: strSplit_v
	 * --------------------------
	 * Splits a string into a vector with given delimiter
	 */
	void strSplit_v(const std::string& str, std::vector<std::string>& vec, const std::string& delim) const;
	
	/*
	 * Private Method: strSplit_q
	 * --------------------------
	 * Splits a string into a queue with given delimiter
	 */
	void strSplit_q(const std::string& str, std::queue<std::string>& vec, const std::string& delim) const;

	/*
	 * Private Method: filecp
	 * ----------------------
	 * Copies files
	 */
	bool filecp(std::string src, std::string dest);

	/*
	 * Private Method: unit_se
	 * -----------------------
	 * Unit operation of deserialisation used for recursion
	 */
	void unit_se(std::string& str1, std::string& str2);

};
