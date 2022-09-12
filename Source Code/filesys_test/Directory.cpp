/*
 * File: Directory.cpp
 * ----------
 * This file implements Directory
 */

# include "Directory.h"

 //////////////////////////////////////// PUBLIC ////////////////////////////////////////

Directory::Directory(std::string rootPath)
{
	userId = 0;
	nodeIdPool.first = 0;
# ifdef _WIN32
	MnodeIdPool.first = INT_MIN;
# else
	MnodeIdPool.first = INT8_MIN;
# endif
	MnodeIdPool.second = 0;

	mountmode = false;
	current_mountpoint = 0;

	root = new Folder("root", rootPath, nodeIdPool.first, userId, current_mountpoint);
	root->parentNode = root;
	current = root;
	this->rootPath = rootPath;
	WD = rootPath;

	unit_block_size = 20;
	storage_limit = 1000000 * 100 / unit_block_size;

	this->newUser(0, "default_user");
}

bool Directory::cd(std::string name, bool print, bool warning, bool mount)
{
	if (mount == true)
	{
		int i = atoi(name.c_str());
		if (i == 0)
		{
			if (warning != false)
			{
				std::cout << "cd: "<< name << ": Invalid mount point" << std::endl;
			}
			return false;
		}
		mountmode = true;
		current_mountpoint = i;
		current = mountPoints[-i];
		WD = current->path;
		if (print != false)
		{
			history.push(current);
			pwd();
		}
		current->setTime(current->access.lastAccess);
		current->access.accessby = this->userId;
		return true;
	}
	if (name == "-")
	{
		if (!history.empty())
		{
			history.pop();
			current = history.top();
			if (current->nodeId < 0)
			{
				mountmode = true;
				current_mountpoint = current->location;
			}
			else 
			{
				mountmode = false;
				current_mountpoint = 0;
			}

			WD = current->path;
			if (print != false)
			{
				pwd();
			}
			current->setTime(current->access.lastAccess);
			current->access.accessby = this->userId;
			return true;
		}
		else
		{
			if (warning != false)
			{
				std::cout << "cd: Cannot go back further" << std::endl;
			}
			return false;
		}
	}
	else
	{
		Folder* temp;
		temp = current;
		if (name[0] == '~')
		{
			if (mountmode == false)
			{
				if (warning != false)
				{
					std::cout << "cd: Invalid input with '~'" << std::endl;
				}
				return false;
			}
			current = mountPoints[-current_mountpoint];
			if (name.size() > 1)
			{
				name = name.substr(1);
			}
			else
			{
				WD = current->path;

				if (print != false)
				{
					history.push(current);
					pwd();
				}
				current->setTime(current->access.lastAccess);
				current->access.accessby = this->userId;
				return true;
			}
		}
		else if (name[0] == '/')
		{
			mountmode = false;
			current_mountpoint = 0;
			current = root;
			if (name.size() > 1)
			{
				name = name.substr(1);
			}
			else
			{
				WD = current->path;

				if (print != false)
				{
					history.push(current);
					pwd();
				}
				current->setTime(current->access.lastAccess);
				current->access.accessby = this->userId;
				return true;
			}
		}
		std::queue<std::string> strvec;
		strSplit_q(name, strvec, "/");
		while (!strvec.empty())
		{
			std::string s = strvec.front();
			strvec.pop();
			if (s == "..")
			{
				current = current->parentNode;
				continue;
			}
			std::vector<FCB*> vec;
			if (current->find(s, vec, false, true, 2))
			{
				current = (Folder*)vec[0];
			}
			else
			{
				if (warning != false)
				{
					std::cout << "cd: " << name << ": No such file or directory" << std::endl;
				}
				current = temp;
				return false;
			}
		}
	}
	WD = current->path;
	if (print != false)
	{
		history.push(current);
		pwd();
	}
	current->setTime(current->access.lastAccess);
	current->access.accessby = this->userId;
	return true;
}

void Directory::pwd() const
{
	std::cout << wdfR() << std::endl;
}

std::string Directory::rwd(bool withRoot)
{
	std::string str;
	if (withRoot)
	{
		str = WD;
	}
	else
	{
		str = wdfR();
	}
	return str;
}

