/**
 * @file
 * @ingroup Filter
 * @image html difference.png
 */
#pragma once

#include "filter/IFilter.h"
#include <algorithm>

namespace ai {

/**
 * @brief This filter performs a difference operation between several filter results. The result
 * consists of elements that are in A and not in B, C, D, ...
 */
class Difference: public IFilter {
public:
	FILTER_ACTION_CLASS(Difference)
	FILTER_ACTION_FACTORY(Difference)

	void filter (const AIPtr& entity) override;
};

inline void Difference::filter (const AIPtr& entity) {
	FilteredEntities& filtered = getFilteredEntities(entity);
	// create a copy
	const FilteredEntities alreadyFiltered = filtered;
	// now clear the entity list
	filtered.clear();

	std::vector<FilteredEntities> filteredArray(_filters.size());
	int n = 0;
	size_t max = 0u;
	for (auto& f : _filters) {
		f->filter(entity);
		filteredArray[n++] = filtered;
		max = std::max(filtered.size(), max);
		// safe and clear
		filtered.clear();
	}

	FilteredEntities result(max);
	std::set_difference(
			filteredArray[0].begin(), filteredArray[0].end(),
			filteredArray[1].begin(), filteredArray[1].end(),
			std::back_inserter(result));

	if (filteredArray.size() >= 2) {
		FilteredEntities buffer(max);
		for (size_t i = 2; i < filteredArray.size(); ++i) {
			buffer.clear();
			std::set_difference(
					result.begin(), result.end(),
					filteredArray[i].begin(), filteredArray[i].end(),
					std::back_inserter(buffer));
			std::swap(result, buffer);
		}
	}

	filtered.reserve(alreadyFiltered.size() + max);
	for (auto& e : alreadyFiltered) {
		filtered.push_back(e);
	}
	for (auto& e : result) {
		filtered.push_back(e);
	}
}

}
