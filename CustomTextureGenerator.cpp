#include <filesystem>
#include <fstream>

using ios = std::ios;
namespace fs = std::filesystem;

const std::string ImageFileName = "testing.png";
const std::string MetadataFileName = "metadata.txt";
const std::string OutputFileName = "output.tex";

const std::string CurrentDir = "C:/Users/grigo/Repos/sfml-framework/";

int main() {

    std::ifstream imageFile(CurrentDir + ImageFileName, ios::in | ios::binary);
    std::ifstream metadataFile(CurrentDir + MetadataFileName);
    std::ofstream outFile(CurrentDir + OutputFileName, ios::out | ios::binary);

    auto imageFileSize = fs::file_size(CurrentDir + ImageFileName);
    char* imageBuffer = new char[imageFileSize];
    imageFile.read(imageBuffer, imageFileSize);

    auto metadataFileSize = fs::file_size(CurrentDir + MetadataFileName);
    char* metadataBuffer = new char[metadataFileSize];
    metadataFile.read(metadataBuffer, metadataFileSize);

    outFile.write(reinterpret_cast<char*>(&imageFileSize), sizeof(imageFileSize));
    outFile.write(reinterpret_cast<char*>(&metadataFileSize), sizeof(metadataFileSize));
    outFile.write(imageBuffer, imageFileSize);
    outFile.write(metadataBuffer, metadataFileSize);

    delete[] imageBuffer;
    delete[] metadataBuffer;

    return 0;
}