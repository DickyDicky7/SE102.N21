#include "SpatialPartitioning.h"

namespace Space
{
	QueryResult::QueryResult()
	{
		this->_entries.reserve(Constants::Physics::SPATIAL_QUERY_RESERVE_CAPACITY);
	}

	void QueryResult::clear() noexcept
	{
		this->_entries.clear();
	}

	size_t QueryResult::size() const noexcept
	{
		return this->_entries.size();
	}

	bool QueryResult::empty() const noexcept
	{
		return this->_entries.empty();
	}

	QueryResult::iterator QueryResult::begin() noexcept
	{
		return this->_entries.begin();
	}

	QueryResult::iterator QueryResult::end() noexcept
	{
		return this->_entries.end();
	}

	QueryResult::const_iterator QueryResult::begin() const noexcept
	{
		return this->_entries.begin();
	}

	QueryResult::const_iterator QueryResult::end() const noexcept
	{
		return this->_entries.end();
	}

	QueryResult::const_iterator QueryResult::cbegin() const noexcept
	{
		return this->_entries.cbegin();
	}

	QueryResult::const_iterator QueryResult::cend() const noexcept
	{
		return this->_entries.cend();
	}

	void QueryResult::insert(const Entry& entry)
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

	void QueryResult::insert(Entity* entity, void* nodePtr)
	{
		this->insert(Entry{ entity, nodePtr });
	}

	void QueryResult::erase(Entity* entity)
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

	QueryResult::iterator QueryResult::find(Entity* entity)
	{
		for (auto it = this->_entries.begin(); it != this->_entries.end(); ++it)
		{
			if (it->first == entity) return it;
		}
		return this->_entries.end();
	}

	QueryResult::const_iterator QueryResult::find(Entity* entity) const
	{
		for (auto it = this->_entries.cbegin(); it != this->_entries.cend(); ++it)
		{
			if (it->first == entity) return it;
		}
		return this->_entries.cend();
	}

	bool QueryResult::contains(Entity* entity) const
	{
		return this->find(entity) != this->_entries.cend();
	}

	const std::vector<QueryResult::Entry>& QueryResult::entries() const noexcept
	{
		return this->_entries;
	}
}
