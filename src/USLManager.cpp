#include "USLManager.hpp"
#include <Geode/loader/Mod.hpp>

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
	return Mod::get()->getSettingValue<std::string>("api-url");
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
