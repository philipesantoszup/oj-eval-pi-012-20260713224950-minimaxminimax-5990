/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */
    
template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap {
public:
	/**
	 * give iterator and const_iterator access to private members
	 */
	friend class iterator;
	friend class const_iterator;

	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::linked_hashmap as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;

	/**
	 * A node in the doubly linked list
	 */
	struct Node {
		value_type data;
		Node* prev;
		Node* next;
		Node(const value_type& d, Node* p = nullptr, Node* n = nullptr) 
			: data(d), prev(p), next(n) {}
	};

	/**
	 * A bucket entry for the hash table
	 */
	struct BucketNode {
		Node* node;
		BucketNode* next;
		BucketNode(Node* n = nullptr, BucketNode* ne = nullptr) 
			: node(n), next(ne) {}
	};

	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = linked_hashmap.begin(); --it;
	 *       or it = linked_hashmap.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	public:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		Node* node;
		linked_hashmap* map;
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. 
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://blog.csdn.net/u013854486/article/details/79404865
		using difference_type = std::ptrdiff_t;
		using value_type = typename linked_hashmap::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::bidirectional_iterator_tag;


		iterator() : node(nullptr), map(nullptr) {
			// TODO
		}
		iterator(Node* n, linked_hashmap* m) : node(n), map(m) {
			// TODO
		}
		iterator(const iterator &other) : node(other.node), map(other.map) {
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator temp = *this;
			if (node != nullptr) {
				node = node->next;
			}
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (node != nullptr) {
				node = node->next;
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator temp = *this;
			if (node == nullptr && map != nullptr) {
				// This is end(), move to tail
				node = map->tail;
			} else if (node != nullptr) {
				node = node->prev;
			}
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (node == nullptr && map != nullptr) {
				// This is end(), move to tail
				node = map->tail;
			} else if (node != nullptr) {
				node = node->prev;
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return node->data;
		}
		bool operator==(const iterator &rhs) const {
			return node == rhs.node;
		}
		bool operator==(const const_iterator &rhs) const {
			return node == rhs.node;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return node != rhs.node;
		}
		bool operator!=(const const_iterator &rhs) const {
			return node != rhs.node;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return &(node->data);
		}
	};
 
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		public:
			// data members.
			Node* node;
			const linked_hashmap* map;
		public:
			const_iterator() : node(nullptr), map(nullptr) {
				// TODO
			}
			const_iterator(const const_iterator &other) : node(other.node), map(other.map) {
				// TODO
			}
			const_iterator(const iterator &other) : node(other.node), map(other.map) {
				// TODO
			}

			const_iterator(Node* n, const linked_hashmap* m) : node(n), map(m) {}

			// And other methods in iterator.
			const_iterator operator++(int) {
				const_iterator temp = *this;
				if (node != nullptr) {
					node = node->next;
				}
				return temp;
			}
			const_iterator & operator++() {
				if (node != nullptr) {
					node = node->next;
				}
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator temp = *this;
				if (node == nullptr && map != nullptr) {
					// This is end(), move to tail
					node = const_cast<linked_hashmap*>(map)->tail;
				} else if (node != nullptr) {
					node = node->prev;
				}
				return temp;
			}
			const_iterator & operator--() {
				if (node == nullptr && map != nullptr) {
					// This is end(), move to tail
					node = const_cast<linked_hashmap*>(map)->tail;
				} else if (node != nullptr) {
					node = node->prev;
				}
				return *this;
			}
			value_type & operator*() const {
				return node->data;
			}
			bool operator==(const iterator &rhs) const {
				return node == rhs.node;
			}
			bool operator==(const const_iterator &rhs) const {
				return node == rhs.node;
			}
			bool operator!=(const iterator &rhs) const {
				return node != rhs.node;
			}
			bool operator!=(const const_iterator &rhs) const {
				return node != rhs.node;
			}
			value_type* operator->() const noexcept {
				return &(node->data);
			}
	};
 
	/**
	 * TODO two constructors
	 */
	linked_hashmap() : bucketArray(nullptr), bucketSize(0), listSize(0), 
					   head(nullptr), tail(nullptr), loadFactor(0.75) {
		bucketSize = 16;
		bucketArray = new BucketNode*[bucketSize];
		for (size_t i = 0; i < bucketSize; ++i) {
			bucketArray[i] = nullptr;
		}
	}
	linked_hashmap(const linked_hashmap &other) : bucketArray(nullptr), bucketSize(0), listSize(0),
													head(nullptr), tail(nullptr), loadFactor(0.75) {
		bucketSize = other.bucketSize;
		bucketArray = new BucketNode*[bucketSize];
		for (size_t i = 0; i < bucketSize; ++i) {
			bucketArray[i] = nullptr;
		}
		
		// Copy all elements from other in insertion order
		Node* curr = other.head;
		while (curr != nullptr) {
			insert(curr->data);
			curr = curr->next;
		}
	}
 
	/**
	 * TODO assignment operator
	 */
	linked_hashmap & operator=(const linked_hashmap &other) {
		if (this != &other) {
			clear();
			
			delete[] bucketArray;
			bucketSize = other.bucketSize;
			bucketArray = new BucketNode*[bucketSize];
			for (size_t i = 0; i < bucketSize; ++i) {
				bucketArray[i] = nullptr;
			}
			
			// Copy all elements from other in insertion order
			Node* curr = other.head;
			while (curr != nullptr) {
				insert(curr->data);
				curr = curr->next;
			}
		}
		return *this;
	}
 
	/**
	 * TODO Destructors
	 */
	~linked_hashmap() {
		clear();
		delete[] bucketArray;
	}
 
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		Node* node = findNode(key);
		if (node == nullptr) {
			throw index_out_of_bound();
		}
		return node->data.second;
	}
	const T & at(const Key &key) const {
		Node* node = findNode(key);
		if (node == nullptr) {
			throw index_out_of_bound();
		}
		return node->data.second;
	}
 
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		// Try to find existing key first
		Node* node = findNode(key);
		if (node != nullptr) {
			return node->data.second;
		}
		// Key doesn't exist, insert default-constructed value
		// This calls insert which handles resizing and creates new node properly
		auto result = insert(value_type(key, T()));
		return result.first->second;
	}
 
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		Node* node = findNode(key);
		if (node == nullptr) {
			throw index_out_of_bound();
		}
		return node->data.second;
	}
 
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		return iterator(head, this);
	}
	const_iterator cbegin() const {
		return const_iterator(head, this);
	}
 
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		return iterator(tail ? tail->next : nullptr, this);
	}
	const_iterator cend() const {
		return const_iterator(tail ? tail->next : nullptr, this);
	}
 
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return listSize == 0;
	}
 
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return listSize;
	}
 
	/**
	 * clears the contents
	 */
	void clear() {
		// Delete all nodes in the linked list
		Node* curr = head;
		while (curr != nullptr) {
			Node* next = curr->next;
			delete curr;
			curr = next;
		}
		head = nullptr;
		tail = nullptr;
		listSize = 0;
		
		// Clear bucket array
		for (size_t i = 0; i < bucketSize; ++i) {
			BucketNode* curr = bucketArray[i];
			while (curr != nullptr) {
				BucketNode* next = curr->next;
				delete curr;
				curr = next;
			}
			bucketArray[i] = nullptr;
		}
	}
 
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		Key key = value.first;
		
		// Check if key already exists
		Node* existingNode = findNode(key);
		if (existingNode != nullptr) {
			// Key already exists, return iterator to existing element and false
			return pair<iterator, bool>(iterator(existingNode, this), false);
		}
		
		// Check if we need to resize
		if (listSize >= bucketSize * loadFactor) {
			resize(bucketSize * 2);
		}
		
		// Create new node and add to linked list (at the end)
		Node* newNode = new Node(value, tail, nullptr);
		if (tail != nullptr) {
			tail->next = newNode;
		} else {
			head = newNode;
		}
		tail = newNode;
		
		// Add to hash table
		size_t bucketIndex = hash(key) % bucketSize;
		BucketNode* bucketNode = new BucketNode(newNode, bucketArray[bucketIndex]);
		bucketArray[bucketIndex] = bucketNode;
		
		listSize++;
		
		return pair<iterator, bool>(iterator(newNode, this), true);
	}
 
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if (pos == end()) {
			throw invalid_iterator();
		}
		if (pos.node == nullptr || pos.map != this) {
			throw invalid_iterator();
		}
		
		Node* node = pos.node;
		
		// Remove from linked list
		if (node->prev != nullptr) {
			node->prev->next = node->next;
		} else {
			head = node->next;
		}
		if (node->next != nullptr) {
			node->next->prev = node->prev;
		} else {
			tail = node->prev;
		}
		
		// Remove from hash table
		size_t bucketIndex = hash(node->data.first) % bucketSize;
		BucketNode* curr = bucketArray[bucketIndex];
		BucketNode* prev = nullptr;
		while (curr != nullptr) {
			if (curr->node == node) {
				if (prev != nullptr) {
					prev->next = curr->next;
				} else {
					bucketArray[bucketIndex] = curr->next;
				}
				delete curr;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
		
		delete node;
		listSize--;
	}
 
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 */
	size_t count(const Key &key) const {
		return findNode(key) != nullptr ? 1 : 0;
	}
 
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		Node* node = findNode(key);
		return iterator(node, this);
	}
	const_iterator find(const Key &key) const {
		Node* node = findNode(key);
		return const_iterator(node, this);
	}