void Directory::ls(bool detail) const
{
	if (detail)
	{
		for (auto c : current->child)
		{
			int i = c->name.find_last_of('.');
			std::string name = c->name.substr(0, i);
			std::cout << name
				<< "\t type: " << c->type << "\t"
				<< "\t size: " << c->size() << "\t";
			if (c->isFolder == true)
			{
				Folder* ptr = dynamic_cast<Folder*>(c);
				std::cout << "\t count: " << ptr->count() << "\n";
			}
			else
			{
				std::cout << std::endl;
			}
			std::cout << "\t last mod: "
				<< " by userId " << c->access.lastEditor << "\t"
				<< " at " << c->timetostr(c->access.lastModification)
				<< "\t last acc: "
				<< " by userId " << c->access.accessby << "\t"
				<< " at " << c->timetostr(c->access.lastAccess) << "\n";
		}
		std::cout << "\n";
	}
	else
	{
		for (auto c : current->child)
		{
			std::cout << c->name << "\t";
		}
		std::cout << "\n";
	}
}

bool Directory::Mkdir(std::string name, bool warning)
{
	std::vector<FCB*> vec;
	if (!current->find(name, vec, false, true, 2))
	{
		std::string str = WD + "/" + name;
# ifdef _WIN32
		if (_mkdir(str.c_str()) == -1)
# else
		if (mkdir(str.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
# endif
		{
			// std::cout << "mkdir failed" << std::endl;
			return false;
		}
		else
		{
			Folder* newdir = new Folder(name, WD, nodeIdPool.first, userId, current_mountpoint);

			newdir->parentNode = current;
			current->newChild(newdir);

			// std::cout << name << " is created" << std::endl;
			current->setTime(current->access.lastModification);
			return true;
		}
	}
	else
	{
		if (warning)
		{
			std::cout << "mkdir: cannot create directory '" << name << "': File exists" << std::endl;
		}
		return false;
	}
}

bool Directory::Rmdir(std::string name, bool warning)
{
	std::vector<FCB*> vec;
	if (current->find(name, vec, false, true, 2))
	{
		Folder* ptr = dynamic_cast<Folder*>(vec[0]);
		cd(name, false);
		for (auto c : ptr->child)
		{
			if (c->isFolder == true)
			{
				Rmdir(c->name);
			}
			else
			{
				rm(c->name);
			}
		}
		cd("..", false);

		std::string str = WD + "/" + name;
# ifdef _WIN32
		if (_rmdir(str.c_str()) == -1)
# else
		if (rmdir(str.c_str()) == -1)
# endif
		{
			// std::cout << "rmdir failed" << std::endl;
			return false;
		}
		else
		{
			current->removeChild(vec[0]);
			nodeIdPool.second.insert(vec[0]->nodeId);
			delete vec[0];

			// std::cout << name << " is removed" << std::endl;
			current->setTime(current->access.lastModification);
			current->access.lastEditor = this->userId;
			return true;
		}
	}
	else
	{
		if (warning)
		{
			std::cout << "rmdir: failed to remove '" << name << "': No such file or directory" << std::endl;
		}
		return false;
	}
}

bool Directory::rm(std::string name)
{
	std::vector<FCB*> vec;
	if (current->find(name, vec, false, true, 1))
	{
		current->removeChild(vec[0]);
		nodeIdPool.second.insert(vec[0]->nodeId);

		if (remove(vec[0]->path.c_str()) == -1)
		{
			// std::cout << "remove failed" << std::endl;
			return false;
		}

		std::map<int, File*>::iterator itr;
		for (itr = storageMap.first.begin(); itr != storageMap.first.end();)
		{
			if ((*itr).second->nodeId == vec[0]->nodeId)
			{
				storageMap.second.insert(itr->first);
				storageMap.first.erase(itr++);
			}
			else
			{
				itr++;
			}
		}

		delete vec[0];

		current->setTime(current->access.lastModification);
		current->access.lastEditor = this->userId;
		return true;
	}
	else
	{
		std::cout << "rm: cannot remove '"<< name <<"': No such file or directory" << std::endl;
		return false;
	}
}

bool Directory::mv(std::string src, std::string dest, int dest_loc, bool cp, bool secondary)
{
	int temp_m = current_mountpoint;
	Folder* temp = current;
	std::vector<FCB*> vec;

	if (!secondary)
	{
		if (src == "~")
		{
			std::cout << "mv: cannot stat '" << src << "': Cannot use mounted point as a source" << std::endl;
		}
		else if (src[0] == '~')
		{
			src = src.substr(1);
		}

		if (src.find('/') != std::string::npos)
		{
			std::string src0, src1;
			std::string::size_type pos = src.find_last_of('/');
			src0 = src.substr(0, pos);
			src1 = src.substr(pos + 1);
			src = src1;
			cd(src0, false);
		}

		if (!current->find(src, vec, false, true))
		{
			std::cout << "mv: cannot stat '" << src << "': No such file or directory" << std::endl;
			if (temp_m != 0)
			{
				setMount(temp_m, false);
			}
			cd(wdfR(temp->path), false);
			return false;
		}
		if (temp_m != 0)
		{
			setMount(temp_m, false);
		}
		cd(wdfR(temp->path), false);

		if (dest_loc != 0)
		{
			setMount(dest_loc, false);
		}
		if (!cd(dest, false, false))
		{
			if (dest.find('/') != std::string::npos)
			{
				std::string dest0, dest1;
				std::string::size_type pos = dest.find_last_of('/');
				dest0 = dest.substr(0, pos);
				dest1 = dest.substr(pos + 1);
				if (cd(dest0, false, false) && !cp)	// rename then move
				{
					dest = dest0;
					src = dest1;
					vec[0]->name = dest1;
				}
				else
				{
					std::cout << "mv: cannot move '" << src << "' to '" << wdfR(temp->path) + '/' + dest << "': No such file or directory" << std::endl;
					if (temp_m != 0)
					{
						setMount(temp_m, false);
					}
					cd(wdfR(temp->path), false);
					return false;
				}
			}
			else // rename
			{
				vec[0]->name = dest;
				vec[0]->path = vec[0]->parentNode->path + "/" + dest;
				if (temp_m != 0)
				{
					setMount(temp_m, false);
				}
				cd(wdfR(temp->path), false);
				return true;
			}
		}
		if (dest[0] != '/')
		{
			dest = wdfR(WD);
		}
	}
	else
	{
		current->find(src, vec, false, true);
		if (dest_loc != 0)
		{
			setMount(dest_loc, false);
		}
		cd(dest, false);
	}

	if (vec[0]->isFolder)
	{
		Folder* ptr = dynamic_cast<Folder*>(vec[0]);
		if (Mkdir(ptr->name, false))
		{
			std::vector<FCB*> _vec;
			current->find(ptr->name, _vec, false, true, 2);
			_vec[0]->access = ptr->access;
		}

		if (ptr->location != 0)
		{
			setMount(ptr->location, false);
		}
		cd(wdfR(ptr->path), false);

		std::string newSrc, newDest;
		for (auto c : ptr->child)
		{
			newSrc = c->name;
			if (dest != "/")
			{
				newDest = dest + "/" + ptr->name;
			}
			else
			{
				newDest = dest + ptr->name;
			}
			mv(newSrc, newDest, dest_loc, true, true);
		}
	}
	else
	{
		bool overwrite = false;
		std::vector<FCB*> _vec;
		if (current->find(src, _vec, false, true))
		{
			char O;
			std::cout << "File already exists, overwrite? (Y/N)" << std::endl;
			while (true)
			{
				std::cin >> O;
				if (toupper(O) == 'Y')
				{
					overwrite = true;
					break;
				}
				else if (toupper(O) == 'N')
				{
					if (temp_m != 0)
					{
						setMount(temp_m, false);
					}
					cd(wdfR(temp->path), false);
					return false;
				}
				else
				{
					std::cout << "Invalid input" << std::endl;
				}
			}
		}
		std::string newSrc, newDest;
		newSrc = vec[0]->path;
		newDest = current->path + "/" + vec[0]->name;

		if (filecp(newSrc, newDest) && !overwrite)
		{
			File* ptr = dynamic_cast<File*>(vec[0]);
			File* newFile = new File(*ptr, current->path, nodeIdPool.first, current_mountpoint);

			if (dest_loc == 0)
			{
				int f_size = newFile->size();
				int i = (f_size / unit_block_size) + 1;

				if (storageMap.first.size() + i > storage_limit)
				{
					rm(newFile->name);
					std::cout << "mv: cannot move '" << src << "' to '" << wdfR(temp->path) + '/' + dest << "': No more storage" << std::endl;
					if (temp_m != 0)
					{
						setMount(temp_m, false);
					}
					cd(wdfR(temp->path), false);
					return false;
				}

				for (int j = 0; j < i; j++)
				{
					storageMap.first.insert(std::pair<int, File*>(storageMap.first.size(), newFile));
				}
			}
			newFile->parentNode = current;
			current->newChild(newFile);
		}
	}
	
	if (temp_m != 0)
	{
		setMount(temp_m, false);
	}
	cd(wdfR(temp->path), false);
	if (!cp)
	{
		if (vec[0]->isFolder)
		{
			Rmdir(vec[0]->name);
		}
		else
		{
			rm(vec[0]->name);
		}
	}
	return true;
}

bool Directory::find(std::string name) const
{
	std::vector<FCB*> result;
	if (current->find(name, result, true))
	{
		for (auto r : result)
		{
			std::cout << r->name << "\t" << wdfR(r->path) << "\n";
		}
		return true;
	}
	else
	{
		std::cout << "find: cannot find " << name << ": No matching file or directory" << std::endl;
		return false;
	}
}

void Directory::serialise(std::string path)
{
	std::ofstream fo1, fo2;
	std::string str1, str2, path1, path2;

	cd("/", false);

	path1 = path + "_struct.txt";
	path2 = path + "_detail.txt";
	fo1.open(path1.c_str(), std::ios::out | std::ios::trunc);
	fo2.open(path2.c_str(), std::ios::out | std::ios::trunc);

	unit_se(str1, str2);
	fo1 << str1;
	fo2 << str2;

	fo1.close();
	fo2.close();

	if (str2.size() == 0)
	{
		std::cout << "EMPTY" << std::endl;
		remove("_struct.txt");
		remove("_detail.txt");
# ifdef _WIN32
		int rm = _rmdir(rootPath.c_str());
# else
		int rm = rmdir(rootPath.c_str());
# endif
	}
	else
	{
		std::cout << str1 << std::endl;
		std::cout << str2 << std::endl;
	}
}

void Directory::deserialise(std::string path)
{
	std::ifstream fi;
	std::string line;

	path = path + "_detail.txt";
	fi.open(path.c_str(), std::ios::in);

	std::vector<std::string> strvec;

	while (!fi.eof()) // this is actually of no use, since there is an empty line at the bottom, we must break before eof()
	{
		getline(fi, line);
		if (line.empty())
		{
			break;
		}
		strSplit_v(line, strvec, ",");

		int nodeId, creator, lastEditor, accessby;
		bool isFolder, visibility;
		std::string name, _path, mode;
		std::time_t lastMod, lastAcc;
		std::string::size_type pos;
		
		nodeId = stoi(strvec[0]);
		pos = strvec[1].find_last_of('/');
		name = strvec[1].substr(pos + 1);
		_path = strvec[1].substr(0, pos);
		isFolder = stoi(strvec[2]);
		creator = stoi(strvec[3]);
		lastEditor = stoi(strvec[4]);
		accessby = stoi(strvec[5]);
		lastMod = stol(strvec[6]);
		lastAcc = stol(strvec[7]);
		visibility = stoi(strvec[8]);
		mode = strvec[9];

		if (isFolder)
		{
			cd(wdfR(_path), false);
			Folder* newFolder = new Folder(name, _path, nodeId, creator, current_mountpoint);
			newFolder->parentNode = current;
			current->newChild(newFolder);
			newFolder->access.lastEditor = lastEditor;
			newFolder->access.accessby = accessby;
			newFolder->access.lastModification = lastMod;
			newFolder->access.lastAccess = lastAcc;
			newFolder->access.visibility = visibility;
			for (int i = 0; i < 9; i++)
			{
				newFolder->access.mode[i] = mode[i];
			}
		}
		else
		{
			cd(wdfR(_path), false);
			File* newFile = new File(name, _path, nodeId, creator, current_mountpoint);			
			int f_size = newFile->size();
			int i = (f_size / unit_block_size) + 1;
			for (int j = 0; j < i; j++)
			{
				storageMap.first.insert(std::pair<int, File*>(storageMap.first.size(), newFile));
			}
			newFile->parentNode = current;
			current->newChild(newFile);
			newFile->access.lastEditor = lastEditor;
			newFile->access.accessby = accessby;
			newFile->access.lastModification = lastMod;
			newFile->access.lastAccess = lastAcc;
			newFile->access.visibility = visibility;
			for (int i = 0; i < 9; i++)
			{
				newFile->access.mode[i] = mode[i];
			}
		}
		std::vector<std::string>().swap(strvec);
	}
	fi.close();
	cd("/");
}

bool Directory::mount(std::string path, bool secondary)
{
	if (!secondary)
	{
		std::vector<std::string> vec2, vec3;

		strSplit_v(path, vec2, "/");
		strSplit_v(rootPath, vec3, "/");
		if (vec2.size() > vec3.size())
		{
			for (unsigned int i = 0; i < vec3.size(); ++i)
			{
				if (vec2[i] != vec3[i])
				{
					break;
				}
				if (i == vec3.size() - 1)
				{
					std::cout << "mount: cannot mount \"" << path << "\": Directory is under the root" << std::endl;
					return false;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < vec2.size(); ++i)
			{
				if (vec2[i] != vec3[i])
				{
					break;
				}
				if (i == vec2.size() - 1)
				{
					std::cout << "mount: cannot mount \"" << path << "\": Directory is the root or at a higher level of the root" << std::endl;
					return false;
				}
			}
		}

		if (!mountPoints.empty())
		{
			std::string mountedPath;
			std::vector<std::string> vec0, vec1;

			strSplit_v(path, vec0, "/");

			for (auto c : mountPoints)
			{
				mountedPath = c.second->path;
				strSplit_v(mountedPath, vec1, "/");

				for (unsigned int i = 0; i < vec1.size(); ++i)
				{
					if (vec0[i] != vec1[i])
					{
						break;
					}
					if (i == vec1.size() - 1)
					{
						std::cout << "mount: cannot mount \"" << path << "\": A directory of higher order has already been mounted" << std::endl;
						return false;
					}
				}
				std::vector<std::string>().swap(vec1);
			}
		}

# ifdef _WIN32
		_finddata_t data;
		intptr_t handle = _findfirst(path.c_str(), &data);
		if (handle == -1)
# else
		DIR* dir = opendir(path.c_str());
		if (dir == NULL)
# endif
		{
			std::cout << "mount: cannot mount \"" << path << "\": No such directory" << std::endl;
			return false;
		}
	}

	int temp_m = current_mountpoint;
	Folder* temp = current;

	if (!secondary)
	{
		int id = MnodeIdPool.second - 1;
		MnodeIdPool.second--;

		std::string::size_type pos;
		std::string _path, name;

		pos = path.find_last_of('/');
		_path = path.substr(0, pos);
		name = path.substr(pos + 1);

		Folder* newFolder = new Folder(name, _path, id, userId, -id);
		newFolder->parentNode = newFolder;
		id--;
		mountPoints.insert(std::pair<int, Folder*>(id, newFolder));
		setMount(-id, false);
	}

	std::string subDir = path + "/*";

# ifdef _WIN32
	_finddata_t data;
	intptr_t handle = _findfirst(subDir.c_str(), &data);
	while (_findnext(handle, &data) == 0)
	{
		if (data.attrib & _A_SUBDIR)
		{
# else
	DIR* dir = opendir(path.c_str());
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR) 
		{
# endif
			std::string name;
# ifdef _WIN32
			name = data.name;
# else
			name = entry->d_name;
# endif
			if (strcmp(name.c_str(), ".") == 0 || strcmp(name.c_str(), "..") == 0)
			{
				continue;
			}
			std::string newPath = path + "/" + name;

			Folder* newF = new Folder(name, path, MnodeIdPool.first, userId, current_mountpoint);
			newF->parentNode = current;
			current->newChild(newF);

			cd(name, false);
			mount(newPath, true);
			cd("..", false);
		}
		else
		{
			std::string name;
# ifdef _WIN32
			name = data.name;
# else
			name = entry->d_name;
# endif
			std::string newPath = path + "/" + name;
			File* newF = new File(name, path, MnodeIdPool.first, userId, current_mountpoint);
			newF->parentNode = current;
			current->newChild(newF);
		}
	}
# ifdef _WIN32
	_findclose(handle);
# else
	closedir(dir);
# endif
	if (!secondary)
	{
		std::cout << path << " has been mounted as point" << current_mountpoint << std::endl;

		if (temp_m != 0)
		{
			setMount(temp_m, false);
		}
		cd(wdfR(temp->path), false);
	}
	return true;
}

bool Directory::umount(int mId)
{
	if (mountPoints.count(-mId) == 0)
	{
		return false;
	}

	delete mountPoints[-mId];
	mountPoints.erase(-mId);

	std::cout <<  "Point" << mId << " has been umounted" << std::endl;
	
	return true;
}

void Directory::lsMount() const
{
	for (auto c : mountPoints)
	{
		std::cout << "Point" << -(c.first) << " => " << c.second->path << std::endl;
	}
}

bool Directory::setMount(int mId, bool print)
{
	if (cd(std::to_string(mId), print, true, true))
	{
		return true;
	}
	return false;
}

bool Directory::searchM(std::string path, std::string& dest, int& result_loc)
{
	int temp_m = current_mountpoint;
	Folder* temp = current;

	if (!mountPoints.empty())
	{
		std::string mountedPath;
		std::vector<std::string> vec0, vec1;

		strSplit_v(path, vec0, "/");

		for (auto c : mountPoints)
		{
			mountedPath = c.second->path;
			strSplit_v(mountedPath, vec1, "/");

			for (unsigned int i = 0; i < vec1.size(); ++i)
			{
				if (vec0[i] != vec1[i])
				{
					break;
				}
				if (i == vec1.size() - 1)
				{
					setMount(-c.second->nodeId, false);

					dest = wdfR(path);
					result_loc  = -(c.second->nodeId);

					if (temp_m != 0)
					{
						setMount(temp_m, false);
					}
					cd(wdfR(temp->path), false);

					return true;
				}
			}
			std::vector<std::string>().swap(vec1);
		}
	}
	if (temp_m != 0)
	{
		setMount(temp_m, false);
	}
	cd(wdfR(temp->path), false);

	std::cout << "No mounted point matching" << std::endl;
	return false;
}

bool Directory::touch(std::string name)
{
	std::ofstream fo;
	std::string newPath;
	std::vector<FCB*> vec;

	if (current->find(name, vec, false, true, 1))
	{
		std::cout << "File already exists" << std::endl;
		return false;
	}

	newPath = current->path + "/" + name;
	fo.open(newPath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);

	File* newFile = new File(name, current->path, nodeIdPool.first, userId, current_mountpoint);
	if (current_mountpoint == 0)
	{
		int f_size = newFile->size();
		int i = (f_size / unit_block_size) + 1;

		if (storageMap.first.size() + i > storage_limit)
		{
			rm(newFile->name);
			std::cout << "touch: cannot create '" << name << "': No more storage" << std::endl;
			return false;
		}

		for (int j = 0; j < i; j++)
		{
			storageMap.first.insert(std::pair<int, File*>(storageMap.first.size(), newFile));
		}
	}
	newFile->parentNode = current;
	current->newChild(newFile);

	fo.close();
	return true;
}

bool Directory::cat(std::string name) const
{
	std::ifstream fi;
	std::string newPath, line;
	char buf[256];

	std::vector<FCB*> vec;
	if (!current->find(name, vec, false, true, 1))
	{
		std::cout << "cat: cannot read " << name << ": No such file" << std::endl;
		return false;
	}

	vec[0]->setTime(vec[0]->access.lastAccess);
	vec[0]->access.accessby = this->userId;

	newPath = current->path + "/" + name;
	fi.open(newPath.c_str(), std::ios::in);

	if (!fi.is_open())
	{
		std::cout << "Error when open the file" << std::endl;
		return false;
	}

	while (!fi.eof())
	{
		fi.getline(buf, 256);
		std::cout << buf << std::endl;
	}
	fi.close();
	return true;
}

bool Directory::open(std::string name)
{
	std::vector<FCB*> vec;
	if (!current->find(name, vec, false, true, 1))
	{
		std::cout << "open: cannot open " << name << ": No such file" << std::endl;
		return false;
	}

	File* ptr = dynamic_cast<File*>(vec[0]);

	for (auto c : openFiles)
	{
		if (c->nodeId == ptr->nodeId)
		{
			std::cout << "open: cannot open " << name << ":File is already opened" << std::endl;
			return false;
		}
	}
	openFiles.push_back(ptr);
	currentObj = ptr;

	ptr->setTime(vec[0]->access.lastAccess);
	ptr->access.accessby = this->userId;

	return true;
}

bool Directory::close()
{
	std::vector<File*>::iterator itr;
	for (itr = openFiles.begin(); itr != openFiles.end(); itr++)
	{
		if ((*itr)->nodeId == currentObj->nodeId)
		{
			openFiles.erase(itr);
			currentObj = NULL;
			return true;
		}
	}
	return false;
}

bool Directory::switchObj(int ID)
{
	for (auto c : openFiles)
	{
		if (c->nodeId == ID)
		{
			currentObj = c;
		}
	}
	return false;
}

void Directory::lsOpen() const
{
	if (openFiles.empty())
	{
		std::cout << "lsOpen: None file is open" << std::endl;
		return void();
	}
	for (auto c : openFiles)
	{
		std::cout << "name: " << c->name << "\tID: " << c->nodeId << std::endl;
	}
}

bool Directory::get(int size, bool end)
{
	std::string str;
	if (currentObj->get(str, size, end))
	{
		std::cout << str << std::endl;
		return true;
	}
	return false;
}

bool Directory::put(std::string str, bool end, bool replace)
{
	int size = str.size();
	if (currentObj->location == 0)
	{
		if (currentObj->size() + str.size() > unit_block_size)
		{

			int str_size = str.size();
			int i = (str_size / unit_block_size) + 1;

			if (storageMap.first.size() + i > storage_limit)
			{
				std::cout << "put: cannot put '" << str << "' into " << currentObj->name << ": No more storage" << std::endl;
				return false;
			}
			if (currentObj->put(str, size, end, replace))
			{
				for (int j = 0; j < i; j++)
				{
					storageMap.first.insert(std::pair<int, File*>(storageMap.first.size(), currentObj));
				}
				return true;
			}
		}
		else
		{
			if (currentObj->put(str, size, end, replace))
			{
				return true;
			}
		}
	}
	else
	{
		if (currentObj->put(str, size, end, replace))
		{
			return true;
		}
	}
	return false;
}

bool Directory::repos(int ptr_type, std::streamoff off, bool end)
{
	if (currentObj->repos(ptr_type, off, end))
	{
		return true;
	}
	return false;
}

bool Directory::trunc()
{
	if (currentObj->trunc())
	{
		return true;
	}
	return false;
}

void Directory::newUser(int userId, std::string name)
{
	userTable.insert(std::pair<int, std::string>(userId, name));
}

bool Directory::renameUser(int userId, std::string newName)
{
	int i;
	std::map<int, std::string>::iterator itr;
	for (itr = userTable.begin(); itr != userTable.end(); ++itr)
	{
		i = itr->first;

		if (i == userId)
		{
			itr->second = newName;
			return true;
		}
	}
	return false;
}

bool Directory::switchUser(int userId)
{
	int i;
	std::map<int, std::string>::iterator itr;
	for (itr = userTable.begin(); itr != userTable.end(); ++itr)
	{
		i = itr->first;

		if (i == userId)
		{
			this->userId = userId;
			std::stack<Folder*>().swap(history);

			return true;
		}
	}
	return false;
}

bool Directory::deleteUser(int userId)
{
	if (this->userId == userId)
	{
		return false;
	}
	int i;
	std::map<int, std::string>::iterator itr;
	for (itr = userTable.begin(); itr != userTable.end(); ++itr)
	{
		i = itr->first;

		if (i == userId)
		{
			userTable.erase(itr);
			return true;
		}
	}
	return false;
}

bool Directory::setMode(std::string name, int category, char newValue)
{
	std::vector<FCB*> vec;
	if (!current->find(name, vec))
	{
		std::cout << "setMode: " << name << ": No such file or directory" << std::endl;
		return false;
	}
	if (category == 1 && newValue == '0')
	{
		return false;
	}
	if (vec[0]->access.creator == userId)
	{
		if (vec[0]->access.mode[1] != '0')
		{
			return false;
		}
	}
	else
	{
		if (vec[0]->access.mode[7] != '0')
		{
			return false;
		}
	}

	std::string str;
	switch (category)
	{
	case 0:
		str = "RUSR";
	case 1:
		str = "WUSR";
	case 2:
		str = "XUSR";
	case 3:
		str = "RGRP";
	case 4:
		str = "WGRP";
	case 5:
		str = "XGRP";
	case 6:
		str = "ROTH";
	case 7:
		str = "WOTH";
	case 8:
		str = "XOTH";
	default:
		return false;
	}
	vec[0]->access.mode[category] = newValue;
	std::cout << name << ": " << str << "->" << newValue << std::endl;
	return true;
}

//////////////////////////////////////// PRIVATE ////////////////////////////////////////

std::string Directory::wdfR() const
{
	std::string subWD, mRootPath;
	std::string::size_type pos;

	if (mountmode)
	{
		mRootPath = mountPoints.at(-current_mountpoint)->path;
		pos = WD.find(mRootPath);
		subWD = WD.substr(pos + mRootPath.size());
		if (subWD.empty())
		{
			subWD = "MP" + std::to_string(current_mountpoint) + "/";
		}
		else
		{
			subWD = "MP" + std::to_string(current_mountpoint) + subWD;
		}
	}
	else
	{
		pos = WD.find(rootPath);
		subWD = WD.substr(pos + rootPath.size());
		if (subWD.empty())
		{
			subWD = "/";
		}
	}
	return subWD;
}

std::string Directory::wdfR(std::string wd) const
{
	std::string subWD, mRootPath;
	std::string::size_type pos;

	std::vector<std::string> vec0, vec1;
	bool is_inRoot = true;

	strSplit_v(wd, vec0, "/");
	strSplit_v(rootPath, vec1, "/");

	for (unsigned int i = 0; i < vec1.size(); ++i)
	{
		if (vec0[i] != vec1[i])
		{
			is_inRoot = false;
			break;
		}
	}

	if (!is_inRoot)
	{
		mRootPath = mountPoints.at(-current_mountpoint)->name;
		pos = wd.find(mRootPath);
		subWD = wd.substr(pos + mRootPath.size());

		if (subWD.empty())
		{
			subWD = "~";
		}
		else
		{
			subWD = "~" + subWD.substr(1);
		}
	}
	else
	{
		pos = wd.find(rootPath);
		subWD = wd.substr(pos + rootPath.size());
		if (subWD.empty())
		{
			subWD = "/";
		}
	}
	return subWD;
}

void Directory::strSplit_q(const std::string& str, std::queue<std::string>& vec, const std::string& delim) const
{
	std::string::size_type pos1, pos2;
	pos2 = str.find(delim);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		vec.push(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = str.find(delim, pos1);
	}
	if (pos1 != str.length())
		vec.push(str.substr(pos1));
}

void Directory::strSplit_v(const std::string& str, std::vector<std::string>& vec, const std::string& delim) const
{
	std::string::size_type pos1, pos2;
	pos2 = str.find(delim);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		vec.push_back(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = str.find(delim, pos1);
	}
	if (pos1 != str.length())
		vec.push_back(str.substr(pos1));
}

bool Directory::filecp(std::string src, std::string dest)
{
	std::ifstream fi;
	std::ofstream fo;
	int size;
	char* buf;

	fi.open(src.c_str(),  std::ios::binary | std::ios::in | std::ios::ate);
	fo.open(dest.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
	
	if (fi.bad() || fo.bad())
	{
		fi.close();
		fo.close();
		return false;
	}

	size = (unsigned int) fi.tellg();
	buf = new char[size];

	fi.seekg(0, std::ios::beg);
	fi.read(buf, size);
	fi.close();

	fo.write(buf, size);
	fo.close();
	delete[] buf;

	return true;
}

void Directory::unit_se(std::string& str1, std::string& str2)
{
	std::string line1, line2;
	line1 = std::to_string(current->nodeId);
	for (auto c : current->child)
	{
		line1 = line1 + ',' + std::to_string(c->nodeId);

		long int t1 = static_cast<long int> (c->access.lastModification);
		long int t2 = static_cast<long int> (c->access.lastAccess);

		line2 = std::to_string(c->nodeId) + ','
			+ c->path + ','
			+ std::to_string(c->isFolder) + ','
			+ std::to_string(c->access.creator) + ','
			+ std::to_string(c->access.lastEditor) + ','
			+ std::to_string(c->access.accessby) + ','
			+ std::to_string(t1) + ','
			+ std::to_string(t2) + ','
			+ std::to_string(c->access.visibility) + ','
			+ std::string(c->access.mode);

		line2 = line2 + '\n';
		str2 = str2 + line2;
	}
	line1 = line1 + '\n';
	str1 = str1 + line1;

	for (auto c : current->child)
	{
		if (c->isFolder)
		{
			Folder* ptr = dynamic_cast<Folder*>(c);
			if (ptr->count() != 0)
			{
				cd(ptr->name, false);
				unit_se(str1, str2);
				cd("..", false);
			}
		}
	}
}

void Directory::defragment(bool print)
{
	if (print)
	{
		std::cout << "Before defragmentation: " << std::endl;
		for (auto c : storageMap.first)
		{
			std::cout << "Index: " << c.first << "\tName: " << c.second->name << std::endl;
		}
	}

	std::map<File*, int> first_occ;
	std::map<int, File*> newStMap;

	std::map<int, File*>::iterator itr;
	for (itr = storageMap.first.begin(); itr != storageMap.first.end(); ++itr)
	{
		if (first_occ.count((*itr).second) == 0)
		{
			first_occ.insert(std::pair<File*, int>((*itr).second, (*itr).first));
			newStMap.insert(std::pair<int, File*>(newStMap.size(), (*itr).second));
			if (print)
			{
				std::cout << (*itr).first << "=>" << newStMap.size() - 1 << std::endl;
			}
			std::map<int, File*>::iterator _itr = itr;
			_itr++;
			while(_itr != storageMap.first.end())
			{
				if ((*itr).second->nodeId == (*_itr).second->nodeId)
				{
					newStMap.insert(std::pair<int, File*> (newStMap.size(), (*_itr).second));
					if (print)
					{
						std::cout << (*_itr).first << "=>" << newStMap.size() - 1 << std::endl;
					}
				}
				_itr++;
			}
		}
	}
	newStMap.swap(storageMap.first);
	std::set<int>().swap(storageMap.second);

	if (print)
	{
		std::cout << "After defragmentation: " << std::endl;
		for (auto c : storageMap.first)
		{
			std::cout << "Index: " << c.first << "\tName: " << c.second->name << std::endl;
		}
	}
}