
#include "FileSystem.h"


namespace CommonCode
{

using namespace std;

FileInfo::FileInfo(string path)
{
	m_Path = path;
	m_Name = "";
	m_Extension = "";

	Split();
}

void FileInfo::Split()
{
	vector<string> tokens;

	string result = StringUtil::ReplaceString(m_Path, "//", "\\");
	StringUtil::Tokenize(result.c_str(), tokens, "\\");
	if (tokens.size() > 0)
	{
		string lastPart = tokens[tokens.size() - 1];

		tokens.clear();
		StringUtil::Tokenize(lastPart, tokens, ".");

		if (tokens.size() == 2)
		{
			m_Name = tokens[0];
			m_Extension = tokens[1];
		}
	}
}

string FileInfo::GetExtension()
{
	return m_Extension;
}

string FileInfo::GetName()
{
	return m_Name;
}

string FileInfo::GetPath()
{
	return m_Path;
}

vector<string> FileSystem::GetFiles(string path)
{
    vector<string> foundFiles;

#ifdef _WIN32

	WIN32_FIND_DATA findData;
	TCHAR buffer[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD hError = 0;

	size_t pathLen = path.size();
	StringCchLength(path.c_str(), MAX_PATH, &pathLen);

	if (pathLen > MAX_PATH)
	{
		return foundFiles;
	}

	StringCchCopy(buffer, MAX_PATH, path.c_str());
	StringCchCat(buffer, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(buffer, &findData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return foundFiles;
	}

	do
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			foundFiles.push_back(path + "\\" + string(findData.cFileName));
		}
	} while (FindNextFile(hFind, &findData) != 0);

#endif
	
	return foundFiles;
}

bool FileSystem::IsDirectory(string path)
{

#ifdef _WIN32

	const char* ptrPath = path.c_str();
	if (PathFileExists(ptrPath))
	{
		auto dwAttrs = GetFileAttributes(ptrPath);
		// detect if given path is a directory
		if (dwAttrs == FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}
	}

#endif

	return false;
}

bool FileSystem::CreateDir(string path)
{
#ifdef _WIN32

	return (CreateDirectory(path.c_str(), nullptr) != 0);

#endif
}

string FileSystem::CurrentDir()
{
#ifdef _WIN32

	char buffer[MAX_PATH];
	DWORD result = GetCurrentDirectory(MAX_PATH, buffer);
	return string(buffer);

#else
	return "Not supported yet";
#endif
}

bool FileSystem::Exists(string path)
{
#ifdef _WIN32

	return (PathFileExists(path.c_str()) != 0);

#else
	return false;
#endif
}

bool FileSystem::Delete(string path)
{
#ifdef _WIN32

	const char* ptrPath = path.c_str();
	if (PathFileExists(ptrPath))
	{
		auto dwAttrs = GetFileAttributes(ptrPath);

		if (dwAttrs == INVALID_FILE_ATTRIBUTES // corrupt
			|| ((dwAttrs & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY))
		{
			return false;
		}

		// Detect if given path is a directory.
		if (dwAttrs == FILE_ATTRIBUTE_DIRECTORY)
		{
			// Directory can only be removed if there are no files contained.
			return (RemoveDirectory(ptrPath) != 0);
		}
		return (DeleteFile(ptrPath) != 0);
	}
	return false;

#else

	return false;

#endif
}

}
