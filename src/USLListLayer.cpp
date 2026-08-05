#include "USLListLayer.hpp"
#include "USLManager.hpp"
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

USLListLayer* USLListLayer::create() {
	auto ret = new USLListLayer();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

CCScene* USLListLayer::scene() {
	auto ret = CCScene::create();
	AppDelegate::get()->m_runningScene = ret;
	ret->addChild(USLListLayer::create());
	return ret;
}

bool USLListLayer::init() {
	if (!CCLayer::init()) return false;

	setID("USLListLayer");
	auto winSize = CCDirector::get()->getWinSize();
	m_levelsPerPage = (int)Mod::get()->getSettingValue<int64_t>("levels-per-page");

	auto bg = CCSprite::create("GJ_gradientBG.png");
	bg->setAnchorPoint({ 0.0f, 0.0f });
	bg->setScaleX((winSize.width + 10.0f) / bg->getTextureRect().size.width);
	bg->setScaleY((winSize.height + 10.0f) / bg->getTextureRect().size.height);
	bg->setPosition({ -5.0f, -5.0f });
	bg->setColor({ 29, 82, 148 });
	bg->setID("background");
	addChild(bg);

	auto bottomLeftCorner = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
	bottomLeftCorner->setPosition({ -1.0f, -1.0f });
	bottomLeftCorner->setAnchorPoint({ 0.0f, 0.0f });
	bottomLeftCorner->setID("left-corner");
	addChild(bottomLeftCorner);

	auto bottomRightCorner = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
	bottomRightCorner->setPosition({ winSize.width + 1.0f, -1.0f });
	bottomRightCorner->setAnchorPoint({ 1.0f, 0.0f });
	bottomRightCorner->setFlipX(true);
	bottomRightCorner->setID("right-corner");
	addChild(bottomRightCorner);

	auto logo = CCSprite::create("usl-logo.png"_spr);
	logo->setScale(0.28f);
	logo->setPosition({ winSize.width / 2.0f, winSize.height - 45.0f });
	logo->setID("usl-logo"_spr);
	addChild(logo);

	m_countLabel = CCLabelBMFont::create("", "goldFont.fnt");
	m_countLabel->setAnchorPoint({ 1.0f, 1.0f });
	m_countLabel->setScale(0.6f);
	m_countLabel->setPosition({ winSize.width - 7.0f, winSize.height - 3.0f });
	m_countLabel->setID("level-count-label");
	addChild(m_countLabel);

	m_list = GJListLayer::create(nullptr, "Ultimate Shitty List", { 0, 0, 0, 180 }, 356.0f, 220.0f, 0);
	m_list->setPosition(winSize / 2.0f - m_list->getContentSize() / 2.0f);
	m_list->setID("GJListLayer");
	addChild(m_list, 2);

	auto menu = CCMenu::create();
	menu->setPosition({ 0.0f, 0.0f });
	menu->setID("button-menu");
	addChild(menu);

	auto backButton = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(USLListLayer::onBack)
	);
	backButton->setPosition({ 25.0f, winSize.height - 25.0f });
	backButton->setID("back-button");
	menu->addChild(backButton);

	auto leftBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	m_leftButton = CCMenuItemSpriteExtra::create(leftBtnSpr, this, menu_selector(USLListLayer::onPrevPage));
	m_leftButton->setPosition({ 24.0f, winSize.height / 2.0f });
	m_leftButton->setID("prev-page-button");
	menu->addChild(m_leftButton);

	auto rightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	rightBtnSpr->setFlipX(true);
	m_rightButton = CCMenuItemSpriteExtra::create(rightBtnSpr, this, menu_selector(USLListLayer::onNextPage));
	m_rightButton->setPosition({ winSize.width - 24.0f, winSize.height / 2.0f });
	m_rightButton->setID("next-page-button");
	menu->addChild(m_rightButton);

	auto refreshBtnSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	auto refreshButton = CCMenuItemSpriteExtra::create(refreshBtnSpr, this, menu_selector(USLListLayer::onRefresh));
	refreshButton->setPosition({ winSize.width - refreshBtnSpr->getContentWidth() / 2.0f - 4.0f, refreshBtnSpr->getContentHeight() / 2.0f + 4.0f });
	refreshButton->setID("refresh-button");
	menu->addChild(refreshButton, 2);

	auto pageBtnSpr = CCSprite::create("GJ_button_02.png");
	pageBtnSpr->setScale(0.7f);
	m_pageLabel = CCLabelBMFont::create("1", "bigFont.fnt");
	m_pageLabel->setScale(0.8f);
	m_pageLabel->setPosition(pageBtnSpr->getContentSize() / 2.0f);
	pageBtnSpr->addChild(m_pageLabel);
	m_pageButton = CCMenuItemSpriteExtra::create(pageBtnSpr, this, menu_selector(USLListLayer::onPage));
	m_pageButton->setPositionY(winSize.height - 39.5f);
	m_pageButton->setID("page-button");
	menu->addChild(m_pageButton);

	auto lastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	lastArrow->setFlipX(true);
	auto otherLastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherLastArrow->setPosition(lastArrow->getContentSize() / 2.0f + CCPoint { 20.0f, 0.0f });
	otherLastArrow->setFlipX(true);
	lastArrow->addChild(otherLastArrow);
	lastArrow->setScale(0.4f);
	m_lastButton = CCMenuItemSpriteExtra::create(lastArrow, this, menu_selector(USLListLayer::onLast));
	m_lastButton->setPositionY(
		m_pageButton->getPositionY() - m_pageButton->getContentHeight() / 2.0f - m_lastButton->getContentHeight() / 2.0f - 5.0f);
	m_lastButton->setID("last-button");
	menu->addChild(m_lastButton);

	auto x = winSize.width - m_pageButton->getContentWidth() / 2.0f - 3.0f;
	m_pageButton->setPositionX(x);
	m_lastButton->setPositionX(x - 4.0f);

	auto firstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	auto otherFirstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherFirstArrow->setPosition(firstArrow->getContentSize() / 2.0f - CCPoint { 20.0f, 0.0f });
	firstArrow->addChild(otherFirstArrow);
	firstArrow->setScale(0.4f);
	m_firstButton = CCMenuItemSpriteExtra::create(firstArrow, this, menu_selector(USLListLayer::onFirst));
	m_firstButton->setPosition({ 21.5f, m_lastButton->getPositionY() });
	m_firstButton->setID("first-button");
	menu->addChild(m_firstButton);

	m_failure = [this](int code) {
		FLAlertLayer::create(fmt::format("Load Failed ({})", code).c_str(), "Failed to load the USL level list. Please try again later.", "OK")->show();
		m_loadingCircle->setVisible(false);
	};

	m_loadingCircle = LoadingCircle::create();
	m_loadingCircle->setParentLayer(this);
	m_loadingCircle->setID("loading-circle");
	m_loadingCircle->show();

	showLoading();
	setKeypadEnabled(true);
	setKeyboardEnabled(true);

	if (USLManager::isLoaded()) {
		populateList();
	}
	else {
		USLManager::load(m_listener, [this] {
			populateList();
		}, m_failure);
	}

	return true;
}

