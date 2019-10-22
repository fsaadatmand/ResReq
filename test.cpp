#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <algorithm>
#include "Date.h"
#include <vector>
#include <list>

SCENARIO("Create a Date object") {
	GIVEN("A date 02/03/2019") {
		Date date("02/02/2019");
		REQUIRE 
SCENARIO("Blob container test") {
	GIVEN("A Blob b of int of size 10") {
		Blob<int> b{21, 53, 84, 91, 23};
		REQUIRE(b.size() == 5);
		WHEN("pop_back() is executed") {
			b.pop_back();
			THEN("size of b is decremented") {
			REQUIRE(b.size() == 4);
			}
		}
		WHEN("push_back(42) is executed") {
			b.push_back(42);
			THEN("the last element is 42 and size of b is incremented") {
			REQUIRE(b.size() == 6);
			REQUIRE(b.back() == 42);
			}
		}
		WHEN("Used in accumulate algorithm") {
			auto ret = std::accumulate(b.begin(), b.end(), 0);
			THEN("the returned value is the sum of all the elements") {
				ret = 273;
			}
		}
	}
}

SCENARIO("Initializing a Blob from a range of iterators") {
	GIVEN("An array of int") {
		int ia[] = {0,1,2,3,4,5,6,7,8,9};
		WHEN("Initizalizing the a Blob from the array") {
			Blob<int> blob(std::begin(ia), std::end(ia));
			THEN("The element of the blob and the array are equal") {
				auto ia_size = sizeof(ia) / sizeof(ia[0]);
				REQUIRE(blob.size() == ia_size);
				REQUIRE(std::equal(blob.begin(), blob.end(), std::begin(ia)));
			}
		}
	}
	GIVEN("A vector of long") {
		std::vector<long> vi = {0,1,2,3,4,5,6,7,8,9};
		WHEN("Initizalizing the a Blob from the array") {
			Blob<int> blob(vi.cbegin(), vi.cend());
			THEN("The element of the blob and the vector are equal") {
				REQUIRE(blob.size() == vi.size());
				REQUIRE(std::equal(blob.begin(), blob.end(), vi.begin()));
			}
		}
	}

	GIVEN("A list of strings") {
		std::list<const char*> w = {"now", "is", "the" , "time"}; 
		WHEN("Initizalizing the a Blob from the list") {
			Blob<std::string> blob(w.cbegin(), w.cend());
			THEN("The element of the blob and the list are equal") {
				REQUIRE(blob.size() == w.size());
				REQUIRE(std::equal(blob.begin(), blob.end(), w.begin()));
			}
		}
	}
}
