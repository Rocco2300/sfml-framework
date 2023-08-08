#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>

using ios = std::ios;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cout << "Usage: customTexGen <path_to_image> <path_to_metadata> <path_to_output>" << std::endl;
    }

    const std::string imageFilePath = argv[1];
    const std::string metadataFilePath = argv[2];
    const std::string outputFilePath = argv[3];

    std::ifstream imageFile(imageFilePath, ios::in | ios::binary);
    std::ifstream metadataFile(metadataFilePath);
    std::ofstream outFile(outputFilePath, ios::out | ios::binary);

    auto imageFileSize = fs::file_size(imageFilePath);
    std::vector<std::byte> imageBuffer(imageFileSize);
    imageFile.read(reinterpret_cast<char*>(imageBuffer.data()), imageFileSize);

    auto metadataFileSize = fs::file_size(metadataFilePath);
    std::vector<std::byte> metadataBuffer(metadataFileSize + 1);
    metadataFile.read(reinterpret_cast<char*>(metadataBuffer.data()), metadataFileSize);

    outFile.write(reinterpret_cast<char*>(&imageFileSize), sizeof(imageFileSize));
    outFile.write(reinterpret_cast<char*>(&metadataFileSize), sizeof(metadataFileSize));
    outFile.write(reinterpret_cast<char*>(imageBuffer.data()), imageFileSize);
    outFile.write(reinterpret_cast<char*>(metadataBuffer.data()), metadataFileSize);

    return 0;
}