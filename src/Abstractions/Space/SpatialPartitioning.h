#pragma once

#include "Constants.h"
#include "AABB.h"
#include "BLAS.h"
#include "BVH.h"
#include "TLAS.h"

namespace Space
{
	// High-Performance Query Result container supporting structured binding and zero per-frame allocation
	class QueryResult
	{
	public:
		using Entry = std::pair<Entity*, void*>;
		using iterator = std::vector<Entry>::iterator;
		using const_iterator = std::vector<Entry>::const_iterator;

		QueryResult()
		{
			this->_entries.reserve(Constants::Physics::SPATIAL_QUERY_RESERVE_CAPACITY);
		}

		void clear() noexcept { this->_entries.clear(); }
		size_t size() const noexcept { return this->_entries.size(); }
		bool empty() const noexcept { return this->_entries.empty(); }

		iterator begin() noexcept { return this->_entries.begin(); }
		iterator end() noexcept { return this->_entries.end(); }
		const_iterator begin() const noexcept { return this->_entries.begin(); }
		const_iterator end() const noexcept { return this->_entries.end(); }
		const_iterator cbegin() const noexcept { return this->_entries.cbegin(); }
		const_iterator cend() const noexcept { return this->_entries.cend(); }

		void insert(const Entry& entry)
		{
			if (!entry.first) return;
			for (auto& item : this->_entries)
			{
				if (item.first == entry.first)
				{
					item.second = entry.second;
					return;
				}
			}
			this->_entries.push_back(entry);
		}

		void insert(Entity* entity, void* nodePtr = nullptr)
		{
			this->insert(Entry{ entity, nodePtr });
		}

		void erase(Entity* entity)
		{
			for (auto it = this->_entries.begin(); it != this->_entries.end(); ++it)
			{
				if (it->first == entity)
				{
					this->_entries.erase(it);
					return;
				}
			}
		}

		iterator find(Entity* entity)
		{
			for (auto it = this->_entries.begin(); it != this->_entries.end(); ++it)
			{
				if (it->first == entity) return it;
			}
			return this->_entries.end();
		}

		const_iterator find(Entity* entity) const
		{
			for (auto it = this->_entries.cbegin(); it != this->_entries.cend(); ++it)
			{
				if (it->first == entity) return it;
			}
			return this->_entries.cend();
		}

		bool contains(Entity* entity) const
		{
			return this->find(entity) != this->_entries.cend();
		}

		const std::vector<Entry>& entries() const noexcept { return this->_entries; }

	private:
		std::vector<Entry> _entries;
	};
}
