#ifndef __STACK__
#define __STACK__

template <class T, template<class, class...> class Tcont>
class MyStack {
	Tcont<T> data;
public:
	void push(const T& val);
	void pop();
	T& top();
	bool empty();
	size_t size();
};

template <class T, template<class, class...> class Tcont>
void inline MyStack<T, Tcont>::push(const T& val) {
	data.push_back(val);
}

template <class T, template<class, class...> class Tcont>
void inline MyStack<T, Tcont>::pop() {
	if (data.empty()) throw "Delete a non existent element";
	data.pop_back();
}

template <class T, template<class, class...> class Tcont>
inline T& MyStack<T, Tcont>::top() {
	if (data.empty()) throw "Stack is empty";
	return data.back();
}

template <class T, template<class, class...> class Tcont>
inline bool MyStack<T, Tcont>::empty() {
	return (data.size() == 0);
}

template <class T, template<class, class...> class Tcont>
inline size_t MyStack<T, Tcont>::size() {
	return data.size();
}

#endif
