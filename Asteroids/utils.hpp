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
    full_file_path (const std::string& partial_file_path);

    std::string path;
};

class files_in_folder
{
public:
    files_in_folder (const std::string& full_folder_path);

    std::vector<std::string> files;
};
