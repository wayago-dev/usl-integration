#include "../USLManager.hpp"
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(USLLevelInfoLayer, LevelInfoLayer) {
	struct Fields {
		async::TaskHolder<web::WebResponse> listener;
		std::string levelId;
	};

	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;
		if (!level) return true;

		m_fields->levelId = std::to_string(level->m_levelID.value());
		if (auto demon = USLManager::findByLevelId(m_fields->levelId)) {
			addRankBadge(demon->rank);
			return true;
		}

		if (!USLManager::isLoaded()) {
			USLManager::load(m_fields->listener, [this] {
				if (auto demon = USLManager::findByLevelId(m_fields->levelId)) {
					addRankBadge(demon->rank);
				}
			}, [](int) {});
		}

		return true;
	}

	void addRankBadge(int rank) {
		auto title = getChildByID("title-label");
		if (!title) return;

		auto anchor = title->getAnchorPoint();
		float leftEdge = title->getPositionX() - anchor.x * title->getScaledContentWidth();
		float iconSize = 28.0f;
		float titleY = title->getPositionY();

		auto badge = CCLabelBMFont::create(fmt::format("#{}", rank).c_str(), "bigFont.fnt");
		badge->setScale(0.6f);
		badge->setAnchorPoint({ 1.0f, 0.5f });
		badge->setPosition({ leftEdge - 8.0f - iconSize - 4.0f, titleY });
		badge->setColor({ 255, 215, 90 });
		badge->setID("usl-rank-badge"_spr);
		addChild(badge, 10);

		auto icon = CCSprite::create("usl-logo-round.png"_spr);
		icon->setScale(iconSize / icon->getContentWidth());
		icon->setPosition({ leftEdge - 8.0f - iconSize / 2.0f, titleY });
		icon->setID("usl-rank-icon"_spr);
		addChild(icon, 10);
	}
};
