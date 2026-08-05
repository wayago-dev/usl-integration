#include "../USLListLayer.hpp"
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(USLLevelSearchLayer, LevelSearchLayer) {
	bool init(int searchType) {
		if (!LevelSearchLayer::init(searchType)) return false;

		if (Mod::get()->getSettingValue<bool>("enable-search-button")) {
			auto buttonSprite = CircleButtonSprite::createWithSprite("usl-logo-round.png"_spr, 1.0f, CircleBaseColor::Blue, CircleBaseSize::Medium);
			buttonSprite->getTopNode()->setScale(buttonSprite->getTopNode()->getScale() * 1.25f);
			auto button = CCMenuItemSpriteExtra::create(buttonSprite, this, menu_selector(USLLevelSearchLayer::onOpenUSL));
			button->setID("usl-button"_spr);
			if (auto menu = getChildByID("other-filter-menu")) {
				menu->addChild(button);
				menu->updateLayout();
			}
			else {
				log::warn("other-filter-menu not found, USL button not added");
			}
		}

		return true;
	}

	void onOpenUSL(CCObject*) {
		CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, USLListLayer::scene()));
	}
};
