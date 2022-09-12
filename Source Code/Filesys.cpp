# include <iostream>
# include <fstream>
# include <Windows.h>
# include <fileapi.h>
# include "Directory.h"

using namespace std;

Directory* fileSysInit();

int main()
{
	Directory* direct = fileSysInit();






    int userID = 0;

    File file1("file1.txt","D:\root",userID);





	bool test = (*direct).Mkdir("test");
	test = (*direct).Mkdir("test1");
	test = (*direct).Mkdir("test1");
	test = (*direct).Mkdir("test2");
	test = (*direct).cd("test");
	test = (*direct).cd("..");
	(*direct).ls(true);
	test = (*direct).cd("test");
	test = (*direct).cd("/test2");
	test = (*direct).Mkdir("test");
	test = (*direct).cd("test");
	test = (*direct).Mkdir("test");
	test = (*direct).cd("test");
	test = (*direct).cd("../..");
	test = (*direct).cd("/test1");
	test = (*direct).cd("/test2/test/test");
	test = (*direct).cd("-");
	test = (*direct).cd("/test2");
	test = (*direct).find("test");
	test = (*direct).Mkdir("test3");
	test = (*direct).cd("test3");
	test = (*direct).Mkdir("test4");
	test = (*direct).cd("test4");
	test = (*direct).cd("/test2");

	test = (*direct).mv("test3", "/");
	(*direct).pwd();
	(*direct).ls();
	test = (*direct).cd("/");
	(*direct).ls();
	test = (*direct).cd("test3");
	(*direct).ls();

	test = (*direct).cd("/");
    //test = (*direct).Rmdir("test");
	(*direct).ls();
    //test = (*direct).Rmdir("test1");
	(*direct).ls();
    //test = (*direct).Rmdir("test2");
    //test = (*direct).Rmdir("test3");

	delete direct;
	return 0;
}

Directory* fileSysInit()
{
	string rootPath = "D:";
	rootPath = rootPath + "/root";

	size_t convertedChars = 0;
	size_t pathsize = rootPath.length() + 1;
	wchar_t* wcstring = new wchar_t[pathsize - 1];
	mbstowcs_s(&convertedChars, wcstring, pathsize, rootPath.c_str(), _TRUNCATE);

	DWORD dwAttrib = GetFileAttributes(wcstring);

	bool exist = INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
	if (!exist)
	{
		if (_mkdir(rootPath.c_str())) {}
	}
	Directory* _direct = new Directory(rootPath);
	return _direct;
}
