#include "utils.hpp"

#include <Windows.h>

#include <Shlwapi.h>
#include <strsafe.h>


std::string utils::get_full_file_path (const std::string partial_file_path)
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
