#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>

#include <realm/object-store/dictionary.hpp>
#include <realm/object-store/object_schema.hpp>
#include <realm/object-store/property.hpp>
#include <realm/object-store/results.hpp>
#include <realm/object-store/schema.hpp>
#include <realm/object-store/impl/realm_coordinator.hpp>
#include <realm/object-store/impl/object_accessor_impl.hpp>


#include <realm.hpp>

namespace fs = std::filesystem;
 
using namespace realm;


const std::string TEST_FOLDER = "/tmp/test/";
const std::string TEST_FILE = TEST_FOLDER + "realm_XXXXXX";

void makeFolder() {
    auto tmpFile = fs::status(TEST_FILE);
    if(!fs::is_regular_file(tmpFile)){
        std::cout << "creating folder" << std::endl;
        fs::create_directory(TEST_FOLDER);
        std::ofstream output(TEST_FILE);
    }
}


template <typename RealmDictionary>
void insertValuesIntoDict(RealmDictionary dict, std::map<std::string, std::string>& mmap){

    for(std::pair<std::string, std::string> const key_value: mmap) {
        dict.insert(key_value.first, key_value.second);
    }
}

template <typename RealmDictionary>
void printValuesFromDict(RealmDictionary dict){

    // this obscure dict.template here, is because is required by C++ to call methods like dict.get which is a template function member. 
    std::cout << " value_one -> " << dict.template get<realm::String>("value_one")
    << std::endl; //one
    std::cout << " value_two -> " << dict.template get<realm::String>("value_two")
    << std::endl;
}

int main() {

    makeFolder();

    realm::Realm::Config config;
    config.in_memory = true;
    config.cache = false;
    config.automatic_change_notifications = false;
    config.path = TEST_FILE;
    config.schema_version = 0;
    config.schema = Schema{
        {"object", { {"value", PropertyType::Dictionary | PropertyType::String} } },
    };
    
    auto r = Realm::get_shared_realm(config);
    
    /*
        Here read_group will cascade into <begin_read(versionID)> ->
            shared_realm.cpp <realm-core>
     */
    auto table = r->read_group().get_table("class_object");
    
    r->begin_transaction();
    Obj obj = table->create_object();
    ColKey col = table->get_column_key("value");

    object_store::Dictionary dict(r, obj, col);
    auto track_result_set = dict.as_results();
    CppContext ctx(r);
    
    std::map<std::string, std::string> my_map = {
        {"value_one", "one"},
        {"value_two", "two"}
    };
    
    insertValuesIntoDict(dict, my_map);
    printValuesFromDict(dict);
    
   
    std::cout << "tracking result_set: " << track_result_set.size() << std::endl;
    std::cout << "Dict size: " << dict.size() << std::endl;
    
    return 0;
}

