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
		auto downloadsIcon = getChildByID("downloads-icon");
		auto lengthIcon = getChildByID("length-icon");
		if (!downloadsIcon || !m_downloadsLabel || !lengthIcon || !m_lengthLabel ||
			!m_likesIcon || !m_likesLabel || !m_orbsIcon || !m_orbsLabel) return;

		downloadsIcon->setPositionY(downloadsIcon->getPositionY() + 10.0f);
		m_downloadsLabel->setPositionY(m_downloadsLabel->getPositionY() + 10.0f);
		m_likesIcon->setPositionY(m_likesIcon->getPositionY() + 12.0f);
		m_likesLabel->setPositionY(m_likesLabel->getPositionY() + 12.0f);
		lengthIcon->setPositionY(lengthIcon->getPositionY() + 14.0f);
		m_lengthLabel->setPositionY(m_lengthLabel->getPositionY() + 14.0f);
		m_exactLengthLabel->setPositionY(m_exactLengthLabel->getPositionY() + 14.0f);
		m_orbsIcon->setPositionY(m_orbsIcon->getPositionY() + 16.0f);
		m_orbsLabel->setPositionY(m_orbsLabel->getPositionY() + 16.0f);

		float iconSize = 23.0f;
		float iconX = lengthIcon->getPositionX() + lengthIcon->getContentWidth() / 2.0f;
		float iconY = m_orbsIcon->isVisible()
			? m_orbsIcon->getPositionY() - (downloadsIcon->getPositionY() - m_likesIcon->getPositionY())
			: lengthIcon->getPositionY() - (m_likesIcon->getPositionY() - lengthIcon->getPositionY());

		auto label = CCLabelBMFont::create(fmt::format("#{}", rank).c_str(), "bigFont.fnt");
		label->setScale(0.5f);
		label->setAnchorPoint({ 0.0f, 0.5f });
		label->setPosition({ iconX, iconY });
		label->setColor({ 255, 215, 90 });
		label->setID("usl-rank-badge"_spr);
		addChild(label, 10);

		auto icon = CCSprite::create("usl-logo-round.png"_spr);
		icon->setScale(iconSize / icon->getContentWidth());
		icon->setPosition({
			label->getPositionX() + label->getScaledContentWidth() + 4.0f + iconSize / 2.0f, iconY
		});
		icon->setID("usl-rank-icon"_spr);
		addChild(icon, 10);
	}
};