void USLListLayer::onBack(CCObject* sender) {
	CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void USLListLayer::onPrevPage(CCObject* sender) {
	page(m_page - 1);
}

void USLListLayer::onNextPage(CCObject* sender) {
	page(m_page + 1);
}

void USLListLayer::onRefresh(CCObject* sender) {
	showLoading();
	USLManager::load(m_listener, [this] {
		m_page = 0;
		populateList();
	}, m_failure);
}

void USLListLayer::onPage(CCObject* sender) {
	auto totalPages = std::max<int>(1, (int)std::ceil((double)USLManager::levels().size() / m_levelsPerPage));
	auto popup = SetIDPopup::create(m_page + 1, 1, totalPages, "Go to Page", "Go", true, 1, 60.0f, false, false);
	popup->m_delegate = this;
	popup->show();
}

void USLListLayer::onFirst(CCObject* sender) {
	page(0);
}

void USLListLayer::onLast(CCObject* sender) {
	page((int)std::ceil((double)USLManager::levels().size() / m_levelsPerPage) - 1);
}

void USLListLayer::showLoading() {
	m_pageLabel->setString(fmt::to_string(m_page + 1).c_str());
	m_loadingCircle->setVisible(true);
	if (auto listView = m_list->m_listView) listView->setVisible(false);
	m_countLabel->setVisible(false);
	m_leftButton->setVisible(false);
	m_rightButton->setVisible(false);
	m_firstButton->setVisible(false);
	m_lastButton->setVisible(false);
	m_pageButton->setVisible(false);
}

void USLListLayer::populateList() {
	std::vector<std::string> ids;
	auto levels = USLManager::levels();
	for (int i = m_page * m_levelsPerPage; i < (int)levels.size() && i < (m_page + 1) * m_levelsPerPage; ++i) {
		ids.push_back(levels[i].levelId);
	}

	if (ids.empty()) {
		loadLevelsFinished(CCArray::create(), "", 0);
		m_countLabel->setString("");
		return;
	}

	auto glm = GameLevelManager::get();
	glm->m_levelManagerDelegate = this;

	auto searchObject = GJSearchObject::create(SearchType::Type19);
	std::string query;
	for (size_t i = 0; i < ids.size(); ++i) {
		if (i > 0) query += ',';
		query += ids[i];
	}
	searchObject->m_searchQuery = query;

	if (auto storedLevels = glm->getStoredOnlineLevels(searchObject->getKey())) {
		loadLevelsFinished(storedLevels, "", 0);
		setupPageInfo("", "");
	}
	else {
		glm->getOnlineLevels(searchObject);
	}
}

void USLListLayer::loadLevelsFinished(CCArray* levels, const char*, int) {
	if (auto listView = m_list->m_listView) {
		listView->removeFromParent();
		listView->release();
	}

	auto listView = CustomListView::create(levels, BoomListType::Level, 190.0f, 356.0f);
	listView->retain();
	m_list->addChild(listView, 6, 9);
	m_list->m_listView = listView;

	m_countLabel->setVisible(true);
	m_loadingCircle->setVisible(false);
	auto size = USLManager::levels().size();
	if (size > (size_t)m_levelsPerPage) {
		auto maxPage = (int)std::ceil((double)size / m_levelsPerPage) - 1;
		m_leftButton->setVisible(m_page > 0);
		m_rightButton->setVisible(m_page < maxPage);
		m_firstButton->setVisible(m_page > 0);
		m_lastButton->setVisible(m_page < maxPage);
		m_pageButton->setVisible(true);
	}
}

void USLListLayer::loadLevelsFailed(const char*, int) {
	m_countLabel->setVisible(true);
	m_loadingCircle->setVisible(false);
	FLAlertLayer::create("Load Failed", "Failed to load levels. Please try again later.", "OK")->show();
}

void USLListLayer::setupPageInfo(gd::string, const char*) {
	auto size = USLManager::levels().size();
	m_countLabel->setString(fmt::format("{} to {} of {}", m_page * m_levelsPerPage + 1,
		std::min<int>((int)size, (m_page + 1) * m_levelsPerPage), size).c_str());
	m_countLabel->limitLabelWidth(100.0f, 0.6f, 0.0f);
}

void USLListLayer::page(int page) {
	auto maxPage = (int)std::ceil((double)USLManager::levels().size() / m_levelsPerPage);
	m_page = maxPage > 0 ? (maxPage + (page % maxPage)) % maxPage : 0;
	showLoading();
	populateList();
}

void USLListLayer::keyDown(enumKeyCodes key, double timestamp) {
	switch (key) {
		case KEY_Left:
		case CONTROLLER_Left:
			if (m_leftButton->isVisible()) page(m_page - 1);
			break;
		case KEY_Right:
		case CONTROLLER_Right:
			if (m_rightButton->isVisible()) page(m_page + 1);
			break;
		default:
			CCLayer::keyDown(key, timestamp);
			break;
	}
}

void USLListLayer::keyBackClicked() {
	onBack(nullptr);
}

void USLListLayer::setIDPopupClosed(SetIDPopup*, int page) {
	auto maxPage = (int)std::ceil((double)USLManager::levels().size() / m_levelsPerPage);
	m_page = std::clamp<int>(page - 1, 0, maxPage - 1);
	showLoading();
	populateList();
}

USLListLayer::~USLListLayer() {
	auto glm = GameLevelManager::get();
	if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
}
