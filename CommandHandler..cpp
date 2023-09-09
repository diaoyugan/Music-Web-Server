#include "CommandHandler.h"
#include <iostream>

void reloadMusicMap(std::string musicMapFile){
    // 执行重新加载音乐映射文件的操作
std::cout << "Reloading music map file..." << std::endl;

std::wifstream file(musicMapFile);
if (file) {
    std::wstring line;
    while (std::getline(file, line)) {
        size_t pos = line.find(L'=');
        if (pos != std::wstring::npos) {
            std::wstring key = line.substr(0, pos);
            std::wstring value = line.substr(pos + 1);
            musicMap[key] = utility::conversions::to_utf8string(value);
        }
    }
    file.close();
    std::cout << "Music map loaded" << std::endl;

    // 输出音乐映射表
    for (const auto& pair : musicMap) {
        std::wcout << L"Key: " << pair.first << L", Value: " << utility::conversions::to_string_t(pair.second) << std::endl;
    }
    startHandleReloadCommand(musicMapFile);
}
else {
    std::cerr << "Err: music map can't be read or not found" << std::endl;
    startHandleReloadCommand(musicMapFile);
}
}

void handleReloadCommand(const std::string& command, std::string musicMapFile) {
    if (command == "reload") {
        reloadMusicMap(musicMapFile);
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void startHandleReloadCommand(std::string musicMapFile) {
    std::string input;
    std::cout << "Enter *reload* to reload music map \n";
    std::cin >> input;
    handleReloadCommand(input, musicMapFile);
}