private:
	BucketNode** bucketArray;
	size_t bucketSize;
	size_t listSize;
	Node* head;
	Node* tail;
	float loadFactor;
	Hash hash;
	Equal equal;

	// Helper function to find a node by key
	Node* findNode(const Key& key) const {
		size_t bucketIndex = hash(key) % bucketSize;
		BucketNode* curr = bucketArray[bucketIndex];
		while (curr != nullptr) {
			if (equal(curr->node->data.first, key)) {
				return curr->node;
			}
			curr = curr->next;
		}
		return nullptr;
	}

	// Resize the hash table
	void resize(size_t newSize) {
		BucketNode** newArray = new BucketNode*[newSize];
		for (size_t i = 0; i < newSize; ++i) {
			newArray[i] = nullptr;
		}
		
		// Rehash all elements
		Node* curr = head;
		while (curr != nullptr) {
			size_t newIndex = hash(curr->data.first) % newSize;
			BucketNode* bucketNode = new BucketNode(curr, newArray[newIndex]);
			newArray[newIndex] = bucketNode;
			curr = curr->next;
		}
		
		// Delete old bucket array
		for (size_t i = 0; i < bucketSize; ++i) {
			BucketNode* curr = bucketArray[i];
			while (curr != nullptr) {
				BucketNode* next = curr->next;
				delete curr;
				curr = next;
			}
		}
		delete[] bucketArray;
		
		bucketArray = newArray;
		bucketSize = newSize;
	}
};

}

#endif