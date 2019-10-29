#ifndef __MAP_H
#define __MAP_H

class Map
{
private:
	// Define the strucutre to be helnd in our array
	struct pNode
	{
		std::string key;
		int value;
	};

	pNode* pArray = new pNode;
	unsigned int pSize = 0;

	void increment(std::string key)
	{
		pNode* temp = new pNode[pSize + 1];
		if (pSize > 0)
		{
			for (int idx = 0; idx < pSize; idx++)
			{
				temp[idx] = pArray[idx];
			}
			delete[] pArray;
		}
		pArray = temp;

		pArray[pSize].key	 = key;
		pArray[pSize].value = NULL;
		pSize++;
	}

public:
	Map() {};
	~Map() 
	{
		if (pSize > 0)
		{
			delete[] pArray;
		}
	};

	// Operator overloads
	int& operator[](const std::string& key)
	{
		if (pSize > 0)
		{
			for (int idx = 0; idx < pSize; idx++)
			{
				if (pArray[idx].key == key)
				{
					return pArray[idx].value;
				}
			}
		}
		// Key not found
		increment(key);
		return pArray[pSize - 1].value;
	}

	// Return the size of the map
	unsigned int size()
	{
		return pSize;
	}

	// Return the first value
	int start()
	{
		if (pSize > 0)
		{
			return pArray[0].value;
		}
		return -1;
	}

	// Return the last value
	int end()
	{
		if (pSize > 0)
		{
			return pArray[pSize - 1].value;
		}
		return -1;
	}

	// Check if the map is empty
	bool isEmpty()
	{
		return pSize == 0;
	}

	// Sets the value of all keys to null
	void clear()
	{
		for (int idx = 0; idx < pSize; idx++)
		{
			pArray[idx].value == NULL;
		}
	}

	// Sets the value of one key to null
	void clear(std::string key)
	{
		for (int idx = 0; idx < pSize; idx++)
		{
			if (pArray[idx].key == key)
			{
				pArray[idx].value = NULL;
			}
		}
	}

	// Remove all values and keys
	void destroy()
	{
		if (pSize == 0) { return; }

		delete[] pArray;
		pSize = 0;

	}

	// Removes a key & value pair from the array
	void remove(std::string key)
	{
		if (pSize == 0) { return; }

		pNode* temp = new pNode[pSize - 1];

		for (int idx = 0; idx < pSize; idx++)
		{
			if (pArray[idx].key != key)
			{
				temp[idx] = pArray[idx];
			}
			else if (idx != pSize - 1) {
				idx++;
				pSize--;
				temp[idx - 1] = pArray[idx];
			}
		}
		delete[] pArray;
		pArray = temp;

	}

	// Iterated each element and execute passed function
	template<typename F>
	void forEach(F func(std::string key, int value, int idx))
	{
		for (int idx = 0; idx < pSize; idx++)
		{
			func(pArray[idx].key, pArray[idx].value, idx);
		}
	}
};

#endif