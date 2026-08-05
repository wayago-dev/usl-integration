#include "../USLManager.hpp"
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/utils/NodeIDs.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

class $modify(USLLevelCell, LevelCell) {
	static void onModify(auto& self) {
		self.setHookPriority("LevelCell::loadFromLevel", GEODE_ID_PRIORITY + 10);
	}

	void loadFromLevel(GJGameLevel* level) {
		LevelCell::loadFromLevel(level);

		if (auto badge = getChildByID("usl-rank-icon"_spr)) badge->removeFromParent();
		if (auto badge = getChildByID("usl-rank-label"_spr)) badge->removeFromParent();

		if (!level || level->m_levelType == GJLevelType::Main || level->m_levelType == GJLevelType::Editor) return;

		auto demon = USLManager::findByLevelId(std::to_string(level->m_levelID.value()));
		if (!demon) return;

		auto mainLayer = getChildByID("main-layer");
		auto likesLabel = mainLayer ? mainLayer->getChildByID("likes-label") : nullptr;
		auto orbsLabel = mainLayer ? mainLayer->getChildByID("orbs-label") : nullptr;
		if (!mainLayer || !likesLabel) return;

		float y = m_compactView ? 8.5f : 14.0f;
		float iconSize = m_compactView ? 9.2f : 13.8f;

		auto icon = CCSprite::create("usl-logo.png"_spr);
		icon->setScale(iconSize / icon->getContentWidth());
		icon->setPosition({ likesLabel->getPositionX() + likesLabel->getScaledContentWidth() + 9.0f, y });
		icon->setID("usl-rank-icon"_spr);
		mainLayer->addChild(icon);

		auto label = CCLabelBMFont::create(fmt::format("#{}", demon->rank).c_str(), "bigFont.fnt");
		label->setScale(m_compactView ? 0.3f : 0.4f);
		label->setAnchorPoint({ 0.0f, 0.5f });
		label->setPosition({ icon->getPositionX() + icon->getScaledContentWidth() / 2.0f + (m_compactView ? 5.4f : 3.1f), y });
		label->setColor({ 255, 215, 90 });
		label->setID("usl-rank-label"_spr);
		mainLayer->addChild(label);

		float end = label->getPositionX() + 50.0f;
		if (end > 350.0f && orbsLabel) {
			float gap = (end - 350.0f) / 3.0f;
			auto shift = [&](char const* id, float amount) {
				if (auto node = mainLayer->getChildByID(id)) node->setPositionX(node->getPositionX() - amount);
			};
			shift("downloads-icon", gap);
			shift("downloads-label", gap);
			shift("likes-icon", gap * 2.0f);
			shift("likes-label", gap * 2.0f);
			shift("orbs-icon", gap * 3.0f);
			shift("orbs-label", gap * 3.0f);
			icon->setPositionX(icon->getPositionX() - gap * 3.0f);
			label->setPositionX(label->getPositionX() - gap * 3.0f);
		}
	}
};
