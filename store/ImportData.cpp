//
// Created by lml on 23-8-1.
//
#include <string>
#include <vector>
#include <iostream>
#include "ImportData.h"
#include "MySQLClient.h"
#include "MongoClient.h"
using std::string;
using std::vector;
const int DEM = 1;

ImportData* ImportData::instance = nullptr;

ImportData* ImportData::getInstance() {
    if (instance == nullptr) {
        instance = new ImportData();
    }
    return instance;
}

ImportData::ImportData() {}

void ImportData::readDemDirAndImportToDB(string dirPath) {
    vector<string> allFilePath = getFilesList(dirPath);
    for (const auto &fileName: allFilePath) {
        string piece_id = getPieceId(fileName, dirPath);
        int id = MySQLClient::getInstance()->insertDataToFileStorage(piece_id, DEM);
        MongoClient::getInstance()->insertDemFile(id, fileName);
    }
}

#ifdef linux
string ImportData::getPieceId(std::string filePath, std::string dirPath) {
    int len = filePath.size(), i;
    vector<string> split_path;
    string sub_filepath = filePath.substr(dirPath.size(), filePath.size());
    sub_filepath += "/";
    size_t pos = sub_filepath.find("/");
    while (pos != sub_filepath.npos)
    {
        string temp = sub_filepath.substr(0, pos);
        split_path.push_back(temp);
        sub_filepath = sub_filepath.substr(pos + 1, sub_filepath.size());
        pos = sub_filepath.find("/");
    }
    pos = split_path[3].find(".");
    split_path[3] = split_path[3].substr(0, pos);
    return split_path[1] + "_" + split_path[2] + "_" + split_path[3];
}
#endif

#ifdef linux
#include <memory.h>
#include <dirent.h>
vector<string> ImportData::getFilesList(string dirPath) {
    vector<string> allPath;
    DIR *dir = opendir(dirPath.c_str());
    if (dir == NULL)
    {
        std::cout << "opendir error" << std::endl;
        return allPath;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR) {//It's dir
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            string dirNew = dirPath + "/" + entry->d_name;
            vector<string> tempPath = getFilesList(dirNew);
            allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());

        }
        else {
            string name = entry->d_name;
            string imgdir = dirPath + "/" + name;
            allPath.push_back(imgdir);
        }

    }
    closedir(dir);
    return allPath;
}

#endif

#ifdef _WIN32//__WINDOWS_
#include <io.h>
vector<string> ImportData::getFilesList(string dir)
{
	vector<string> allPath;
	// 在目录后面加上"\\*.*"进行第一次搜索
	string dir2 = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1) {// 检查是否成功
		std::cout << "can not found the file ... " << std::endl;
		return allPath;
	}
	while (_findnext(handle, &findData) == 0)
	{
		if (findData.attrib & _A_SUBDIR) 是否含有子目录
		{
			//若该子目录为"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			string dirNew = dir + "\\" + findData.name;
			vector<string> tempPath = getFilesList(dirNew);
			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else //不是子目录，即是文件，则输出文件名和文件的大小
		{
			string filePath = dir + "\\" + findData.name;
			allPath.push_back(filePath);
		}
	}
	_findclose(handle);    // 关闭搜索句柄
	return allPath;
}
#endif

#ifdef _WIN32//__WINDOWS_
string ImportData::getPieceId(std::string filePath) {
    int len = filePath.size(), i;
    vector<string> split_path;
    string sub_filepath = filePath.substr(dirPath.size(), filePath.size());
    sub_filepath += "\\";
    size_t pos = sub_filepath.find("\\");
    while (pos != sub_filepath.npos)
    {
        string temp = sub_filepath.substr(0, pos);
        split_path.push_back(temp);
        sub_filepath = sub_filepath.substr(pos + 1, sub_filepath.size());
        pos = sub_filepath.find("\\");
    }
    pos = split_path[3].find(".");
    split_path[3] = split_path[3].substr(0, pos);
    return split_path[1] + "_" + split_path[2] + "_" + split_path[3];
}
#endif