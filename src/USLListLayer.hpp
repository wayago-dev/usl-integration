#pragma once
#include <Geode/binding/LevelManagerDelegate.hpp>
#include <Geode/binding/SetIDPopupDelegate.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/web.hpp>

class USLListLayer : public cocos2d::CCLayer, SetIDPopupDelegate, LevelManagerDelegate {
public:
	static USLListLayer* create();
	static cocos2d::CCScene* scene();

	void page(int);
	void keyDown(cocos2d::enumKeyCodes, double) override;
	void keyBackClicked() override;

	~USLListLayer() override;
protected:
	geode::async::TaskHolder<geode::utils::web::WebResponse> m_listener;
	geode::async::TaskHolder<geode::utils::web::WebResponse> m_updateListener;
	geode::async::TaskHolder<geode::utils::web::WebResponse> m_downloadListener;
	GJListLayer* m_list;
	LoadingCircle* m_loadingCircle;
	cocos2d::CCNode* m_searchBarMenu;
	geode::TextInput* m_searchBar;
	CCMenuItemSpriteExtra* m_searchButton;
	cocos2d::CCLabelBMFont* m_noResultsLabel;
	cocos2d::CCLabelBMFont* m_countLabel;
	cocos2d::CCLabelBMFont* m_pageLabel;
	CCMenuItemSpriteExtra* m_leftButton;
	CCMenuItemSpriteExtra* m_rightButton;
	CCMenuItemSpriteExtra* m_pageButton;
	CCMenuItemSpriteExtra* m_firstButton;
	CCMenuItemSpriteExtra* m_lastButton;
	cocos2d::CCNode* m_updateBanner;
	cocos2d::CCLabelBMFont* m_updateLabel;
	CCMenuItemSpriteExtra* m_updateButton;
	geode::CopyableFunction<void(int)> m_failure;
	int m_page = 0;
	int m_levelsPerPage = 10;
	int m_totalLevels = 0;
	std::string m_query;
	std::string m_updateVersion;
	std::string m_updateUrl;

	bool init() override;
	void onBack(cocos2d::CCObject*);
	void onSearch(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onRefresh(cocos2d::CCObject*);
	void onPage(cocos2d::CCObject*);
	void onFirst(cocos2d::CCObject*);
	void onLast(cocos2d::CCObject*);
	void onDownloadUpdate(cocos2d::CCObject*);
	void showLoading();
	void showUpdateBanner(std::string version, std::string downloadUrl);
	bool scheduleUpdateInstall(std::filesystem::path const& downloadedPath);
	void populateList();
	void loadLevelsFinished(cocos2d::CCArray* levels, const char* key, int) override;
	void loadLevelsFailed(const char* key, int) override;
	void setupPageInfo(gd::string, const char*) override;
	void setIDPopupClosed(SetIDPopup*, int) override;
};
