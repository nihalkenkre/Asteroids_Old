#pragma once

#include <string>
#include <vector>


namespace utils 
{
    std::string get_full_file_path (const std::string& partial_file_path);
    std::vector<std::string> get_all_files_in_folder (const std::string& folder_path);
}

class full_file_path
{
public:
    full_file_path ();
    full_file_path (const std::string& partial_file_path);
    full_file_path (const full_file_path& other) = delete;
    full_file_path& operator= (const full_file_path& other) = delete;
    full_file_path (full_file_path&& other) noexcept;
    full_file_path& operator= ( full_file_path&& other) noexcept;
    ~full_file_path () noexcept;

    std::string path;
};

class files_in_folder
{
public:
    files_in_folder ();
    files_in_folder (const std::string& full_folder_path);
    files_in_folder (const files_in_folder& other) = delete;
    files_in_folder& operator= (const files_in_folder& other) = delete;
    files_in_folder (const files_in_folder&& other) noexcept;
    files_in_folder& operator= (const files_in_folder&& other) noexcept;
    ~files_in_folder () noexcept;

    std::vector<std::string> files;
};
