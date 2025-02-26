#ifndef LISTMANAGER_H
#define LISTMANAGER_H
#include <algorithm>
#include <cstddef>
#include <tuple>
#include <vector>
#include <string>

using namespace geode::prelude;

class ListManager {
    public:
        inline static std::vector<int> demonIDList;
        inline static bool firstTimeOpen;
        inline static int filterType;
        inline static bool isSupremeSearching;

        inline static void parseRequestString(std::string str) {
            size_t isFound = str.find("_id");

            while (isFound != std::string::npos) {
                str = str.substr(isFound + 5);
                size_t findBracket = str.find("}");

                int id = stoi(str.substr(0, findBracket));
                demonIDList.push_back(id);

                isFound = str.find("_id");
            }
        }

        inline static int getPositionOfID(int id) {
            for (unsigned int i = 0; i < demonIDList.size(); i++) {
                if (demonIDList.at(i) == id) return i;
            }

            return -1;
        }

        // returns null if no sprite found
        inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
            if (pos == -1) return nullptr;

            float div = (float) (pos - 150) / (float) (demonIDList.size() - 150);

            int index = 0;
            
            if (pos == 0) {
                index = 20;
            } else if (pos < 10) {
                index = 19;
            } else if (pos < 25) {
                index = 18;
            } else if (pos < 50) {
                index = 17;
            } else if (pos < 100) {
                index = 16;
            } else if (pos < 150) {
                index = 15;
            } else {
                index = static_cast<int>(15 - (div * 16) + 1) - 1;
            }

            if (index <= 0) return nullptr;

            // just in case
            index = std::clamp(index, 0, 20);

            std::string spriteName = "GrD_demon" + std::to_string(index) + (hasText ? "_text" : "") + ".png";
            return CCSprite::createWithSpriteFrameName(geode::Mod::get()->expandSpriteName(spriteName.c_str()));
        }
        inline static std::tuple<int, int> getRange(int index) {
            if (index == 20) return {0, 0};
            if (index == 19) return {1, 9};
            if (index == 18) return {10, 24};
            if (index == 17) return {25, 49};
            if (index == 16) return {50, 99};
            if (index == 15) return {100, 149};

            float divMax = (15.f - index) / 16.f;
            float divMin = (15.f - (index + 1)) / 16.f;
            int size = demonIDList.size() - 150;

            return {divMin * size + 150, divMax * size - 1 + 150};
        }

        inline static GJSearchObject* getSearchObject(int upper, int lower) {
            std::stringstream download;
            bool first = true;
            if (!(upper == 0 && lower == 0)) {
                for (unsigned int i = upper; i > lower; i--) {
                    if (!first) {
                        download << ",";
                    }
                    download << std::to_string(ListManager::demonIDList.at(i));
                    first = false;
                }
            } else {
                download << std::to_string(ListManager::demonIDList.at(0));
            }
            
            download << "&gameVersion=22";
            GJSearchObject* searchObj = GJSearchObject::create(SearchType::Type19, download.str());
            return searchObj;
        }   

        
};

#endif