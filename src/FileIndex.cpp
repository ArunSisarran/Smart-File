
#include <cstddef>
#include <filesystem>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;
std::mutex fileMutex;

struct FileData {
    
    std::string fileName;
    std::string filePath;
    size_t fileSize;

    FileData(std::string n, std::string p, size_t s) : fileName(n), filePath(p), fileSize(s){}
};

std::unordered_map<std::string, FileData> fileIndex;
std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> fileHeap;

//this function goes through files in the given directory and extracts their name,path, and size
//then it adds it to the fileIndex hashmap for easy lookup and adds the size to the fileHeap
void indexFiles(const std::string& directory){

    std::lock_guard<std::mutex>lock(fileMutex);

    for(const auto& entry : fs::recursive_directory_iterator(directory)){

        if(entry.is_regular_file()){
            std::string path = entry.path().string();
            size_t size = fs::file_size(entry);
            std::string name = entry.path().filename().string();
            fileIndex[name] = FileData(name,path,size);
            fileHeap.push(size);
            std::cout<<"Indexed: "<<name<<" ("<<size<<" bytes)" << std::endl;
        }
    }
}
