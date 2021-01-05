#include "../src/HashMap.hpp"

#include <iostream>
#include <string>
#include <thread> 
#include <mutex>
#include <chrono>
#include "gtest/gtest.h"

TEST(HashMap, ConstructorTest){
    HashMap< std::string, int> map1;
    HashMap< std::string, int> map2(10);

    EXPECT_TRUE(map1.empty());
    EXPECT_TRUE(map2.empty());
}

TEST(HashMap, GetterTest){
    HashMap< std::string, int> map(10);

    map["str_1"] = 1;
    map["str_2"] = 2;
    map["str_1"] = 3;

    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map["str_1"], 3);
}

TEST(HashMap, Iterator){
    HashMap< std::string, int> map(10);

    map["str_1"] = 1;
    map["str_2"] = 2;
    map["str_1"] = 3;

    for (auto it = map.begin(); it != map.end(); ++it){
        std::cout << (*it).first << " : " << (*it).second << '\n';
    }
}


TEST(HashMap, Rehash){
    HashMap< int, int> map(4);
    for (int i = 0; i < 20; ++i){
        map.insert(i, i);
    }
    for (auto it = map.begin(); it != map.end(); ++it){
        std::cout << (*it).first << " : " << (*it).second << '\n';
    }
}


TEST(HashMap, ThreadsInsert){
    using namespace std::chrono_literals;
    std::mutex print_mutex;
    HashMap<int, int> map(4);

    auto print_element = [&](int key, int value){
        const std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "-- " << key << ": " << value << " : size = " << map.size() << '\n'; 
        return;
    };

    auto f1 = [&](HashMap<int, int> &map){
        for (int i = 0; i < 10; ++i){
            map.insert(i, i);
            print_element(i, i);
        }
    };

    auto f2 = [&](HashMap<int, int> &map){
        for (int i = 10; i < 20; ++i){
            map.insert(i, i);
            print_element(i, i);
        }
    };

    std::thread thr1(f1, std::ref(map));
    std::thread thr2(f2, std::ref(map));

    thr1.join();
    thr2.join();

    for (auto it = map.begin(); it != map.end(); ++it){
        std::cout << (*it).first << " : " << (*it).second << '\n';
    }
}