#include <Geode/Bindings.hpp>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include <string>
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/cocos/base_nodes/Layout.hpp"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "ListManager.h"

class $modify(GrdDemonFilterSelectLayer, DemonFilterSelectLayer) {

    CCSprite* m_extraDemonSpr[21];

    static void onModify(auto &self) {
        self.setHookPriority("DemonFilterSelectLayer::init", -1);
    }

    bool init() {
        DemonFilterSelectLayer::init();
        handleTouchPriority(this);
        CCLayer* layer = nullptr;

        CCObject* layerObj;
        CCARRAY_FOREACH(this->getChildren(), layerObj) {
            if (auto newObj = static_cast<CCLayer*>(layerObj)) {
                layer = newObj;
            }
        }

        // background
        CCScale9Sprite* s9spr = nullptr;
        // "demon filter" label
        CCLabelBMFont* label = nullptr;
        // holds the demons and the "ok" button
        CCMenu* menu = nullptr;
        
        CCObject* obj;
        CCARRAY_FOREACH(layer->getChildren(), obj) {
            if (auto newObj = dynamic_cast<CCScale9Sprite*>(obj)) {
                s9spr = newObj;
            } else if (auto newObj2 = dynamic_cast<CCLabelBMFont*>(obj)) {
                label = newObj2;
            } else if (auto newObj3 = dynamic_cast<CCMenu*>(obj)) {
                menu = newObj3;
            }
        }


        // Find OK Button
        CCMenuItemSpriteExtra* okButton = nullptr;
        CCObject* obj_ok;
        CCARRAY_FOREACH(menu->getChildren(), obj_ok) {
            if (auto newObj = dynamic_cast<CCMenuItemSpriteExtra*>(obj_ok)) {
                if (newObj->getPositionY() < 0) {
                    okButton = newObj;
                    break;
                }
            }
        }

        auto demon = menu->getChildByTag(0);
        auto easy = menu->getChildByTag(7);
        auto medium = menu->getChildByTag(8);
        auto hard = menu->getChildByTag(6);
        auto insane = menu->getChildByTag(9);
        auto extreme = menu->getChildByTag(10);

        /*const auto huh = 0.92f;
        demon->setScale(huh);
        easy->setScale(huh);
        medium->setScale(huh);
        hard->setScale(huh);
        insane->setScale(huh);
        extreme->setScale(huh);
        extreme->m_baseScale = huh;*/

        auto csize = s9spr->getContentSize();
        s9spr->setContentSize({510, 315});
        auto newcsize = s9spr->getContentSize();
        label->setPositionY(newcsize.height - 15.f);
        label->setScale(0.8f);
        menu->setPositionY(newcsize.height * 0.5f);
        
        menu->removeChild(okButton, false);
        okButton->setPosition({0, 0});
        okButton->setScale(0.5f);

        auto okMenu = CCMenu::create();
        layer->addChild(okMenu);
        okMenu->addChild(okButton);
        okMenu->setPosition({label->getPositionX(), 15});

        menu->setLayout(RowLayout::create()
            ->setGap(6.f)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisAlignment(AxisAlignment::Even)
            ->setAutoScale(true)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true));
        
        menu->setContentSize({430, 0});

        handleTouchPriority(this);
        
        // create buttons and sprites

        for (int i = 0; i < 21; i++) {
            std::string spriteName = "GrD_demon" + std::to_string(i) + "_text.png";
            auto sprite = CCSprite::createWithSpriteFrameName(geode::Mod::get()->expandSpriteName(spriteName.c_str()));
            sprite->retain();
            //this->m_fields->m_extraDemonSpr[i] = sprite;
            auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(GrdDemonFilterSelectLayer::onButton));
            button->setTag(i);
            menu->addChild(button);
        }

        menu->updateLayout();
        
        return true;
    }

    void onButton(CCObject* sender) {
        int tag = sender->getTag();

        ListManager::filterType = tag;
        ListManager::isSupremeSearching = true;
        int lower, upper;
        std::tie(lower, upper) = ListManager::getRange(tag);
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(upper, lower));
        geode::cocos::switchToScene(browserLayer);
    }
};