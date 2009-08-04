/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: hash.h,v 1.9 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_HASH_H__
#define __NTK_SUPPORT_HASH_H__


#include <assert.h>
#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif


namespace ntk {


NtkExport int hash_table_new_size(uint size);


struct IntHashFunc {
	uint operator()(int val) const
	{
		return val;
	}
};

struct StringHashFunc {
	uint operator()(const String& str) const
	{
		uint hash_val = 0;
		const char_t* pstr = str.c_str();

		while(*pstr != '\0')
			hash_val = hash_val*31 + *pstr++;

		return hash_val + (hash_val >> 5);
	}
};

template<class VALUE, class KEY = String, class HASH_FUNC = StringHashFunc>
class HashTable {
public:
	//
	// types
	//
	typedef VALUE     Value;
	typedef KEY       Key;
	typedef HASH_FUNC HashFunc;
	typedef HashTable<VALUE, KEY, HASH_FUNC> This;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef HashFunc hash_func_t;
		typedef This     this_t;
	#endif

	typedef size_t       size_type;
	typedef VALUE        value_type;
	typedef VALUE*       pointer;
	typedef const VALUE* const_pointer;
	typedef VALUE&       reference;
	typedef const VALUE& const_reference;
	typedef Key          key_type;

	struct Entry {
		uint hash_value;
		Key key;
		Value data;
		Entry* next;

		Entry(uint hash_val, const Key& key_, const Value& data_, Entry* next_)
		:	hash_value(hash_val), key(key_), data(data_), next(next_)
		{}
	};

	class EntryList {
	public:
		//
		//
		//
		typedef Entry*                               value_type;
		typedef std::vector<value_type>              value_type_list_type;
		typedef value_type_list_type::iterator       iterator;
		typedef value_type_list_type::const_iterator const_iterator;
		typedef value_type_list_type::size_type      size_type;

		//
		// methods
		//
		EntryList(int size)
		:	m_initial_size(size)
		,	m_value_type_list(size, NULL)
		{}
		~EntryList()
		{
			delete_all_entries();
		}

		void delete_all_entries()
		{
			Entry *entry, *next_entry;
			for(iterator it = m_value_type_list.begin(); it != m_value_type_list.end(); ++it)
			{
				entry = *it;
				while(entry)
				{
					next_entry = entry->next;
					delete entry;
					entry = next_entry;
				}
			}

			m_value_type_list.clear();
			m_value_type_list.resize(m_initial_size, NULL);
		}

		//
		// accessors
		//
		iterator begin()
		{
			return m_value_type_list.begin();
		}
		const_iterator begin() const
		{
			return m_value_type_list.begin();
		}
		iterator end()
		{
			return m_value_type_list.end();
		}
		const_iterator end() const
		{
			return m_value_type_list.end();
		}

		size_type size() const
		{
			return m_value_type_list.size();
		}

		value_type& at(size_type index)
		{
			return m_value_type_list[index];
		}
		const value_type& at(size_type index) const
		{
			return m_value_type_list[index];
		}

	private:
		//
		// data
		//
		size_type m_initial_size;
		value_type_list_type m_value_type_list;

	};// class EntryList

	class iterator {//: public std::iterator<>
	public:
		//
		// methods
		//
		iterator()
		:	m_entry(NULL),
			m_entry_list(NULL),
			m_list_it(NULL)
		{}
		iterator(const iterator& it)
		:	m_entry(it.m_entry),
			m_entry_list(it.m_entry_list),
			m_list_it(it.m_list_it)
		{}
		~iterator()
		{}

		//
		// accessors
		//
		const key_type& key() const
		{
			return m_entry->key;
		}
		reference get()
		{
			return m_entry->data;
		}
		const_reference get() const
		{
			return m_entry->data;
		}

		//
		// operators
		//
		reference operator*()
		{
			return get();
		}
		const_reference operator*() const
		{
			return get();
		}
		reference operator->()
		{
			return get();
		}
		const_reference operator->() const
		{
			return get();
		}

		iterator& operator++()
		{
			// NHashTable::at() で返される iterator はリスト上の位置情報を持たないのでこの処理は出来ない
			assert(m_entry_list);

			_advance();
			return *this;
		}
		iterator operator++(int)
		{
			// NHashTable::at() で返される iterator はリスト上の位置情報を持たないのでこの処理は出来ない
			assert(m_entry_list);

			iterator tmp(*this);
			_advance();
			return tmp;
		}

		operator bool() const
		{
			return entry != NULL;
		}

		//
		// friends
		//
		friend bool operator==(const iterator& lhs, const iterator& rhs)
		{
			return lhs.m_entry == rhs.m_entry;
		}
		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			return !(lhs == rhs);
		}

	private:
		//
		// data
		//
		Entry* m_entry;
		EntryList* m_entry_list;
		EntryList::iterator m_list_it;

		//
		// methods
		//

		// HashTable からのみ呼ばれる
		iterator(Entry* entry, EntryList* entry_list, EntryList::iterator list_it)
		:	m_entry(entry),
			m_entry_list(entry_list),
			m_list_it(list_it)
		{
			assert(entry_list);

			if(m_entry == NULL)
				_advance();
		}

		void _advance()
		{
			if(m_entry)
				m_entry = m_entry->next;

			while(m_entry == NULL && m_list_it != m_entry_list->end())
			{
				++m_list_it;
				if(m_list_it != m_entry_list->end())
					m_entry = *m_list_it;
			}
		}

		//
		// friends
		//
		friend class HashTable<VALUE, KEY, HASH_FUNC>;

	};// class iterator

	typedef const iterator const_iterator;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Entry     entry_t;
		typedef EntryList entry_list_t;
	#endif

