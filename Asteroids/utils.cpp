#include "utils.hpp"

#include <Windows.h>

#include <Shlwapi.h>
#include <strsafe.h>


std::string utils::get_full_file_path (const std::string& partial_file_path)
{
	char path[MAX_PATH];

	wchar_t t_path[MAX_PATH];
	HMODULE module = GetModuleHandle (NULL);
	GetModuleFileName (module, t_path, MAX_PATH);
	PathRemoveFileSpec (t_path);

	char out_path[MAX_PATH];

	wcstombs_s (NULL, path, MAX_PATH, t_path, MAX_PATH);
	strcpy (out_path, path);
	strcat (out_path, "\\");
	strcat (out_path, partial_file_path.c_str ());

	return std::string (out_path);
}

std::vector<std::string> utils::get_all_files_in_folder (const std::string& full_folder_path)
{
	std::vector<std::string> files;
	files.reserve (5);

	wchar_t folder_path[MAX_PATH];
	mbstowcs (folder_path, (full_folder_path + "*" ).c_str (), MAX_PATH);

	WIN32_FIND_DATA ffd;
	HANDLE find_handle = INVALID_HANDLE_VALUE;

	find_handle = FindFirstFile (folder_path, &ffd);
	size_t num_files = 0;

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file_name[MAX_PATH];
			wcstombs (file_name, ffd.cFileName, MAX_PATH);
			std::string file_name_str (file_name);

			if (file_name_str.find (".glb") != std::string::npos)
			{
				files.emplace_back (full_folder_path + file_name_str);
			}
		}
	} while (FindNextFile (find_handle, &ffd) != 0);

	return files;
}

full_file_path::full_file_path (const std::string& partial_file_path)
{
	OutputDebugString (L"full_file_path::full_file_path partial_file_path\n");

	char path[MAX_PATH];

	wchar_t t_path[MAX_PATH];
	HMODULE module = GetModuleHandle (NULL);
	GetModuleFileName (module, t_path, MAX_PATH);
	PathRemoveFileSpec (t_path);

	char out_path[MAX_PATH];

	wcstombs_s (NULL, path, MAX_PATH, t_path, MAX_PATH);
	strcpy (out_path, path);
	strcat (out_path, "\\");
	strcat (out_path, partial_file_path.c_str ());

	this->path = std::string (out_path);
}

files_in_folder::files_in_folder (const std::string& full_folder_path)
{
	OutputDebugString (L"files_in_folder::files_in_folder full_folder_path\n");

	files.reserve (5);

	wchar_t folder_path[MAX_PATH];
	mbstowcs (folder_path, (full_folder_path + "*" ).c_str (), MAX_PATH);

	WIN32_FIND_DATA ffd;
	HANDLE find_handle = INVALID_HANDLE_VALUE;

	find_handle = FindFirstFile (folder_path, &ffd);
	size_t num_files = 0;

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file_name[MAX_PATH];
			wcstombs (file_name, ffd.cFileName, MAX_PATH);
			std::string file_name_str (file_name);

			if (file_name_str.find (".glb") != std::string::npos)
			{
				files.emplace_back (full_folder_path + file_name_str);
			}
		}
	} while (FindNextFile (find_handle, &ffd) != 0);
}