#include "../src/ApiUrl.hpp"

#include <cassert>
#include <string>

int main() {
	assert(usl::resolveApiUrl("https://ultimateshittylist.space/api/levels") ==
	       "https://ultimateshittylist.fun/api/levels");
	assert(usl::resolveApiUrl("https://example.com/api/levels") ==
	       "https://example.com/api/levels");
}
