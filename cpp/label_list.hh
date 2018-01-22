#ifndef _LABEL_LIST_HH_
#define _LABEL_LIST_HH_

#include <map>
#include <utility>

#include <algorithm>

#include <cstddef>

template <typename T>
class label_list {
	public:
		typedef typename std::map<T,T*>::size_type size_type;

		typedef typename std::map<T,T*>::iterator iterator;
		typedef typename std::map<T,T*>::const_iterator const_iterator;

		label_list() {
			
		}

		label_list(const label_list &other) {
			set_labels(other.labels);
		}

		label_list & operator=(const label_list &other) {
			set_labels(other.labels);

			return *this;
		}

		~label_list() {
			clear();
		}

		/*
		 * Iterators
		 */

		iterator begin() {
			return labels.begin();
		}

		iterator end() {
			return labels.end();
		}

		const_iterator begin() const {
			return labels.begin();
		}

		const_iterator end() const {
			return labels.end();
		}

		/*
		 * Capacity
		 */

		bool empty() const {
			return labels.empty();
		}

		size_type size() const {
			return labels.size();
		}

		/*
		 * Element Access
		 */
		
		T * operator[](const T &item) {
			std::pair<iterator,bool> ret = labels.insert(value_type(item, NULL));
			if(ret.second) {
				ret.first->second = new T(item);
			}

				
			return ret.first->second;;
		}

		/*
		 * Modifiers
		 */

		void clear() {
			iterator iter = begin();
			for(; iter != end(); ++iter) {
				delete(iter->second);
			}

			labels.clear();
		}

		/*
		 * Operations
		 */


	
	protected:
		typedef typename std::map<T,T*>::value_type value_type;

		std::map<T,T*> labels;

		void set_labels(const std::map<T,T*> &other_labels) {
			clear();

			T *new_item = NULL;
			try {
				const_iterator iter = other_labels.begin();
				for(; iter != other_labels.end(); ++iter) {
					new_item = NULL;
					new_item = new T(&(iter->second));
					labels[iter->first] = new_item;
				}
			}
			catch(...) {
				delete new_item;
				throw;
			}
		}
	
};

#endif

