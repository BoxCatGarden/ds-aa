// heap.h: interface for the heap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEAP_H__5AE9443A_0702_4035_B744_07A04E3C55A6__INCLUDED_)
#define AFX_HEAP_H__5AE9443A_0702_4035_B744_07A04E3C55A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//here bool Comp::prior(const T& a, const T& b) determine whether a is prior to b
template <typename T, typename Comp>
class heap  
{
	T* _a;
	int n;

	int parent(int i) {return (i-1)/2;}
	int lc(int i) {return 2*i+1;}
	int rc(int i) {return 2*i+2;}
	void siftdown(int i)  {
		T t;
		int j, halfN = n/2;
		while (i < halfN) {
			j = lc(i);
			if (j+1 < n && Comp::prior(_a[j+1], _a[j])) ++j;
			if (Comp::prior(_a[j], _a[i])) {
				t = _a[j], _a[j] = _a[i], _a[i] = t;
				i = j;
			} else break;
		}
	}

	void pushup(int i) {
		if (i < n) {
			T t;
			int j;
			while (i > 0 && Comp::prior(_a[i], _a[j=parent(i)])) {
				t = _a[j], _a[j] = _a[i], _a[i] = t;
				i = j;
			}
		}
	}
public:
	const int maxSize;  //the max size of the heap

	/* a: the collection of records
	   n: the current number of records in a
	   maxSize: the max size of the heap
	*/
	heap(T* a, int n, int maxSize = 50)
			:_a(a),n(n),maxSize(maxSize) {
		//here n/2-1=(n-1 -1)/2 is the parent position of the last one, where exists the last internal node
		for (int i = n/2-1; i >= 0 ; --i) siftdown(i);
	}
	virtual ~heap() {}

	//insert element into the heap; return false only if there isn't enough room for the new record
	bool insert(const T& element) {
		if (n < maxSize) {
			_a[n++] = element;
			pushup(n-1);
			return true;
		}
		return false;
	}

	//always the a[0], where a is the initializing parameter previously input
	const T& top() const {return _a[0];}

	//delete the top record by just covering it and rebuild the heap
	void pop() {
		_a[0] = _a[--n];
		siftdown(0);
	}

	/* delete the top record by swapping it with the current last record in a (initializing parameter),
	   which relates to the current heap size (a[size-1]), and rebuild the heap; thus, the top record
	   was restored in a, but not in the heap.
	*/
	void sort() {
		T t = _a[0];
		pop();
		_a[n] = t;
	}

	//return the copy of the top record as well as doing pop()
	T pull() {
		T t = _a[0];
		pop();
		return t;
	}

	//return the current size of the heap, which is the current number of records in it
	int size() const {return n;}
};

#endif // !defined(AFX_HEAP_H__5AE9443A_0702_4035_B744_07A04E3C55A6__INCLUDED_)
