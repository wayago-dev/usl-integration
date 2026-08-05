#include "USLListLayer.hpp"
#include "USLManager.hpp"
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/TextInput.hpp>

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

	auto bg = CCSprite::create("usl-bg.png"_spr);
	bg->setAnchorPoint({ 0.5f, 0.5f });
	auto bgSize = bg->getTextureRect().size;
	bg->setScale(std::max(winSize.width / bgSize.width, winSize.height / bgSize.height));
	bg->setPosition(winSize / 2.0f);
	bg->setID("background");
	addChild(bg);

	auto dim = CCLayerColor::create({ 0, 0, 0, 110 }, winSize.width, winSize.height);
	dim->setID("background-dim");
	addChild(dim);

	m_countLabel = CCLabelBMFont::create("", "goldFont.fnt");
	m_countLabel->setAnchorPoint({ 1.0f, 1.0f });
	m_countLabel->setScale(0.6f);
	m_countLabel->setPosition({ winSize.width - 7.0f, winSize.height - 3.0f });
	m_countLabel->setID("level-count-label");
	addChild(m_countLabel);

	m_listFrame = NineSlice::create("GJ_square02.png", {}, { 16.0f, 16.0f, 16.0f, 16.0f });
	m_listFrame->setAnchorPoint({ 0.5f, 0.5f });
	m_listFrame->setContentSize({ 356.0f, 220.0f });
	m_listFrame->setPosition(winSize / 2.0f);
	m_listFrame->setID("list-frame");
	addChild(m_listFrame, 2);

	auto headerLogo = CCSprite::create("usl-logo-round.png"_spr);
	headerLogo->setScale(0.055f);
	headerLogo->setPosition({ 24.0f, 200.0f });
	headerLogo->setID("header-logo"_spr);
	m_listFrame->addChild(headerLogo);

	auto titleLabel = CCLabelBMFont::create("Ultimate Shitty List", "bigFont.fnt");
	titleLabel->setScale(0.6f);
	titleLabel->setAnchorPoint({ 0.0f, 0.5f });
	titleLabel->setPosition({ 42.0f, 200.0f });
	titleLabel->setID("list-title");
	m_listFrame->addChild(titleLabel);

	auto pageBtnSpr = CCSprite::create("GJ_button_02.png");
	pageBtnSpr->setScale(0.5f);
	m_pageLabel = CCLabelBMFont::create("1", "bigFont.fnt");
	m_pageLabel->setScale(0.55f);
	m_pageLabel->setPosition(pageBtnSpr->getContentSize() / 2.0f);
	pageBtnSpr->addChild(m_pageLabel);
	m_pageButton = CCMenuItemSpriteExtra::create(pageBtnSpr, this, menu_selector(USLListLayer::onPage));
	m_pageButton->setPosition({ 331.0f, 200.0f });
	m_pageButton->setID("page-button");
	auto barMenu = CCMenu::create();
	barMenu->setPosition({ 0.0f, 0.0f });
	barMenu->setID("page-menu");
	barMenu->addChild(m_pageButton);
	m_listFrame->addChild(barMenu);

	m_searchBarMenu = CCNode::create();
	m_searchBarMenu->setPosition({ 0.0f, 0.0f });
	m_searchBarMenu->setID("search-bar-menu");
	m_listFrame->addChild(m_searchBarMenu);

	auto searchBg = CCLayerColor::create({ 29, 82, 148, 255 }, 356.0f, 26.0f);
	searchBg->setAnchorPoint({ 0.0f, 0.0f });
	searchBg->setPosition({ 0.0f, 170.0f });
	searchBg->setID("search-bar-background");
	m_searchBarMenu->addChild(searchBg);

	auto searchMenu = CCMenu::create();
	searchMenu->setPosition({ 0.0f, 0.0f });
	searchMenu->setID("search-menu");
	m_searchBarMenu->addChild(searchMenu);

	m_searchBar = TextInput::create(290.0f, "Search levels...");
	m_searchBar->setMaxCharCount(32);
	m_searchBar->setTextAlign(TextInputAlign::Left);
	m_searchBar->getInputNode()->setLabelPlaceholderScale(0.6f);
	m_searchBar->getInputNode()->setMaxLabelScale(0.6f);
	m_searchBar->setPosition({ 148.0f, 183.0f });
	m_searchBar->setID("search-bar");
	searchMenu->addChild(m_searchBar);

	m_searchButton = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(USLListLayer::onSearch)
	);
	m_searchButton->setScale(0.7f);
	m_searchButton->setPosition({ 330.0f, 183.0f });
	m_searchButton->setID("search-button");
	searchMenu->addChild(m_searchButton);

	m_noResultsLabel = CCLabelBMFont::create("No results found", "bigFont.fnt");
	m_noResultsLabel->setScale(0.5f);
	m_noResultsLabel->setPosition({ 178.0f, 90.0f });
	m_noResultsLabel->setVisible(false);
	m_noResultsLabel->setID("no-results-label");
	m_listFrame->addChild(m_noResultsLabel);

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

	auto lastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	lastArrow->setFlipX(true);
	auto otherLastArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherLastArrow->setPosition(lastArrow->getContentSize() / 2.0f + CCPoint { 20.0f, 0.0f });
	otherLastArrow->setFlipX(true);
	lastArrow->addChild(otherLastArrow);
	lastArrow->setScale(0.4f);
	m_lastButton = CCMenuItemSpriteExtra::create(lastArrow, this, menu_selector(USLListLayer::onLast));
	m_lastButton->setPosition({ winSize.width - 21.5f, 21.5f });
	m_lastButton->setID("last-button");
	menu->addChild(m_lastButton);

	auto firstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	auto otherFirstArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	otherFirstArrow->setPosition(firstArrow->getContentSize() / 2.0f - CCPoint { 20.0f, 0.0f });
	firstArrow->addChild(otherFirstArrow);
	firstArrow->setScale(0.4f);
	m_firstButton = CCMenuItemSpriteExtra::create(firstArrow, this, menu_selector(USLListLayer::onFirst));
	m_firstButton->setPosition({ 21.5f, 21.5f });
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
	auto totalPages = std::max<int>(1, (int)std::ceil((double)m_totalLevels / m_levelsPerPage));
	auto popup = SetIDPopup::create(m_page + 1, 1, totalPages, "Go to Page", "Go", true, 1, 60.0f, false, false);
	popup->m_delegate = this;
	popup->show();
}

