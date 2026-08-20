#include "USLManager.hpp"
#include "ApiUrl.hpp"
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/VersionInfo.hpp>

using namespace geode::prelude;

static std::vector<USLDemon> s_levels;
static bool s_loaded = false;

std::vector<USLDemon>& USLManager::levels() {
	return s_levels;
}

bool USLManager::isLoaded() {
	return s_loaded;
}

std::string USLManager::apiUrl() {
	auto url = Mod::get()->getSettingValue<std::string>("api-url");
	auto resolved = usl::resolveApiUrl(url);
	if (resolved != url) {
		Mod::get()->setSettingValue("api-url", resolved);
	}
	return resolved;
}

USLDemon const* USLManager::findByLevelId(std::string const& levelId) {
	for (auto& demon : s_levels) {
		if (demon.levelId == levelId) return &demon;
	}
	return nullptr;
}

void USLManager::checkForUpdate(TaskHolder<web::WebResponse>& listener, Function<void(std::string, std::string)> available) {
	listener.spawn(
		web::WebRequest()
			.userAgent("wayago.usl-integration")
			.header("Accept", "application/vnd.github+json")
			.get("https://api.github.com/repos/wayago-dev/usl-integration/releases/latest"),
		[available = std::move(available)](web::WebResponse res) mutable {
			if (!res.ok()) return;

			auto json = res.json();
			if (!json.isOk()) return;

			auto release = std::move(json).unwrap();
			auto tagName = release.get<std::string>("tag_name");
			if (!tagName.isOk()) return;

			auto version = VersionInfo::parse(tagName.unwrap());
			if (!version.isOk() || version.unwrap() <= Mod::get()->getVersion()) return;

			for (auto& asset : release["assets"]) {
				auto downloadUrl = asset.get<std::string>("browser_download_url");
				if (downloadUrl.isOk() && downloadUrl.unwrap().ends_with(".geode")) {
					available(tagName.unwrap(), downloadUrl.unwrap());
					return;
				}
			}
		}
	);
}

void USLManager::load(TaskHolder<web::WebResponse>& listener, Function<void()> success, CopyableFunction<void(int)> failure) {
	listener.spawn(
		web::WebRequest().get(apiUrl()),
		[failure = std::move(failure), success = std::move(success)](web::WebResponse res) mutable {
			if (!res.ok()) {
				return failure(res.code());
			}

			auto json = res.json();
			if (!json.isOk() || !json.unwrap().contains("levels")) {
				return failure(res.code());
			}

			s_levels.clear();
			for (auto& level : json.unwrap()["levels"]) {
				auto rank = level.get<int>("rank");
				auto levelId = level.get<std::string>("level_id");
				auto name = level.get<std::string>("name");
				if (!rank.isOk() || !levelId.isOk() || !name.isOk()) continue;

				USLDemon demon;
				demon.rank = rank.unwrap();
				demon.levelId = std::move(levelId).unwrap();
				demon.name = std::move(name).unwrap();

				auto creator = level.get<std::string>("creator");
				auto difficulty = level.get<std::string>("difficulty");
				auto points = level.get<int>("points");
				demon.creator = creator.isOk() ? creator.unwrap() : "";
				demon.difficulty = difficulty.isOk() ? difficulty.unwrap() : "";
				demon.points = points.isOk() ? points.unwrap() : 0;
				s_levels.push_back(std::move(demon));
			}

			std::sort(s_levels.begin(), s_levels.end(), [](USLDemon const& a, USLDemon const& b) {
				return a.rank < b.rank;
			});

			s_loaded = true;
			success();
		}
	);
}
