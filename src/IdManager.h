#pragma once
#include <vector>

class IdManager
{
public:
	int getIndex(int id) const {
		if (id < 0 || id >= (int)indices.size()) return -1;
		return indices[id];
	}

	int getNextId() {
		if (!free_ids.empty()) {
			int id = free_ids.back();
			free_ids.pop_back();
			return id;
		}

		indices.push_back(-1);
		return indices.size() - 1;
	}

	void setIndex(int id, int index) {
		if (id >= (int)indices.size()) {
			return;
		}

		indices[id] = index;
	}

	void releaseId(int id) {
		if (id >= 0 && id < (int)indices.size()) {
			indices[id] = -1;
			free_ids.push_back(id);
		}
	}

private:
	std::vector<int> indices;
	std::vector<int> free_ids;
};
