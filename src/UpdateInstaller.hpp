#pragma once

#include <filesystem>

namespace usl {
	inline bool replacePackage(std::filesystem::path const& installed, std::filesystem::path const& downloaded) {
		std::error_code error;
		std::filesystem::remove(installed, error);
		if (error) return false;

		std::filesystem::rename(downloaded, installed, error);
		return !error;
	}
}
