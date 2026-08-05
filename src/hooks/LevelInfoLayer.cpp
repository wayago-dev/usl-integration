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

		auto badge = CCLabelBMFont::create(fmt::format("#{}", rank).c_str(), "bigFont.fnt");
		badge->setScale(0.6f);
		badge->setAnchorPoint({ 1.0f, 0.5f });
		auto anchor = title->getAnchorPoint();
		float leftEdge = title->getPositionX() - anchor.x * title->getScaledContentWidth();
		badge->setPosition({ leftEdge - 8.0f, title->getPositionY() });
		badge->setColor({ 255, 215, 90 });
		badge->setID("usl-rank-badge"_spr);
		addChild(badge, 10);
	}
};
