#pragma once

#include <string>
#include <vector>


namespace utils 
{
    std::string get_full_file_path (const std::string& partial_file_path);
    std::vector<std::string> get_all_files_in_folder (const std::string& folder_path);
}