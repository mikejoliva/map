#ifndef __MAP_H
#define __MAP_H



namespace ml
{
	template <typename K, typename V>
	struct Pair
	{
		Pair() {/* Empty */ }
		Pair(K key, V value)
		{
			this->key = key;
			this->value = value;
		}
		K key;
		V value;
	};

	template <typename K, typename V>
	class Map
	{
	private:
		// Define the array that stores our structure
		Pair<K, V>* pArray = new Pair<K, V>;

		// Keep track of the size of the array
		unsigned int pSize = 0;

		// Keep an integer that represents the top of the array
		unsigned int pTop = 0;

		// Control the maximum size the array can be increased in one go
		// More efficient for larger maps but will increase memory consumption
		// Default - 512
		unsigned int pMaxIncrease = 512;


		V& insert(K key)
		{
			if (pSize > 0)
			{
				for (int idx = 0; idx < pTop; idx++)
				{
					if (pArray[idx].key == key)
					{
						return pArray[idx].value;
					}
				}
			}
			// Key not found
			push(key);
			return pArray[pTop - 1].value;
		}

		// Add a new key onto the array, if there is space.
		void push(K key)
		{
			// Check if our array is full
			if (pTop == pSize)
			{
				// Our array is full - let's increase it
				// To decrease number of times we create a new array, double the size each time.
				unsigned int newSize = pSize == 0 ? 1 : pSize * 2;
				// To save memory, only allocate a maximum of maxIncrease (default 512) extra space at a time
				if (newSize > pMaxIncrease)
				{
					newSize = pMaxIncrease;
				}
				pSize += newSize;

				Pair<K, V>* temp = new Pair<K, V>[pSize];

				// Check we have any values to copy
				if (pTop > 0)
				{
					// Copy old values into our new array
					for (int idx = 0; idx < pTop; idx++)
					{
						temp[idx] = pArray[idx];
					}
					delete[] pArray;
				}
				pArray = temp;
			}
			// We have space to add new values to our existing array.
			pArray[pTop].key = key;
			pArray[pTop].value = NULL;
			pTop++;
		}

	public:

		class Iterator
		{
		private:
			Map& pMap;
			int pIndex = 0;

			inline bool compare(const Iterator& comp)
			{
				return pMap.pArray[pIndex].key == comp.pMap.pArray[comp.pIndex].key &&
					pMap.pArray[pIndex].value == comp.pMap.pArray[comp.pIndex].value;
			}

			// Hide the invalid default constructor
			Iterator() { /* Empty */ }
		public:

			K first = pMap.pArray[pIndex].key;
			V second = pMap.pArray[pIndex].value;

			Iterator(Map& map) :pMap(map) { /* Empty */ };
			Iterator(Map& map, int idx) :pMap(map)
			{
				pIndex = idx;
				first = pMap.pArray[pIndex].key;
				second = pMap.pArray[pIndex].value;
			};

			~Iterator() { /*Empty*/ }

			// Operator overloads
			Iterator& operator++()
			{
				// Update our variables
				pIndex++;
				first = pMap.pArray[pIndex].key;
				second = pMap.pArray[pIndex].value;

				return *this;
			}

			Iterator& operator--()
			{
				// Update our variables
				pIndex--;
				first = pMap.pArray[pIndex].key;
				second = pMap.pArray[pIndex].value;

				return *this;
			}

			inline bool operator==(const Iterator& comp)
			{
				return compare(comp);
			}

			inline bool operator!=(const Iterator& comp)
			{
				return !compare(comp);
			}
		};

		// Constructors
		Map() { /* Empty */ };


		// Destuctor
		~Map()
		{
			// If we have an array to delete - delete it
			if (pSize > 0)
			{
				delete[] pArray;
			}
		};


		/////////////////////////////////////////////
		//  ACCESS ELEMENTS
		/////////////////////////////////////////////

		// Operator overload to assign a value with []
		V& operator[](K key)
		{
			return insert(key);
		}

		// Assign a value with the at function as opposed to []
		V& at(K key)
		{
			return insert(key);
		}

		/////////////////////////////////////////////
		//  Capacity
		/////////////////////////////////////////////

		// Return the size of the map
		unsigned int size()
		{
			return pSize;
		}

		// Return the size of the populated map
		unsigned int top()
		{
			return pTop;
		}

		// Check if the map is empty
		bool isEmpty()
		{
			return pTop == 0;
		}

		// Set the array size to the exact size of the map
		void wrap()
		{
			pSize = pTop;

			Pair<K, V>* temp = new Pair<K, V>[pSize];

			// Check we have any values to copy
			if (pTop > 0)
			{
				// Copy old values into our new array
				for (int idx = 0; idx < pTop; idx++)
				{
					temp[idx] = pArray[idx];
				}
				delete[] pArray;
			}
			pArray = temp;
		}


		// Change the maximum size an array can increase
		void setMaxIncrease(unsigned int newIncrease)
		{
			pMaxIncrease = newIncrease;
		}

		/////////////////////////////////////////////
		//  Iterators
		/////////////////////////////////////////////

		// Return the first value
		Iterator begin()
		{
			return Iterator(*this, 0);
		}

		// Return the last value
		Iterator end()
		{
			return Iterator(*this, pTop);
		}

		/////////////////////////////////////////////
		//  Modifiers
		/////////////////////////////////////////////

		// Remove all values and keys
		void clear()
		{
			if (pSize == 0) { return; }

			delete[] pArray;

			// Create a new array
			pArray = new Pair<K, V>();
			pTop = 0;
			pSize = 0;

		}

		// Removes a key & value pair from the array
		void remove(K key)
		{
			if (pSize == 0) { return; }

			if (pTop < pSize / 2 || pTop < pSize - pMaxIncrease)
			{
				// pSize is now over twice the size of the needed space OR pTop is less that pSize - pMaxIncrease
				// Let's save some memory and set pSize back to pTop
				pSize = pTop;
			}

			Pair<K, V>* temp = new Pair<K, V>[pSize];

			int newIdx = 0;
			for (int oldIdx = 0; oldIdx < pTop; oldIdx++, newIdx++)
			{
				if (pArray[oldIdx].key != key)
				{
					temp[newIdx] = pArray[oldIdx];
				} else {
					newIdx--;
				}
			}
			pTop = newIdx;

			delete[] pArray;
			pArray = temp;
		}

		// Swawp the value of two keys
		void swap(K key1, K key2)
		{
			int temp = at(key1);
			at(key1) = at(key2);
			at(key2) = temp;
		}

		/////////////////////////////////////////////
		//  Operations
		/////////////////////////////////////////////

		// Take the pair struct as an arguments and insert into our array
		void insert(Pair<K, V> newPair)
		{
			insert(newPair.key) = newPair.value;
		}

		// Insert a new map pair by taking the key and value in their raw form
		void emplace(K key, V value)
		{
			insert(key) = value;
		}

		// Insert a new map, if it does not already exist, and return an iterator to it
		// Otherwise if it does exists, return an iterator to the existing item


		// Return an iterator to a given key, or the last key if it cannot be found
		Iterator find(K key)
		{
			for (int idx = 0; idx < pTop; idx++)
			{
				if (pArray[idx].key == key)
				{
					return Iterator(*this, idx);
				}
			}
			return end();
		}

		// Return if a key exists in the map
		bool exists(K key)
		{
			for (int idx = 0; idx < pTop; idx++)
			{
				if (pArray[idx].key == key)
				{
					return true;
				}
			}
			return false;
		}
	
	};
}
#endif