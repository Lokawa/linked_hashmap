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
	friend class iterator;
	friend class const_iterator;
	struct hashnode
	{
		hashnode *next,*pre,*hnext;
		pair<const Key,T> *data;
		hashnode(const pair<const Key,T> &a,hashnode *hn=nullptr,hashnode *p=nullptr,hashnode *n=nullptr)
		{
			data=new pair<const Key,T>(a);
			hnext=hn;
			next=n;
			pre=p;
		}
		hashnode()
		{
			data=nullptr;
			hnext=nullptr;
			next=nullptr;
			pre=nullptr;
		}
		~hashnode()
		{
			if (data!=nullptr) delete data;
		}
	};
	size_t sizee,capacity;
	double loadfactor;
	Hash hash;
	Equal equal;
	hashnode *head,*tail,*map;
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::linked_hashmap as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
 
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = linked_hashmap.begin(); --it;
	 *       or it = linked_hashmap.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		hashnode *position;
	public:
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. 
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		using difference_type = std::ptrdiff_t;
		using value_type = typename linked_hashmap::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::output_iterator_tag;
		
		friend class linked_hashmap<Key,T,Hash,Equal>;
		friend class const_iterator;
		
		iterator() {
			position=nullptr;
			// TODO
		}
		iterator(const iterator &other) {
			position=other.position;
			// TODO
		}
		iterator(const const_iterator &other)
		{
			position=other.position;
		}
		iterator(hashnode *p)
		{
			position=p;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if (position->next==nullptr) throw sjtu::invalid_iterator();
			iterator t=*this;
			position=position->next;
			return t;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (position->next==nullptr) throw sjtu::invalid_iterator();
			position=position->next;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if (position->pre->pre==nullptr) throw sjtu::invalid_iterator();
			iterator t=*this;
			position=position->pre;
			return t;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (position->pre->pre==nullptr) throw sjtu::invalid_iterator();
			position=position->pre;
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return *position->data;
		}
		bool operator==(const iterator &rhs) const {
			return position==rhs.position;
		}
		bool operator==(const const_iterator &rhs) const {
			return position==rhs.position;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return position!=rhs.position;
		}
		bool operator!=(const const_iterator &rhs) const {
			return position!=rhs.position;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return position->data;
		}
	};
 
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
		hashnode *position;
			// data members.
	public:
		friend class iterator;
		friend class linked_hashmap<Key,T,Hash,Equal>;
		const_iterator() {
			position=nullptr;
			// TODO
		}
		const_iterator(const const_iterator &other) {
			position=other.position;
			// TODO
		}
		const_iterator(const iterator &other) {
			position=other.position;
			// TODO
		}
		const_iterator(hashnode *p)
		{
			position=p;
		}
		const_iterator operator++(int) 
		{
			if (position->next==nullptr) throw sjtu::invalid_iterator();
			const_iterator t=*this;
			position=position->next;
			return t;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator++() {
			if (position->next==nullptr) throw sjtu::invalid_iterator();
			position=position->next;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			if (position->pre->pre==nullptr) throw sjtu::invalid_iterator();
			const_iterator t=*this;
			position=position->pre;
			return t;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator--() {
			if (position->pre->pre==nullptr) throw sjtu::invalid_iterator();
			position=position->pre;
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		const value_type & operator*() const {
			return *position->data;
		}
		bool operator==(const iterator &rhs) const {
			return position==rhs.position;
		}
		bool operator==(const const_iterator &rhs) const {
			return position==rhs.position;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return position!=rhs.position;
		}
		bool operator!=(const const_iterator &rhs) const {
			return position!=rhs.position;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return position->data;
		}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
	};
 
	/**
	 * TODO two constructors
	 */
	linked_hashmap() {
		loadfactor=0.75;
		capacity=10;
		sizee=0;
		map=new hashnode[10];
		head=new hashnode;
		tail=new hashnode;
		tail->pre=head;
		head->next=tail;
	}
	linked_hashmap(const linked_hashmap &other) {
		capacity=10;
		loadfactor=0.75;
		sizee=other.sizee;
		head=new hashnode;
		tail=new hashnode;
		tail->pre=head;
		head->next=tail;
		while (capacity*loadfactor<=sizee) capacity=capacity<<1;
		map=new hashnode[capacity];
		for (hashnode *p=other.head->next;p!=other.tail;p=p->next)
		{
			size_t pos=hash(p->data->first)%capacity;
			map[pos].hnext=new hashnode(*p->data,map[pos].hnext,tail->pre,tail);
			tail->pre->next=map[pos].hnext;
			tail->pre=map[pos].hnext;
		}
	}
 
	/**
	 * TODO assignment operator
	 */
	linked_hashmap & operator=(const linked_hashmap &other) {
		if (&other==this) return *this;
		clear();
		if (other.sizee==0) return *this;
		delete []map;
		capacity=10;
		loadfactor=0.75;
		sizee=other.sizee;
		tail->pre=head;
		head->next=tail;
		while (capacity*loadfactor<=sizee) capacity=capacity<<1;
		map=new hashnode[capacity];
		for (hashnode *p=other.head->next;p!=other.tail;p=p->next)
		{
			size_t pos=hash(p->data->first)%capacity;
			map[pos].hnext=new hashnode(*p->data,map[pos].hnext,tail->pre,tail);
			tail->pre->next=map[pos].hnext;
			tail->pre=map[pos].hnext;
		}
		return *this;
	}
 
	/**
	 * TODO Destructors
	 */
	~linked_hashmap() {
		clear();
		delete []map;
		delete head;
		delete tail;
	}
 
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return p->data->second;
		}
		throw sjtu::index_out_of_bound();
	}
	const T & at(const Key &key) const {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return p->data->second;
		}
		throw sjtu::index_out_of_bound();
	}
 
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	void doublespace()
	{
		capacity=capacity<<1;
		hashnode *New=new hashnode[capacity];
		for (hashnode *p=head->next;p!=tail;p=p->next)
		{
			size_t pos=hash(p->data->first)%capacity;
			p->hnext=New[pos].hnext;
			New[pos].hnext=p;
		}
		delete []map;
		map=New;
	}

	T & operator[](const Key &key) {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return p->data->second;
		}
		pair<Key,T> p(key,T());
		if (sizee>=capacity*loadfactor) doublespace();
        pos=hash(key)%capacity;
		map[pos].hnext=new hashnode(p,map[pos].hnext,tail->pre,tail);
		tail->pre->next=map[pos].hnext;
		tail->pre=map[pos].hnext;
		sizee++;
		return map[pos].hnext->data->second;
	}
 
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return p->data->second;
		}
		throw sjtu::index_out_of_bound();
	}
 
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		return iterator(head->next);
	}
	const_iterator cbegin() const {
		return const_iterator(head->next);
	}
 
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		return iterator(tail);
	}
	const_iterator cend() const {
		return const_iterator(tail);
	}
 
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return sizee==0;
	}
 
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return sizee;
	}
 
	/**
	 * clears the contents
	 */
	void clear() {
		sizee=0;
		for (hashnode *p=head->next;p!=tail;)
		{
			hashnode *t=p->next;
			delete p;
			p=t;
		}
		head->next=tail;
		tail->pre=head;
		for (size_t i=0;i<capacity;i++) map[i].hnext=nullptr;
	}
 
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		size_t pos=hash(value.first)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,value.first)) return pair<iterator,bool>(iterator(p),false);
		}
		if (sizee>=capacity*loadfactor) doublespace();
        pos=hash(value.first)%capacity;
		map[pos].hnext=new hashnode(value,map[pos].hnext,tail->pre,tail);
		tail->pre->next=map[pos].hnext;
		tail->pre=map[pos].hnext;
		sizee++;
		return pair<iterator,bool>(iterator(map[pos].hnext),true);
	}
 
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if (pos.position->data==nullptr) throw sjtu::index_out_of_bound();
		size_t poss=hash(pos.position->data->first)%capacity;
		hashnode *t=map+poss;
		for (hashnode *p=map[poss].hnext;p!=nullptr;t=p,p=p->hnext)
		{
			if (p==pos.position) 
			{
				p->next->pre=p->pre;
				p->pre->next=p->next;
				t->hnext=p->hnext;
				delete p;
				sizee--;
				return;
			}
		}
		throw sjtu::index_out_of_bound();
	}
 
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 */
	size_t count(const Key &key) const {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return 1;
		}
		return 0;
	}
 
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return iterator(p);
		}
		return end();
	}
	const_iterator find(const Key &key) const {
		size_t pos=hash(key)%capacity;
		for (hashnode *p=map[pos].hnext;p!=nullptr;p=p->hnext)
		{
			if (equal(p->data->first,key)) return const_iterator(p);
		}
		return cend();
	}
};

}

#endif