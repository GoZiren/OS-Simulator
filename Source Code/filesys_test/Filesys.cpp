# include <iostream>
# include <fstream>

# ifdef _WIN32
# include <Windows.h>
# include <fileapi.h>
# endif

# include "Directory.h"

using namespace std;

Directory* fileSysInit(bool& reconstruct);

int main()
{
    bool a = false;
	bool test;
	Directory* direct = fileSysInit(a);

	if (!a)
	{
		test = (*direct).Mkdir("test");
		test = (*direct).Mkdir("test1");
		test = (*direct).Mkdir("test1"); // error test
		test = (*direct).Mkdir("test2");
		test = (*direct).cd("test");
		test = (*direct).cd("t"); // error test
		test = (*direct).cd("..");
		(*direct).ls(true);
		test = (*direct).cd("test");
		test = (*direct).cd("/test2");
		test = (*direct).Mkdir("test");
		test = (*direct).cd("test");
		test = (*direct).Mkdir("test");
		test = (*direct).cd("test");
		std::cout << std::endl;
		test = (*direct).cd("../..");
		std::cout << std::endl;
		test = (*direct).cd("/test1");
		std::cout << std::endl;
		test = (*direct).cd("/test2/test/test");
		std::cout << std::endl;
		test = (*direct).cd("-");
		test = (*direct).cd("-");
		test = (*direct).cd("-");
		test = (*direct).cd("-");

		test = (*direct).cd("/test2");
		test = (*direct).find("test");

		test = (*direct).Mkdir("test3");
		test = (*direct).cd("test3");
		test = (*direct).Mkdir("test4");
		test = (*direct).Mkdir("test5");
		(*direct).ls();
		test = (*direct).cd("/test2");
		test = (*direct).mv("3", "a", 0); // error test
		test = (*direct).mv("test3", "t", 0); // rename
		(*direct).ls();
		test = (*direct).mv("t", "test3", 0); // rename
		test = (*direct).mv("test3", "/", 0);
		(*direct).pwd();
		(*direct).ls();
		test = (*direct).cd("/");
		(*direct).ls();
		test = (*direct).cd("test3");
		(*direct).ls();
# ifdef _WIN32
        test = (*direct).mount("C:/a");
        test = (*direct).mount("C:/a/b"); // error test
        test = (*direct).mount("C:/a2");
        test = (*direct).mount("C:/"); // error test
		test = (*direct).mount("D:/root"); // error test
# else
		test = (*direct).mount("/home/cuhksz_csc/Desktop/a");
# endif
		(*direct).pwd();

		//test = (*direct).cd("1", true, true, true);
		test = (*direct).setMount(1);
		test = (*direct).mv("c.txt", "/", 0, true);
		test = (*direct).mv("c.txt", "/", 0, true);
		test = (*direct).cd("b");
		test = (*direct).cd("~c");

		test = (*direct).cd("/");
		(*direct).ls(true);
		(*direct).lsMount();

		test = (*direct).umount(1);
		test = (*direct).umount(2);

	}
	else
	{
		(*direct).deserialise("");
		test = (*direct).cd("/test3");
		(*direct).ls(true);
	}

	test = (*direct).cd("/");
	(*direct).touch("d.txt");
	(*direct).ls();
	(*direct).open("d.txt");
	(*direct).lsOpen();
	(*direct).put("added content", false, false);
	//(*direct).cat("d.txt");
	(*direct).get(5);
	(*direct).close();

	(*direct).cat("c.txt");
	(*direct).open("c.txt");
	(*direct).lsOpen();
	(*direct).repos(1, 2);
	(*direct).put("added content", false, false);
	(*direct).cat("c.txt");
	(*direct).repos(0, 1);
	(*direct).get(5);
	(*direct).close();
	(*direct).lsOpen();

	if (a)
	{
		(*direct).defragment();
		test = (*direct).rm("d.txt");
		(*direct).defragment();
		test = (*direct).Rmdir("test");
		test = (*direct).Rmdir("test"); // error test
		(*direct).ls();
		test = (*direct).Rmdir("test1");
		(*direct).ls();
		test = (*direct).Rmdir("test2");
		test = (*direct).Rmdir("test3");
		test = (*direct).rm("c.txt");
		(*direct).ls();
	}

	cout << "\nserialise" << endl;
	(*direct).serialise("");
	cout << "serialise" << endl << endl;

	delete direct;
	return 0;
}

#ifdef _WIN32
Directory* fileSysInit(bool &reconstruct)
{
    string rootPath = "C:";
	rootPath = rootPath + "/root";

	size_t convertedChars = 0;
	size_t pathsize = rootPath.length() + 1;
	wchar_t* wcstring = new wchar_t[pathsize - 1];
	mbstowcs_s(&convertedChars, wcstring, pathsize, rootPath.c_str(), _TRUNCATE);

	DWORD dwAttrib = GetFileAttributes(wcstring);

	bool exist = INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
	if (!exist)
	{
		if (_access("_detail.txt", 0) == -1)
		{
			if (_mkdir(rootPath.c_str()) == 0)
			{
				reconstruct = false;
				Directory* _direct = new Directory(rootPath);
				return _direct;
			}
		}
	}
	else if (_access("_detail.txt", 0) != -1)
	{
		reconstruct = true;
		std::cout << "haha" << std::endl;
		Directory* _direct = new Directory(rootPath);
		return _direct;
	}

	std::cout << "Invalid path" << std::endl;
	return NULL;
}
# else
Directory* fileSysInit(bool& reconstruct)
{
	string rootPath = "/home/cuhksz_csc/Desktop";
	rootPath = rootPath + "/root";

	int exist = access(rootPath.c_str(), F_OK);
	if (exist == -1)
	{
		if (access("_detail.txt", F_OK) == -1)
		{
			if (mkdir(rootPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
			{
				Directory* _direct = new Directory(rootPath);
				return _direct;
			}
		}
	}
	else if (access("_detail.txt", F_OK) != -1)
	{
		reconstruct = true;
		std::cout << "haha" << std::endl;
		Directory* _direct = new Directory(rootPath);
		return _direct;
	}

	std::cout << "Invalid path" << std::endl;
	return NULL;
}
# endif