	//
	// constants
	//
	enum {
		MINIMUM_SIZE = 8,
		DEFAULT_MAX_DENSITY = 5,
	};

	//
	// methods
	//
	HashTable()
	:	m_entry_list(NULL)
	,	m_num_entries(0)
	,	m_rehash(true)
	{
		clear(1);
	}
	HashTable(size_type size_)
	:	m_entry_list(NULL)
	,	m_num_entries(0)
	,	m_rehash(true)
	{
		clear(size_);
	}
	HashTable(const This& rhs)
	:	m_entry_list(NULL)
	,	m_num_entries(0)
	,	m_rehash(true)
	{
		operator=(rhs);
	}
	~HashTable()
	{
		clear_entry_list_();
	}
	This& operator=(const This& rhs)
	{
		clear(rhs.m_entry_list->size());

		bool is_rehash_enabled = m_rehash;
		if(is_rehash_enabled)
			disable_rehash();

		iterator end = rhs.end();
		for(iterator it = rhs.begin(); it != end; ++it)
			insert(it.key(), it.get());

		if(is_rehash_enabled)
			enable_rehash();

		return *this;
	}

	void insert(const Key& key, const Value& data)
	{
		if(m_rehash && (m_num_entries / m_entry_list->size() > DEFAULT_MAX_DENSITY))
			rehash_();

		insert_entry_(new Entry(m_hash_func(key), key, data, NULL));
	}
	void erase(const iterator& it)
	{
		erase_entry_(it);
	}

	iterator find(const Key& key)
	{
		EntryList::iterator it = m_entry_list->begin() + hash_value_to_index_(m_hash_func(key));
		Entry* entry = *it;

		while(entry)
		{
			if(entry->key == key) return iterator(entry, m_entry_list, it);
			entry = entry->next;
		}

		return end();
	}
	const_iterator find(const Key& key) const
	{
		EntryList::iterator it = m_entry_list->begin() + _hash_value_to_index(m_hash_func(key));
		Entry* entry = *it;

		while(entry)
		{
			if(entry->key == key) return iterator(entry, m_entry_list, it);
			entry = entry->next;
		}

		return end();
	}

	void clear(size_type new_size)
	{
		clear_entry_list_();
		m_entry_list = new EntryList(hash_table_new_size(new_size));
	}

	//
	// accessors
	//
	iterator begin()
	{
		return iterator(*(m_entry_list->begin()), m_entry_list, m_entry_list->begin());
	}
	const_iterator begin() const
	{
		return iterator(*(m_entry_list->begin()), m_entry_list, m_entry_list->begin());
	}
	iterator end()
	{
		return iterator(NULL, m_entry_list, m_entry_list->end());
	}
	const_iterator end() const
	{
		return iterator(NULL, m_entry_list, m_entry_list->end());
	}

	size_type size() const
	{
		return m_num_entries;
	}
	bool empty() const
	{
		return size() == 0;
	}

	// enable/disable_rehash() は iterator を使って各要素にアクセスするときに呼ぶこと。
	// なぜなら途中で rehash されて iterator の内部データが正しい位置情報をもつことが
	// 出来なくなってしまう可能性があるから。
	void enable_rehash()
	{
		m_rehash = true;
	}
	void disable_rehash()
	{
		m_rehash = false;
	}

	//
	// operators
	//
	Value& operator[](const Key& key) {return *find(key);}
	const Value& operator[](const Key& key) const {return *find(key);}

private:
	//
	// data
	//
	HashFunc m_hash_func;
	uint m_num_entries;
	EntryList* m_entry_list;
	bool m_rehash;

	//
	// methods
	//
	uint hash_value_to_index_(uint hash_val) const
	{
		return hash_val % m_entry_list->size();
	}

	void insert_entry_(Entry* entry)
	{
		uint entry_point = hash_value_to_index_(entry->hash_value);

		// リストの先頭に挿入
		entry->next = m_entry_list->at(entry_point);
		m_entry_list->at(entry_point) = entry;

		++m_num_entries;
	}
	void erase_entry_(const iterator& it)
	{
		assert(it.m_entry);

		uint entry_point = hash_value_to_index_(it.m_entry->hash_value);

		Entry** entry = &m_entry_list->at(entry_point);
		while(*entry && *entry != it.m_entry)
			entry = &(*entry)->next;

		if(*entry == NULL)
			return;// 見つからず

		Entry* del_entry = *entry;
		*entry = (*entry)->next;

		delete del_entry;
	}

	void rehash_()
	{
		// 新しいリストの作成
		EntryList* old_list = m_entry_list;
		m_entry_list = new EntryList(hash_table_new_size(old_list->size() +1));
		m_num_entries = 0;

		Entry *entry = NULL, *next_entry;
		EntryList::iterator it, end = old_list->end();
		for(it = old_list->begin(); it != end; ++it)
		{
			entry = *it;
			while(entry)
			{
				next_entry = entry->next;// _insert_entry で next は変更される。
				insert_entry_(entry);
				entry = next_entry;
			}
		}

		delete old_list;
	}

	void clear_entry_list_()
	{
		delete m_entry_list;
		m_entry_list = NULL;
		m_num_entries = 0;
	}

};// class HashTable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef IntHashFunc    int_hash_func_t;
	typedef StringHashFunc string_hash_func_t;
	#define hash_table_t   HashTable
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::IntHashFunc    NIntHashFunc;
	typedef ntk::StringHashFunc NStringHashFunc;
	#define NHashTable          ntk::HashTable
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::IntHashFunc    ntk_int_hash_func;
	typedef ntk::StringHashFunc ntk_string_hash_func;
	#define ntk_hash_table      ntk::HashTable
	#endif
#endif


#endif//EOH
