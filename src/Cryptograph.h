#pragma once
#include <string>
#include <vector>

class Cryptograph
{
};

std::vector<unsigned char> Keygen(const std::string& password, const std::vector<unsigned char>& salt);
void FileEncrypt(const std::string& inputPath, const std::string& outputPath, const std::string& password);
void FileDecrypt(const std::string& inputPath, const std::string& outputPath, const std::string& password);