void USLListLayer::onFirst(CCObject* sender) {
	page(0);
}

void USLListLayer::onLast(CCObject* sender) {
	page((int)std::ceil((double)m_totalLevels / m_levelsPerPage) - 1);
}

void USLListLayer::showLoading() {
	if (m_pageLabel) m_pageLabel->setString(fmt::to_string(m_page + 1).c_str());
	m_loadingCircle->setVisible(true);
	if (m_listView) m_listView->setVisible(false);
	m_countLabel->setVisible(false);
	m_leftButton->setVisible(false);
	m_rightButton->setVisible(false);
	m_firstButton->setVisible(false);
	m_lastButton->setVisible(false);
	if (m_pageButton) m_pageButton->setVisible(false);
	m_searchBarMenu->setVisible(false);
	m_noResultsLabel->setVisible(false);
}

void USLListLayer::onSearch(CCObject* sender) {
	m_query = string::toLower(m_searchBar->getString());
	m_page = 0;
	showLoading();
	populateList();
}

void USLListLayer::populateList() {
	std::vector<std::string> ids;
	for (auto& demon : USLManager::levels()) {
		if (!m_query.empty() && !string::toLower(demon.name).contains(m_query)) continue;
		ids.push_back(demon.levelId);
	}
	m_totalLevels = (int)ids.size();

	if (ids.empty()) {
		m_noResultsLabel->setVisible(true);
		m_searchBarMenu->setVisible(true);
		m_countLabel->setVisible(false);
		m_loadingCircle->setVisible(false);
		if (m_listView) {
			m_listView->removeFromParent();
			m_listView->release();
			m_listView = nullptr;
		}
		if (m_pageButton) m_pageButton->setVisible(false);
		return;
	}

	auto glm = GameLevelManager::get();
	glm->m_levelManagerDelegate = this;

	auto searchObject = GJSearchObject::create(SearchType::Type19);
	std::string query;
	for (int i = m_page * m_levelsPerPage; i < m_totalLevels && i < (m_page + 1) * m_levelsPerPage; ++i) {
		if (i > m_page * m_levelsPerPage) query += ',';
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
	if (m_listView) {
		m_listView->removeFromParent();
		m_listView->release();
		m_listView = nullptr;
	}

	auto listView = CustomListView::create(levels, BoomListType::Level, 160.0f, 356.0f);
	listView->setAnchorPoint({ 0.5f, 0.5f });
	listView->retain();
	listView->setPosition({ 178.0f, 90.0f });
	listView->setID("list-view");
	m_listFrame->addChild(listView, 6, 9);
	m_listView = listView;

	m_countLabel->setVisible(true);
	m_loadingCircle->setVisible(false);
	m_searchBarMenu->setVisible(true);
	m_noResultsLabel->setVisible(m_totalLevels == 0);
	if (m_totalLevels > m_levelsPerPage) {
		auto maxPage = (int)std::ceil((double)m_totalLevels / m_levelsPerPage) - 1;
		m_leftButton->setVisible(m_page > 0);
		m_rightButton->setVisible(m_page < maxPage);
		m_firstButton->setVisible(m_page > 0);
		m_lastButton->setVisible(m_page < maxPage);
		if (m_pageButton) m_pageButton->setVisible(true);
	}
	else if (m_pageButton) {
		m_pageButton->setVisible(false);
	}
}

void USLListLayer::loadLevelsFailed(const char*, int) {
	m_countLabel->setVisible(true);
	m_loadingCircle->setVisible(false);
	m_searchBarMenu->setVisible(true);
	FLAlertLayer::create("Load Failed", "Failed to load levels. Please try again later.", "OK")->show();
}

void USLListLayer::setupPageInfo(gd::string, const char*) {
	m_countLabel->setString(fmt::format("{} to {} of {}", m_page * m_levelsPerPage + 1,
		std::min<int>(m_totalLevels, (m_page + 1) * m_levelsPerPage), m_totalLevels).c_str());
	m_countLabel->limitLabelWidth(100.0f, 0.6f, 0.0f);
}

void USLListLayer::page(int page) {
	auto maxPage = (int)std::ceil((double)m_totalLevels / m_levelsPerPage);
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
		case KEY_Enter:
			onSearch(nullptr);
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
	auto maxPage = (int)std::ceil((double)m_totalLevels / m_levelsPerPage);
	m_page = std::clamp<int>(page - 1, 0, maxPage - 1);
	showLoading();
	populateList();
}

USLListLayer::~USLListLayer() {
	auto glm = GameLevelManager::get();
	if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
}
