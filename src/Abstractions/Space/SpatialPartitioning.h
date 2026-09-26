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

		QueryResult();

		void clear() noexcept;
		size_t size() const noexcept;
		bool empty() const noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		void insert(const Entry& entry);
		void insert(Entity* entity, void* nodePtr = nullptr);
		void erase(Entity* entity);
		iterator find(Entity* entity);
		const_iterator find(Entity* entity) const;
		bool contains(Entity* entity) const;
		const std::vector<Entry>& entries() const noexcept;

	private:
		std::vector<Entry> _entries;
	};
}
