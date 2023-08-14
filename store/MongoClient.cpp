//
// Created by lml on 23-7-27.
//

#include "MongoClient.h"

MongoClient* MongoClient::mongoClient = nullptr;

MongoClient* MongoClient::getInstance() {
    if (mongoClient == nullptr) {
        mongoClient = new MongoClient();
    }
    return mongoClient;
}

MongoClient::MongoClient() {
    mongocxx::uri uri(mongouri);
    client = mongocxx::client(uri);
    db = client[databaseName];
    collection = db[collectionName];
}

MongoClient::~MongoClient() {
    delete mongoClient;
}

bool MongoClient::insertDemFile(int id, std::string fileName) {
    // 以二进制方式打开.tif文件
    std::ifstream file(fileName, std::ios::binary);
    // 将文件内容读取到vector<char>中
    std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // 创建一个新文档
    bsoncxx::builder::basic::document document;
    document.append(bsoncxx::builder::basic::kvp("id", id));
    // 添加二进制数据
    document.append(bsoncxx::builder::basic::kvp("content", bsoncxx::types::b_binary{bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(content.size()), reinterpret_cast<const uint8_t*>(content.data())}));
    try {
        const auto insert_result = collection.insert_one(document.view());
        if (!insert_result) {
            return false;
        }
    } catch(const mongocxx::exception& e) {
        std::cerr << "Exception occurred while inserting document: " << e.what() << '\n';
        return false;
    }
    return true;
}

void MongoClient::insertCraters(int id, vector<Store::Crater> craters) {
    bsoncxx::builder::basic::array array_builder;
    for (const auto& crater : craters) {
        bsoncxx::builder::basic::document posDoc;
        posDoc.append(bsoncxx::builder::basic::kvp("longitude", crater.position.longitude));
        posDoc.append(bsoncxx::builder::basic::kvp("latitude", crater.position.latitude));

        bsoncxx::builder::basic::document craterDoc;
        craterDoc.append(bsoncxx::builder::basic::kvp("position", posDoc));
        craterDoc.append(bsoncxx::builder::basic::kvp("width", crater.width));
        craterDoc.append(bsoncxx::builder::basic::kvp("depth", crater.depth));

        array_builder.append(craterDoc);
    }

    auto array_value = array_builder.extract();

// 创建用于查找文档的查询器
    bsoncxx::builder::basic::document query_filter{};
    query_filter.append(bsoncxx::builder::basic::kvp("id", id));  // 替换为实际的id值

// 创建更新策略
    bsoncxx::builder::basic::document update_document{};
    update_document.append(bsoncxx::builder::basic::kvp("$push",[&](bsoncxx::builder::basic::sub_document subdoc) {
        subdoc.append(bsoncxx::builder::basic::kvp("craters",[&](bsoncxx::builder::basic::sub_document subdoc2){
            subdoc2.append(bsoncxx::builder::basic::kvp("$each", array_value));}));}));

// 更新数据库
    collection.update_one(query_filter.view(), update_document.view());
}

Store::DemData MongoClient::getDemDataById(int id) {
    Store::DemData data;
    auto find_one_result = collection.find_one(make_document(kvp("id", id)));
    if (find_one_result) {
        bsoncxx::document::view view = *find_one_result;
        if (view["content"]) {
            auto binary_data_view = view["content"].get_binary();
            // 转换为Base64字符串
            std::ostringstream ostr;
            Poco::Base64Encoder encoder(ostr);
            encoder.write(reinterpret_cast<const char*>(binary_data_view.bytes), binary_data_view.size);
            encoder.close();  // Call close to flush the remaining bytes

            data.content = ostr.str();
//            data.content = view["content"].get_string().value.to_string();
        }
        if (view["craters"]) {
            auto array_view = view["craters"].get_array().value;
            for (const auto &element: array_view) {
                Store::Crater crater;
                auto crater_doc = element.get_document().value;
                auto pos = crater_doc["position"].get_document().value;
                crater.position.longitude = pos["longitude"].get_double().value;
                crater.position.latitude = pos["latitude"].get_double().value;
                crater.width = crater_doc["width"].get_double().value;
                crater.depth = crater_doc["depth"].get_double().value;
                data.craters.push_back(crater);
            }
        }
    }
    return data;
}


