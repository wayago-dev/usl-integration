#pragma once

#include <string>

namespace usl {
	inline std::string resolveApiUrl(std::string url) {
		if (url == "https://ultimateshittylist.space/api/levels") {
			return "https://ultimateshittylist.fun/api/levels";
		}
		return url;
	}
}
