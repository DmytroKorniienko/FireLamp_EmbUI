/*
	LList.h - V1.1 - Generic LList implementation
	Works better with FIFO, because LIFO will need to
	search the entire List to find the last one;

	For instructions, go to https://github.com/ivanseidel/LList

	Created by Ivan Seidel Gomes, March, 2013.
	Released into the public domain.
*/


#ifndef LList_h
#define LList_h

#include <stddef.h>

template<class T>
struct LListNode
{
	T data;
	LListNode<T> *next;
};

template <typename T>
class LList{

protected:
	int _size;
	LListNode<T> *root;
	LListNode<T>	*last;

	// Helps "get" method, by saving last position
	LListNode<T> *lastNodeGot;
	int lastIndexGot;
	// isCached should be set to FALSE
	// everytime the list suffer changes
	bool isCached;

	LListNode<T>* getNode(int index);

	LListNode<T>* findEndOfSortedString(LListNode<T> *p, int (*cmp)(T &, T &));

public:
	LList();
	LList(int sizeIndex, T _t); //initiate list size and default value
	~LList();

	/*
		Returns current size of LList
	*/
	virtual int size();
	/*
		Adds a T object in the specified index;
		Unlink and link the LList correcly;
		Increment _size
	*/
	virtual bool add(int index, T);
	/*
		Adds a T object in the end of the LList;
		Increment _size;
	*/
	virtual bool add(T);
	/*
		Adds a T object in the start of the LList;
		Increment _size;
	*/
	virtual bool unshift(T);
	/*
		Set the object at index, with T;
	*/
	virtual bool set(int index, T);
	/*
		Remove object at index;
		If index is not reachable, returns false;
		else, decrement _size
	*/
	virtual T remove(int index);
	/*
		Remove last object;
	*/
	virtual T pop();

	virtual T end();

	/*
		Remove first object;
	*/
	virtual T shift();
	/*
		Get the index'th element on the list;
		Return Element if accessible,
		else, return false;
	*/
	virtual T get(int index);

	/*
		Clear the entire array
	*/
	virtual void clear();

	/*
		Sort the list, given a comparison function
	*/
	virtual void sort(int (*cmp)(T &, T &));

		// add support to array brakets [] operator
	inline T& operator[](int index);
	inline T& operator[](size_t& i) { return this->get(i); }
  	inline const T& operator[](const size_t& i) const { return this->get(i); }

};

// Initialize LList with false values
template<typename T>
LList<T>::LList()
{
	root=NULL;
	last=NULL;
	_size=0;

	lastNodeGot = root;
	lastIndexGot = 0;
	isCached = false;
}

// Clear Nodes and free Memory
template<typename T>
LList<T>::~LList()
{
	LListNode<T>* tmp;
	while(root!=NULL)
	{
		tmp=root;
		root=root->next;
		delete tmp;
	}
	last = NULL;
	_size=0;
	isCached = false;
}

/*
	Actualy "logic" coding
*/

template<typename T>
LListNode<T>* LList<T>::getNode(int index){

	int _pos = 0;
	LListNode<T>* current = root;

	// Check if the node trying to get is
	// immediatly AFTER the previous got one
	if(isCached && lastIndexGot <= index){
		_pos = lastIndexGot;
		current = lastNodeGot;
	}

	while(_pos < index && current){
		current = current->next;

		_pos++;
	}

	// Check if the object index got is the same as the required
	if(_pos == index){
		isCached = true;
		lastIndexGot = index;
		lastNodeGot = current;

		return current;
	}

	return NULL;
}

template<typename T>
int LList<T>::size(){
	return _size;
}

template<typename T>
LList<T>::LList(int sizeIndex, T _t){
	for (int i = 0; i < sizeIndex; i++){
		add(_t);
	}
}

template<typename T>
bool LList<T>::add(int index, T _t){

	if(index >= _size)
		return add(_t);

	if(index == 0)
		return unshift(_t);

	LListNode<T> *tmp = new LListNode<T>(),
				 *_prev = getNode(index-1);
	tmp->data = _t;
	tmp->next = _prev->next;
	_prev->next = tmp;

	_size++;
	isCached = false;

	return true;
}

