/*
 * File: FCB.cpp
 * ----------
 * This file implements file control block
 */

# include "FCB.h"

//////////////////////////////////////// FCB ////////////////////////////////////////

FCB::FCB(std::string filename, std::string currentPath, int& nodeId, int userId, int location)
{
	name = filename;
	this->nodeId = nodeId;
	if (nodeId == 0)
	{
		path = currentPath;
	}
	else
	{
		path = currentPath + "/" + filename;
	}
	nodeId++;
	sz = 0;
	setTime(access.lastModification);
	setTime(access.lastAccess);
	access.creator = userId;
	access.lastEditor = userId;
	access.accessby = userId;
	access.visibility = true;
# ifdef _WIN32
	strcpy_s(access.mode, "111111111");
# elif __APPLE__
	strlcpy(access.mode, "111111111", 9);
# else
	strcpy(access.mode, "111111111");
# endif
	this->location = location;
}

FCB::FCB(const FCB& old, std::string currentPath, int& nodeId, int location)
{
	name = old.name;
	this->nodeId = nodeId;
	nodeId++;
	path = currentPath + "/" + name;
	sz = old.sz;
	access = old.access;

	this->location = location;
}

bool FCB::setMode(int category, char newValue)
{
	access.mode[category] = newValue;
	return true;
}

void FCB::setTime(time_t& lastTime)
{
	lastTime = time(0);
}


std::string FCB::timetostr(time_t& lastTime)
{
	std::string str;
	char buf[26];
# ifdef _WIN32
	ctime_s(buf, sizeof(buf), &lastTime);
# else
	tm* info = localtime(&lastTime);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info);
# endif
	str = buf;
	return str;
}

//////////////////////////////////////// File ///////////////////////////////////////

File::File(std::string filename, std::string currentPath, int& nodeId, int userId, int location) : FCB(filename, currentPath, nodeId, userId, location)
{
	isFolder = false;

	getptr = putptr = 0;

	std::string::size_type pos = filename.find_last_of('.');
	type = filename.substr(pos + 1);
}

File::File(const File& old, std::string currentPath, int& nodeId, int location) : FCB(old, currentPath, nodeId, location)
{
	isFolder = false;

	getptr = putptr = 0;

	type = old.type;
}

int File::size()
{
	std::ifstream in;
	int size;

	in.open(path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
	
	size = (unsigned int) in.tellg();

	in.close();

	sz = size;
	return sz;
}

bool File::find(std::string name, std::vector<FCB*>& result, bool further, bool exac,
	            int mode) const
{
	return false;
}

bool File::get(std::string& str, int size, bool end)
{
	std::ifstream fi;
	fi.open(path.c_str(), std::ios::binary | std::ios::in);
	
	if (!fi.is_open())
	{
		return false;
	}
	if (!end)
	{
		fi.seekg(getptr, std::ios::beg);
	}
	else
	{
		fi.seekg(getptr, std::ios::end);
	}

	char* s = new char[size];

	fi.read(s, size);
	str = std::string(s, size);
	delete[] s;

	fi.close();
	getptr = getptr + size;
	if (getptr > size)
	{
		getptr = size;
	}
	return true;
}

bool File::put(std::string& str, int size, bool end, bool replace)
{
	std::fstream f;
	f.open(path.c_str(), std::ios::binary | std::ios::out | std::ios::in);
	
	int subsize = this->size() - (int) putptr;
	char* s = new char[subsize];
	if (!replace)
	{
		f.seekg(putptr, std::ios::beg);
		f.read(s, subsize);
	}

	f.seekp(putptr, std::ios::beg);
	f.write(str.c_str(), size);

	if (!replace)
	{
		f.write(s, subsize);
	}

	f.close();
	putptr = putptr + size;
	if (putptr > size)
	{
		putptr = size;
	}
	return true;
}

bool File::repos(int ptr_type, std::streamoff off, bool end)
{
	int size = this->size();
	if (ptr_type == 0)
	{
		if (end)
		{
			getptr = size + off;
		}
		else
		{
			getptr = off;
		}
		if (getptr < 0 || getptr > size)
		{
			return false;
		}
		return true;
	}
	else if (ptr_type == 1)
	{

		if (end)
		{
			putptr = size + off;
		}
		else
		{
			putptr = off;
		}
		if (putptr < 0 || putptr > size)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool File::trunc()
{
	std::fstream tr;

	tr.open(path.c_str(), std::ios::binary | std::ios::trunc);
	if (!tr.is_open())
	{
		return false;
	}
	tr.close();
	return true;
}


//////////////////////////////////////// Folder /////////////////////////////////////

Folder::Folder(std::string filename, std::string currentPath, int& nodeId, int userId, int location) : FCB(filename, currentPath, nodeId, userId, location)
{
	isFolder = true;

	type = "folder";
}

Folder::Folder(const Folder& old, std::string currentPath, int& nodeId, int location) : FCB(old, currentPath, nodeId, location)
{
	isFolder = true;

	type = "folder";
}

Folder::~Folder()
{
	for (auto c : child)
	{
		delete c;
	}
}

int Folder::size()
{
	int size_ = 0;
	for (auto c : child)
	{
		size_ += c->size();
	}
	sz = size_;
	return sz;
}

int Folder::count() const
{
	return child.size();
}

bool Folder::find(std::string name, std::vector<FCB*>& result, bool further, bool exact,
				  int mode) const
{
	for (auto c : child)
	{
		if (further == true)
		{
			if (c->isFolder == true)
			{
				(*c).find(name, result, true);
			}
		}
		if (exact == false)
		{
			if (c->name.find(name) != std::string::npos)
			{
				if (mode == 0)
				{
					result.push_back(c);
					return true;
				}
				if (mode == 1 && c->isFolder == false)
				{
					result.push_back(c);
					return true;
				}
				if (mode == 2 && c->isFolder == true)
				{
					result.push_back(c);
					return true;
				}
			}
		}
		else
		{
			if (c->name == name)
			{
				if (mode == 0)
				{
					result.push_back(c);
					return true;
				}
				if (mode == 1 && c->isFolder == false)
				{
					result.push_back(c);
					return true;
				}
				if (mode == 2 && c->isFolder == true)
				{
					result.push_back(c);
					return true;
				}
			}
		}
	}
	return false;
}

void Folder::newChild(FCB* newFCB)
{
	child.push_back(newFCB);
}

bool Folder::removeChild(FCB* rFCB)
{
	std::vector<FCB*>::iterator itr;
	for (itr = child.begin(); itr != child.end(); itr++)
	{
		if ((*itr)->name == rFCB->name &&
			(*itr)->isFolder == rFCB->isFolder)
		{
			child.erase(itr);
			return true;
		}
	}
	return false;
}