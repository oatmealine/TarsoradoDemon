#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <vector>
#include <string>
#include "ListManager.h"

using namespace geode::prelude;

class $modify(GrDInfoLayer, LevelInfoLayer) {
    bool m_hasBeenOpened = false;

    void updateDifficultyFace() {

        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);

        // Get the original difficulty icon
        CCSprite* originalIcon = nullptr;
        bool iconFound = false;

        // Iterate through every object that is a direct child of the layer to find the difficulty face.
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            // Check to see if the object is a sprite.
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(obj)) {
                // Check to see if the object is the demon difficulty icon
                // Note that the child-index "stars-icon" doesn't appear to work all the time.
                // Instead of using an absolute index, get the object that fits the following criteria:
                if (newObj->getPosition() == m_difficultySprite->getPosition()
                && newObj->getZOrder() == 3) {
                    //newObj->setColor({0, 255, 0});
                    originalIcon = newObj;
                    iconFound = true;
                    break;
                }
            }
        }

        // If the demon face somehow isn't found, notify the user.
        if (originalIcon == nullptr || !iconFound) {
            auto alert = FLAlertLayer::create("Error", "There was a problem loading the demon difficulty face.\nYour sceen resolution may not be supported.\n\n<cb>-Grandpa Demon</c>", "OK");
            alert->m_scene = this;
            alert->show();
            return;
        }

        CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, true);
        if (newIcon == nullptr) return;

        //CCSprite* newIcon = CCSprite::createWithSpriteFrameName("GrD_demon0_text.png"_spr);
        newIcon->setID("grd-difficulty");
        
        auto newPos = originalIcon->getPosition();
        newIcon->setPosition(originalIcon->getPosition());
        newIcon->setZOrder(originalIcon->getZOrder()+10);
        

        CCObject* clearObj;
        CCARRAY_FOREACH(originalIcon->getChildren(), clearObj) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(clearObj)) {
                if (newObj->getTag() == 69420) {
                    newObj->removeFromParentAndCleanup(true);
                }
            }
        }

        CCObject* iconObj;
        CCARRAY_FOREACH(originalIcon->getChildren(), iconObj) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(iconObj)) {
                newObj->setTag(69420);
                this->addChild(newObj);
                newObj->setPosition(newPos);
            }
        }

        originalIcon->setVisible(false);

        this->addChild(newIcon);
        
        if (m_fields->m_hasBeenOpened) {
            return;
        }

        m_fields->m_hasBeenOpened = true;
        return;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        if (ListManager::demonIDList.size() == 0) {
            return;
        }

        if (this->m_level->m_stars != 10) {
            return;
        }
        
        updateDifficultyFace();
    }
};