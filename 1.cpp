#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include <windows.h>
#include <cstring>

#include<windows.h>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
void HideWindow() {
	HWND hwnd = GetForegroundWindow();
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);
	}
}
int RunAsAdmin(LPCSTR Path, LPCSTR Param, LPCSTR Dir, int Showcmd)
{
    HINSTANCE res;
    ShellExecute(NULL, "runas", Path, Param, Dir, Showcmd);

}

void trimstring(std::string& str)
{
    if (!str.empty())
    {
        str.erase(0, str.find_first_not_of(" "));
        str.erase(str.find_last_not_of(" ") + 1);
    }
}

std::string getlocalip()
{
    std::string ip("127.0.0.1");
    std::string ipconfig_content;

    FILE* fp = _popen("ipconfig", "r");
    if (NULL != fp)
    {
        char line[4096];
        while (NULL != fgets(line, sizeof(line), fp))
        {
            ipconfig_content += line;
        }

        auto p = ipconfig_content.rfind("IPv4");
        if (p != std::string::npos)
        {
            auto p2 = ipconfig_content.find(":", p);
            if (p2 != std::string::npos)
            {
                auto p3 = ipconfig_content.find("\n", p2);
                if (p3 != std::string::npos)
                {
                    ip = ipconfig_content.substr(p2 + 1, p3 - p2 - 1);
                    trimstring(ip);
                }
            }
        }

        _pclose(fp);
    }

    return ip;
}
// 使用字符分割
void Stringsplit(const string& str, const char split, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + split;
	size_t pos = strs.find(split);

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
}
// 使用字符串分割
void Stringsplit(const string& str, const string& splits, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}



int main()
{
    HideWindow();
    auto ipret = getlocalip();
//    std::cout << ipret << std::endl;
    vector<string> strList;
    Stringsplit(ipret, '.', strList);
//    cout<<strList[2];
    std::string cmd_ = "change 0.0.0.0 mask 0.0.0.0 10.10."+strList[2]+".252";
//    cout<<cmd_<<endl;
    char *p=(char*) cmd_.data();
    RunAsAdmin("route",p,NULL,0);

//    std::cin.get();
}
