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
#include <JavaScriptCore/JSValue.h>

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
    
    dict.insert("oops", 4);
}

void printInts(std::string key, object_store::Dictionary& dict) {
    Mixed v = dict.get<Mixed>(key).get_type();
    

    std::cout << "Int printer:  " << key << " => " << dict.template get<realm::Int>(key)
    << std::endl;
}

void printString(std::string key, object_store::Dictionary& dict) {
    std::cout << "String printer:  " << key << " => " << dict.template get<realm::String>(key)
    << std::endl;
}

std::map< DataType, std::function<void(std::string, object_store::Dictionary&)> > _lambda = {
    { DataType::type_Int, printInts },
    { DataType::type_String, printString },
};


void readValuesFromDict(object_store::Dictionary& dict) {

    for(std::pair<Mixed, Mixed> mix: dict){
        auto valueType = mix.second.get_type();
        
        auto exec = _lambda[ valueType ];
            
        exec( mix.first.get<String>(), dict);
    }
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
        {"object", { {"value", PropertyType::Dictionary | PropertyType::Mixed } } },
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
   
    std::cout << "test 01: is Mixed ? " << (dict.get_type() == PropertyType::Mixed) << std::endl;
//
//    table->add_column(type_UUID, "id", true);
//    table->get_column_key("id");
//    table->create_object().set(<#ColKey col_key#>, <#U value#>)
//      std::cout << "UUID validaty: " << UUID::is_valid_string("5fc77d9fae64461661a5c160") << std::endl;
//    UUID myuid("5fc77d9fae64461661a5c160");
    
    insertValuesIntoDict(dict, my_map);
    readValuesFromDict(dict);

    r->commit_transaction();
    
    return 0;
}

