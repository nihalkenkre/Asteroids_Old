#include "Utils.hpp"

#include <Windows.h>

#include <Shlwapi.h>
#include <strsafe.h>

std::string Utils::GetFullFilePath (const std::string partial_file_path)
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
