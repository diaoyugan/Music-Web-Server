#include "NFCMS_include.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// 存储音乐文件对照表的文件路径
const std::string musicMapFile = "music_map.txt";

// 音乐映射表
std::map<std::wstring, std::string> musicMap;

// 从音乐文件对照表文件中加载映射
void loadMusicMap() {
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
    }
    else {
        std::cerr << "Err: music map can't be read or not found" << std::endl;
        exit(1);
    }
}

// 从音乐文件对照表中查找映射
std::string findMusicMapping(const std::wstring& track) {
    auto it = musicMap.find(track);
    if (it != musicMap.end()) {
        return it->second;
    }
    return "";
}

int main() {
    // 获取程序所在的目录
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH); // 注意使用GetModuleFileNameA

    // 从路径中提取目录部分
    std::string path(buffer);
    size_t found = path.find_last_of("\\/");
    std::string directory = path.substr(0, found);

    // 设置当前工作目录为ANSI字符串
    if (SetCurrentDirectoryA(directory.c_str())) { // 注意使用SetCurrentDirectoryA
        std::cout << "The current working directory has been set to the program's location: " << directory << std::endl;
    }
    else {
        std::cerr << "Failed to set the current working directory: " << GetLastError() << std::endl;
    }

    // 加载音乐映射表
    loadMusicMap();

    // 创建音乐文件服务监听器
    http_listener music_listener(L"http://localhost:8080");
    music_listener.support(methods::GET, [=](http_request request) {
        auto track = uri::decode(request.relative_uri().to_string());

        if (track.find(L"/music/") == 0) {
            std::wstring musicName = track.substr(7);
            std::string filePath = findMusicMapping(musicName);

            if (!filePath.empty()) {
                try {
                    std::ifstream musicFile(filePath, std::ios::binary);
                    if (musicFile) {
                        http_response response(status_codes::OK);
                        response.headers().set_content_type(L"audio/mpeg");

                        std::ostringstream responseStream;
                        responseStream << musicFile.rdbuf();
                        auto responseString = responseStream.str();

                        response.set_body(responseString);
                        request.reply(response);
                        std::cout << "Request URL: " << utility::conversions::to_utf8string(request.request_uri().to_string()) << std::endl;
                        musicFile.close();
                    }
                    else {
                        request.reply(status_codes::InternalError, L"Internal Server Error");
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    request.reply(status_codes::InternalError, L"Internal Server Error");
                }
            }
            else {
                request.reply(status_codes::NotFound, L"Music Not Found");
            }
        }
        else {
            request.reply(status_codes::NotFound, L"Invalid URL Format");
        }
        });

    // 创建 HTML 文件服务监听器
    http_listener html_listener(L"http://localhost:8081/html");
    html_listener.support(methods::GET, [](http_request request) {
        // 读取 HTML 文件内容并发送响应
        std::ifstream htmlFile("web/player.html");
        if (htmlFile) {
            http_response response(status_codes::OK);
            response.headers().set_content_type(L"text/html");

            std::ostringstream responseStream;
            responseStream << htmlFile.rdbuf();
            auto responseString = responseStream.str();

            response.set_body(responseString);
            request.reply(response);
        }
        else {
            request.reply(status_codes::InternalError, L"Internal Server Error");
        }
        });

    try {
        music_listener.open().wait();
        html_listener.open().wait();
        std::wcout << L"Music service listening for requests at: " << music_listener.uri().to_string() << std::endl;
        std::wcout << L"HTML service listening for requests at: " << html_listener.uri().to_string() << std::endl;
        while (true);
    }
    catch (const std::exception& e) {
        std::wcerr << L"Error: " << e.what() << std::endl;
    }

    return 0;
}
