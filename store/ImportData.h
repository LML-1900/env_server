//
// Created by lml on 23-8-1.
//

#ifndef IMPORT_DEM_IMPORTDATA_H
#define IMPORT_DEM_IMPORTDATA_H
#include <string>
#include <vector>

class ImportData {
public:
    void readDemDirAndImportToDB(std::string dirPath);

    static ImportData* getInstance();
private:
    static ImportData* instance;

    std::vector<std::string> getFilesList(std::string dirPath);

    std::string getPieceId(std::string filePath, std::string dirPath);

    ~ImportData();

    ImportData();

    ImportData(const ImportData&);

    ImportData& operator=(const ImportData&);
};


#endif //IMPORT_DEM_IMPORTDATA_H