#pragma once

#include <string>
#include <vector>


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
