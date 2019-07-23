/// -----------------------------------------------------------------------------------------------
/// Cross platform file system code interface. 
/// Currently supported platforms:
/// - Windows Desktop.
/// -----------------------------------------------------------------------------------------------

#ifndef FILESYSTEM_INCLUDED_H
#define FILESYSTEM_INCLUDED_H

#include <string>
#include "StringUtil.h"

#ifdef _WIN32
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <Shlwapi.h>
#include <vector>

#pragma comment(lib, "User32.lib")
#endif

namespace CommonCode
{

using namespace std;

class FileInfo
{
	public:

		FileInfo(string path);

		string GetExtension(); 
		string GetName();
		string GetPath();

	private:

		void Split();

		string m_Name;
		string m_Extension;
		string m_Path;
};

class FileSystem
{

	public:
		
		// File

		// Directory 
		static bool CreateDir(string path);
		static string CurrentDir();

		static string CombinePath(string left, string right);

		static bool IsDirectory(string path);

		// File & Directory

		static vector<string> GetFiles(string path);

		///<summary>
		///Checks if the given path/filepath exists.
		///</summary>
		static bool Exists(string path);

		static bool Delete(string path);


};

}


#endif