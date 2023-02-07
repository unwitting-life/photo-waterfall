// photo-waterfall.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#define WWWROOTA "wwwroot"
#define WWWROOTW _T(WWWROOTA)
#ifdef UNICODE
#define WWWROOT WWWROOTW
#else
#define WWWROOT WWWROOTA
#endif

#define INDEX_HTML _T("photo-waterfall.html")
#define IMG _T("img")

#define CPPHTTPLIB_HTTPLIB_SUPPORT
#include "../cpp-utils/utils.hpp"
#include "./x64/Release/pack.hpp"

void Get(const ::httplib::Request&, ::httplib::Response&);

int main() {
    std::cout << "photo-waterfall!\n";
    utils::httplib::listen(Get);
}

void Get(const ::httplib::Request& req, ::httplib::Response& res) {
    auto imageUri = std::vector <std::string>();
    auto physicalPath = utils::io::path::combineA(WWWROOTA, req.path);
    res.status = utils::httplib::status::NOT_FOUND;
    if (req.path == "/" && utils::io::file::exists(utils::io::path::combine(WWWROOT, INDEX_HTML))) {
        for (auto& e : utils::io::directory::GetFiles(utils::io::path::combine(WWWROOT, IMG), false)) {
            auto extension = utils::strings::lower(utils::io::path::GetFileExtensionName(e));
            if (extension == _T(".jpg") || extension == _T(".gif") || extension == _T(".png") || extension == _T(".bmp") ||
                extension == _T(".webp")) {
                imageUri.push_back(utils::strings::t2s(e));
            }
        }
        auto images = std::string();
        for (auto& e : utils::io::file::sortByModifyTimeA(imageUri)) {
            if (!images.empty()) {
                images += "\n";
            }
            images += utils::strings::formatA("    <img data-src=\"%s\" data-title=\"%s\" style=\"opacity: 0\"/>",
                                              utils::strings::replaceA(e.substr(_tcslen(WWWROOT)), PATH_SEPARATOR_A, "/").c_str(),
                                              utils::io::path::GetFileNameA(e).c_str());
        }
        auto html = utils::io::file::read(utils::io::path::combine(WWWROOT, INDEX_HTML));
        if (!html.empty()) {
            res.set_content(utils::strings::replaceA(html, "%1", images.c_str()),
                            utils::httplib::mime::GetMimeTypeA(".html"));
            res.status = utils::httplib::status::OK;
        }
    } else if (utils::io::file::existsA(physicalPath)) {
        res.set_content(utils::io::file::readA(physicalPath),
                        utils::httplib::mime::GetMimeTypeA(physicalPath));
        res.status = utils::httplib::status::OK;
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
