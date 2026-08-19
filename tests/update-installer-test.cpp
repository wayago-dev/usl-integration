#include "../src/UpdateInstaller.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
	auto directory = std::filesystem::temp_directory_path() / "usl-update-installer-test";
	std::filesystem::remove_all(directory);
	std::filesystem::create_directories(directory);

	auto installed = directory / "wayago.usl-integration.geode";
	auto downloaded = directory / "wayago.usl-integration-v1.7.3.geode";
	std::ofstream(installed) << "old";
	std::ofstream(downloaded) << "new";

	assert(usl::replacePackage(installed, downloaded));
	assert(!std::filesystem::exists(downloaded));

	std::ifstream file(installed);
	assert(std::string(std::istreambuf_iterator<char>(file), {}) == "new");

	std::filesystem::remove_all(directory);
}
