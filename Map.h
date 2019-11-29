#ifndef __MAP_H
#define __MAP_H

namespace ml
{
	/** Structure to store the key & value pairs */
	template <typename K, typename V> struct Pair
	{
		K key;
		V value;
		const K* address;

		Pair() { /** Empty */ }
		Pair(const K& key, const V& value)
		{
			this->address	= &key;
			this->key		= key;
			this->value		= value;
		}
	};

	template <typename K, typename V> class Map
	{
	private:
		/** Define the array that stores our structure */
		Pair<K, V>* pArray = new Pair<K, V>();

		/** Keep track of the size of the array */
		unsigned int pSize = 0;

		/** Keep an integer that represents the top of the array */
		unsigned int pTop = 0;

		/**
		* Control the maximum size the array can be increased in one go
		* More efficient for larger maps but will increase memory consumption
		* Default - 512
		*/
		unsigned int pMaxIncrease = 512;


		/**
		* @param key The key for which data should be retireved or addes
		* @return A reference to the value of a given key
		*
		* Return a reference of the value at a key
		* If the key is new, create a new element under that key and return NULL
		*/
		V& insert(const K& key)
		{
			if (pSize > 0)
			{
				for (unsigned int idx = 0; idx < pTop; idx++)
				{
					if (pArray[idx].address == &key || pArray[idx].key == key)
					{
						return pArray[idx].value;
					}
				}
			}
			/** Key not found */
			push(key);
			return pArray[pTop - 1].value;
		}

		/**
		* @param key The key for which data should be retireved or addes
		* @return A reference to the value of a given key
		*
		* Control the maximum size the array can be increased in one go
		* More efficient for larger maps but will increase memory consumption
		* Default - 512
		*/
		void push(const K& key)
		{
			/** Check if our array is full */
			if (pTop == pSize)
			{
				/** Our array is full - let's increase it */
				/** To decrease number of times we create a new array, double the size each time. */
				unsigned int newSize = pSize == 0 ? 1 : pSize * 2;
				/** To save memory, only allocate a maximum of maxIncrease (default 512) extra space at a time */
				if (newSize > pMaxIncrease)
				{
					newSize = pMaxIncrease;
				}
				pSize += newSize;

				Pair<K, V>* temp;
				/** Try to create our new array without running out of memeory */
				try {
					temp = new Pair<K, V>[pSize];
				}
				catch (std::bad_alloc& ex) {
					throw ex;
				}
					

				/** Check we have any values to copy */
				if (pTop > 0)
				{
					/** Copy old values into our new array */
					for (unsigned int idx = 0; idx < pTop; idx++)
					{
						temp[idx] = pArray[idx];
					}
					delete[] pArray;
				}
				pArray = temp;
			}

			/** We have space to add new values to our existing array. */
			pArray[pTop].key		= key;
			pArray[pTop].address	= &key;
			pArray[pTop].value		= NULL;

			/** Increment our counter to the next free space */
			pTop++;
		}

	public:

		class Iterator
		{
		private:
			Map& pMap;
			unsigned int pIndex = 0;

			inline bool compare(const Iterator& comp)
			{
				return	pMap.pArray[pIndex].address == comp.pMap.pArray[comp.pIndex].address;
			}

			void update()
			{
				if (pIndex < pMap.pTop)
				{
					this->first		= pMap.pArray[pIndex].key;
					this->second	= pMap.pArray[pIndex].value;
					this->index		= pIndex;
				}
			}

			/** Hide the invalid default constructor */
			Iterator() { /** Empty */ }
		public:
			/** Variables exposed to the user, similar style to the std::iterator */
			K first		= pMap.pArray[pIndex].key;
			V second	= pMap.pArray[pIndex].value;

			/** Don't expose our actual pIndex incase the user attemps to change it! */
			int index	= pIndex;	

			Iterator(Map& map)			: pMap(map) { /* Empty */ };
			Iterator(Map& map, int idx) : pMap(map)
			{
				pIndex	= idx;
				update();
			};

			~Iterator() { /** Empty*/ }

			/** Operator overloads */

			/** Overload ++ prefix */
			Iterator& operator++ ()
			{
				pIndex++;
				update();

				return *this;
			}

			/** Overload ++ postfix */
			Iterator& operator++ (int)
			{
				// Make a copy to return
				Iterator retVal(*this);
				++(*this);

				return retVal;
			}

			/** Overload -- prefix */
			Iterator& operator-- ()
			{
				pIndex--;
				update();

				return *this;
			}

			/** Overload -- postfix */
			Iterator& operator-- (int)
			{
				// Make a copy to return
				Iterator retVal(*this);
				--(*this);

				return retVal;
			}

			/** Overload comparison operators */

			inline bool operator== (const Iterator& comp)
			{
				return compare(comp);
			}

			inline bool operator!= (const Iterator& comp)
			{
				return !compare(comp);
			}
		};

		/** Constructors */
		Map()	{ /** Empty */ };
		Map(unsigned int maxIncrease) { this->pMaxIncrease = maxIncrease; };


		/** Destuctor */
		~Map()
		{
			/** If we have an array to delete - delete it */
			if (pSize > 0)
			{
				delete[] pArray;
			}
		};


		/** Operator overload to assign a value with [] */
		V& operator[] (const K& key)
		{
			return insert(key);
		}

		/** Assign a value with the at function as opposed to [] */
		V& at(const K& key)
		{
			return insert(key);
		}


		/** Return the size of the map */
		unsigned int size()
		{
			return pSize;
		}

		/** Return the size of the populated map */
		unsigned int top()
		{
			return pTop;
		}

		/** Check if the map is empty */
		bool isEmpty()
		{
			return pTop == 0;
		}

		/** Set the array size to the exact size of the map */
		void wrap()
		{
			// Return if empty
			if (pTop == 0) { return; }

			pSize = pTop;

			Pair<K, V>* temp = new Pair<K, V>[pSize]();

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

		/**
		* @param newIncrease new maximum array increase size
		*
		* Change the maximum size an array can increase
		*/
		void setMaxIncrease(unsigned int newIncrease)
		{
			pMaxIncrease = newIncrease;
		}

		
		/** Return the first value */
		Iterator begin()
		{
			return Iterator(*this, 0);
		}

		/** Return the last value */
		Iterator end()
		{
			return Iterator(*this, pTop);
		}


		/** Remove all values and keys */
		void clear()
		{
			if (pSize == 0) { return; }

			delete[] pArray;

			/** Create a new array */
			pArray = new Pair<K, V>();
			pTop = 0;
			pSize = 0;
		}

		/**
		* @param key to be removed
		*
		* Remove a key & value pair from the map
		*/
		void remove(const K& key)
		{
			if (pSize == 0) { return; }

			if (pTop < pSize / 2 || pTop < pSize - pMaxIncrease)
			{
				/**	
				*	pSize is now over twice the size of the needed space OR pTop is less that pSize - pMaxIncrease
				*	Let's save some memory and set pSize back to pTop 
				*/
				pSize = pTop;
			}

			Pair<K, V>* temp = new Pair<K, V>[pSize];

			unsigned int newIdx = 0;
			for (unsigned int oldIdx = 0; oldIdx < pTop; oldIdx++, newIdx++)
			{
				/** 
				* Lets copy across our old array into the new one -
				* unless the current key is the one we want to remove
				*/
				if (pArray[oldIdx].address != &key && pArray[oldIdx].key != key)
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

		/**
		* @param Pair struct to be removed from the map
		* 
		* Remove a key and value pair from the map 
		*/
		void remove(Pair<K, V> pair)
		{
			remove(pair.key);
		}

		/**
		* @param key1 first value to find
		* @param key2 second value to find
		*
		* Swap the values of key1 and key2
		*/
		void swap(const K& key1, const K& key2)
		{
			V temp = insert(key1);
			insert(key1) = insert(key2);
			insert(key2) = temp;
		}

		/**
		* @param pair to be inserted
		*
		* Take the pair struct as an arguments and insert into our array
		*/
		void insert(Pair<K, V>& pair)
		{
			insert(pair.key) = pair.value;
		}

		/**
		* @param key to be inserted
		* @param value to be inserted
		* 
		* Insert a new map pair by taking the key and value in their raw form
		*/
		void emplace(const K& key, V value)
		{
			insert(key) = value;
		}

		/**
		* @param key to find
		*
		* Return an iterator to a given key, or the last key if it cannot be found
		*/
		Iterator find(const K& key)
		{
			for (unsigned int idx = 0; idx < pTop; idx++)
			{
				if (pArray[idx].address == &key || pArray[idx].key == key)
				{
					return Iterator(*this, idx);
				}
			}
			return end();
		}

		/**
		* @param key to find
		*
		* Return if a key exists in the map
		*/
		bool exists(const K& key)
		{
			for (int idx = 0; idx < pTop; idx++)
			{
				if (pArray[idx].address == &key || pArray[idx].key == key)
				{
					return true;
				}
			}
			return false;
		}
	
	};
}
#endif