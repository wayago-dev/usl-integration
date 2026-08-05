#pragma once
#include <Geode/utils/web.hpp>
#include <string>
#include <vector>

struct USLDemon {
	int rank = 0;
	int points = 0;
	std::string levelId;
	std::string name;
	std::string creator;
	std::string difficulty;
};

namespace USLManager {
	std::vector<USLDemon>& levels();
	bool isLoaded();
	std::string apiUrl();
	USLDemon const* findByLevelId(std::string const& levelId);

	void load(
		geode::async::TaskHolder<geode::utils::web::WebResponse>& listener,
		geode::Function<void()> success,
		geode::CopyableFunction<void(int)> failure
	);
}
