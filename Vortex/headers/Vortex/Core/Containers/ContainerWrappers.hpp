#pragma once
#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

namespace Vortex {

	template <typename T, std::size_t N>
	using Array = std::array<T, N>;

	template <typename T, class Allocator = std::allocator<T>>
	using Vector = std::vector<T, Allocator>;

	template <typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
	using Set = std::set<T, Compare, Allocator>;

	template <typename Key, typename Val, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Val>>>
	using Map = std::map<Key, Val, Compare, Allocator>;

	template <typename T, class Hash = std::hash<T>, class KeyEqual = std::equal_to<T>, class Allocator = std::allocator<T>>
	using HashSet = std::unordered_set<T, Hash, KeyEqual, Allocator>;

	template <typename Key, typename Val, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Allocator = std::allocator<std::pair<const Key, Val>>>
	using HashMap = std::unordered_map<Key, Val, Hash, KeyEqual, Allocator>;
}