template<typename T>
bool LList<T>::add(T _t){

	LListNode<T> *tmp = new LListNode<T>();
	tmp->data = _t;
	tmp->next = NULL;

	if(root){
		// Already have elements inserted
		last->next = tmp;
		last = tmp;
	}else{
		// First element being inserted
		root = tmp;
		last = tmp;
	}

	_size++;
	isCached = false;

	return true;
}

template<typename T>
bool LList<T>::unshift(T _t){

	if(_size == 0)
		return add(_t);

	LListNode<T> *tmp = new LListNode<T>();
	tmp->next = root;
	tmp->data = _t;
	root = tmp;

	_size++;
	isCached = false;

	return true;
}


template<typename T>
T& LList<T>::operator[](int index) {
	return getNode(index)->data;
}

template<typename T>
bool LList<T>::set(int index, T _t){
	// Check if index position is in bounds
	if(index < 0 || index >= _size)
		return false;

	getNode(index)->data = _t;
	return true;
}

template<typename T>
T LList<T>::pop(){
	if(_size <= 0)
		return T();

	isCached = false;

	if(_size >= 2){
		LListNode<T> *tmp = getNode(_size - 2);
		T ret = tmp->next->data;
		delete(tmp->next);
		tmp->next = NULL;
		last = tmp;
		_size--;
		return ret;
	}else{
		// Only one element left on the list
		T ret = root->data;
		delete(root);
		root = NULL;
		last = NULL;
		_size = 0;
		return ret;
	}
}

template<typename T>
T LList<T>::end(){
	if(_size <= 0)
		return T();
	return last->data;
}

template<typename T>
T LList<T>::shift(){
	if(_size <= 0)
		return T();

	if(_size > 1){
		LListNode<T> *_next = root->next;
		T ret = root->data;
		delete(root);
		root = _next;
		_size --;
		isCached = false;

		return ret;
	}else{
		// Only one left, then pop()
		return pop();
	}

}

template<typename T>
T LList<T>::remove(int index){
	if (index < 0 || index >= _size)
	{
		return T();
	}

	if(index == 0)
		return shift();

	if (index == _size-1)
	{
		return pop();
	}

	LListNode<T> *tmp = getNode(index - 1);
	LListNode<T> *toDelete = tmp->next;
	T ret = toDelete->data;
	tmp->next = tmp->next->next;
	delete(toDelete);
	_size--;
	isCached = false;
	return ret;
}


template<typename T>
T LList<T>::get(int index){
	LListNode<T> *tmp = getNode(index);

	return (tmp ? tmp->data : T());
}

template<typename T>
void LList<T>::clear(){
	while(size() > 0)
		shift();
}

template<typename T>
void LList<T>::sort(int (*cmp)(T &, T &)){
	if(_size < 2) return; // trivial case;

	for(;;) {

		LListNode<T> **joinPoint = &root;

		while(*joinPoint) {
			LListNode<T> *a = *joinPoint;
			LListNode<T> *a_end = findEndOfSortedString(a, cmp);

			if(!a_end->next	) {
				if(joinPoint == &root) {
					last = a_end;
					isCached = false;
					return;
				}
				else {
					break;
				}
			}

			LListNode<T> *b = a_end->next;
			LListNode<T> *b_end = findEndOfSortedString(b, cmp);

			LListNode<T> *tail = b_end->next;

			a_end->next = NULL;
			b_end->next = NULL;

			while(a && b) {
				if(cmp(a->data, b->data) <= 0) {
					*joinPoint = a;
					joinPoint = &a->next;
					a = a->next;
				}
				else {
					*joinPoint = b;
					joinPoint = &b->next;
					b = b->next;
				}
			}

			if(a) {
				*joinPoint = a;
				while(a->next) a = a->next;
				a->next = tail;
				joinPoint = &a->next;
			}
			else {
				*joinPoint = b;
				while(b->next) b = b->next;
				b->next = tail;
				joinPoint = &b->next;
			}
		}
	}
}

template<typename T>
LListNode<T>* LList<T>::findEndOfSortedString(LListNode<T> *p, int (*cmp)(T &, T &)) {
	while(p->next && cmp(p->data, p->next->data) <= 0) {
		p = p->next;
	}

	return p;
}

#endif
