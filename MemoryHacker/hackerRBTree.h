#ifndef HACKER_RB_TREE_H
#define HACKER_RB_TREE_H

#include "memoryhacker.h"

typedef enum
{
	BLACKCOLOR,
	REDCOLOR
}nodecolor;

template <typename T1,typename T2>
struct RB_Tree_Node
{
	
	typedef RB_Tree_Node<T1,T2>* nodeptr;
	nodeptr parent;
	nodeptr left;
	nodeptr right;
	nodecolor color;
	T1 key;
	T2 value;
};

template<typename T1, typename T2,class Alloc = MemHacker>
class TreeNodeAlloc
{
	typedef Alloc treenodealloc;
	typedef RB_Tree_Node<T1,T2>* nodeptr;
	typedef RB_Tree_Node<T1, T2> nodest;
public:
	TreeNodeAlloc(){}
	~TreeNodeAlloc(){}

	static nodeptr CreateNode(T1 key,T2 value)
	{
		return (nodeptr)MemHacker::HackAlloc(sizeof(RB_Tree_Node<T1,T2>));
	}
	static nodeptr CreateNode()
	{
		return (nodeptr)MemHacker::HackAlloc(sizeof(RB_Tree_Node<T1, T2>));
	}

	static void PutNode(nodeptr& node)
	{
		MemHacker::HackDealloc((void*)node, sizeof(nodest));
	}
};

template<typename T1,typename T2>
class RBTree_iterator
{
	typedef RB_Tree_Node<T1, T2>* nodeptr;
	typedef RBTree_iterator self;
public:
	RBTree_iterator() :m_node(nullptr){
		
	}
	RBTree_iterator(nodeptr _node) :m_node(nullptr){
		m_node = _node;
	}
	~RBTree_iterator(){}
public:
	self& operator++()
	{
		incremnet();
		return *this;
	}
	self& operator++(int)
	{
		self tmp = *this;
		incremnet();
		return tmp;
	}

	self& operator--()
	{
		decreament();
		return *this;
	}

	self& operator--(int)
	{
		self tmp = *this;
		decreament();
		return tmp;
	}
	nodeptr operator->()
	{
		return m_node;
	}

	nodeptr& operator*()
	{
		return m_node;
	}
private:
	void incremnet();

	void decreament();
private:
	nodeptr m_node;
};

template<typename T1, typename T2>
void RBTree_iterator<T1,T2>::incremnet()
{
	if (m_node->right != 0)
	{
		m_node = m_node->right;
		while (m_node->left != 0)
			m_node = m_node->left;
	}
	else
	{
		nodeptr y = m_node->parent;
		while (m_node == y->right)
		{
			m_node = y;
			y = y->parent;
		}
		if (m_node->right != y)
		{
			m_node = y;
		}
	}
}

template<typename T1, typename T2>
void RBTree_iterator<T1,T2>::decreament()
{
	if (m_node->color == REDCOLOR && m_node->parent->parent == m_node)
		m_node = m_node->right;
	else if (m_node->left != 0)
	{
		nodeptr y = m_node->left;
		while (y->right != 0)
			y = y->right;
		m_node = y; 
	}
	else
	{
		nodeptr y = m_node->parent;
		while (m_node == y->left)
		{
			m_node = y;
			y = y->parent;
		}
		m_node = y;
	}
}

template<typename T1,typename T2,typename keycompare, class ALLOC = TreeNodeAlloc<T1,T2>>
class hackerRBTree
{
	
	typedef T2 value_type;
	typedef T1 value_key;
public:
	typedef RB_Tree_Node<T1, T2>* nodeptr;
	typedef RBTree_iterator<T1, T2> iterator;
	hackerRBTree() :header(nullptr), _M_node_count(0){
		header = ALLOC::CreateNode();
		header->color = nodecolor::REDCOLOR;
		header->left = header;
		header->right = header;
		_M_root() = 0;
	}
	~hackerRBTree(){}
	void remove_node(iterator node)
	{
		nodeptr tmp = tree_rebalance_for_erase(*node, _M_root(), _M_leftmost(), _M_rightmost());
		ALLOC::PutNode(tmp);
		--_M_node_count;
	}
	bool insert_unequal(const value_key& k, const value_type& v)
	{
		nodeptr y = header;
		nodeptr x = _M_root();
		while (x != nullptr)
		{
			y = x;
			if (k == x->key)
			{
				printf("input value key has exist\n");
				return false;
			}
			x = _M_Comp(k, x->key) ? _S_left(x) : _S_right(x);
		}
		insert(x, y, k, v, _M_Comp);
		return true;
	}

	iterator findByKey(T1 key)
	{
		nodeptr y = header;
		nodeptr x = _M_root();
		while (x != nullptr)
		{
			if (_M_Comp(x->key, key))
			{
				x = x->right;
			}
			else
			{
				x = x->left;
			}
			if (x!= nullptr && x->key == key)
			{
				return x;
			}
		}
		if (x == nullptr)
		{
			nodeptr tmp = nullptr;
			return tmp;
		}
	}

	iterator begin()
	{
		return _M_leftmost();
	}

	iterator end()
	{
		return _M_rightmost();
	}

	bool empty()
	{
		return _M_node_count == 0;
	}

	size_t size()
	{
		return _M_node_count;
	}


private:
	nodeptr _S_minimum(nodeptr __x)
	{
		while (__x->left != 0) __x = __x->left;
		return __x;
	}

	nodeptr _S_maximum(nodeptr __x)
	{
		while (__x->right != 0) __x = __x->right;
		return __x;
	}

	

	void insert(nodeptr x, nodeptr y, const value_key key, const value_type value, keycompare compare)
	{
		nodeptr __x = (nodeptr)x;
		nodeptr __y = (nodeptr)y;
		nodeptr __z;

		if (__y == header || __x != 0 ||
			compare(key, __y->key))
		{
			__z = ALLOC::CreateNode(key, value);
			__z->key = key;
			__z->value = value;
			_S_left(__y) = __z;             
			if (__y == header) {
				_M_root() = __z;
				_M_rightmost() = __z;
			}
			else if (__y == _M_leftmost())
				_M_leftmost() = __z;  
		}
		else {
			__z = ALLOC::CreateNode(key, value);
			__z->key = key;
			__z->value = value;
			_S_right(__y) = __z;
			if (__y == _M_rightmost())
				_M_rightmost() = __z;  
		}
		_S_parent(__z) = __y;
		_S_left(__z) = 0;
		_S_right(__z) = 0;
		_Rb_tree_rebalance(__z, header->parent);
		++_M_node_count;
		return ;
	}

	void _Rb_tree_rebalance(nodeptr __x, nodeptr& __root);
	void _Rb_tree_rotate_left(nodeptr __x, nodeptr& __root);
	void _Rb_tree_rotate_right(nodeptr __x, nodeptr& __root);
	nodeptr tree_rebalance_for_erase(nodeptr __z, nodeptr& __root, nodeptr& __leftmost, nodeptr& __rightmost);
private:

	nodeptr& _M_root()
	{
		return header->parent;
	}

	nodeptr& _M_leftmost()
	{
		return header->left;
	}
	nodeptr& _M_rightmost()
	{
		return header->right;
	}

	nodeptr& _S_left(nodeptr x)
	{
		return x->left;
	}

	nodeptr& _S_right(nodeptr x)
	{
		return x->right;
	}

	nodeptr& _S_parent(nodeptr x)
	{
		return x->parent;
	}

private:
	nodeptr header;
	size_t _M_node_count;
	keycompare _M_Comp;
	
};

template<typename T1, typename T2, typename keycompare, class ALLOC = TreeNodeAlloc<T1, T2>>
typename hackerRBTree<T1, T2, keycompare, ALLOC>::nodeptr hackerRBTree<T1, T2, keycompare, ALLOC>::tree_rebalance_for_erase(nodeptr __z, nodeptr& __root, nodeptr& __leftmost, nodeptr& __rightmost)
{
	nodeptr __y = __z;
	nodeptr __x = 0;
	nodeptr __x_parent = 0;
	if (__y->left == 0)     
		__x = __y->right;    
	else
		if (__y->right == 0)  
			__x = __y->left;    
		else {                   
			__y = __y->right;   
			while (__y->left != 0)
				__y = __y->left;
			__x = __y->right;
		}
		if (__y != __z) {          
			__z->left->parent = __y;
			__y->left = __z->left;
			if (__y != __z->right) {
				__x_parent = __y->parent;
				if (__x) __x->parent = __y->parent;
				__y->parent->left = __x;      
				__y->right = __z->right;
				__z->right->parent = __y;
			}
			else
				__x_parent = __y;
			if (__root == __z)
				__root = __y;
			else if (__z->parent->left == __z)
				__z->parent->left = __y;
			else
				__z->parent->right = __y;
			__y->parent = __z->parent;
			nodecolor tmp = __y->color;
			__y->color = __z->color;
			__z->color = tmp;
			__y = __z;
			
		}
		else {                        
			__x_parent = __y->parent;
			if (__x) __x->parent = __y->parent;
			if (__root == __z)
				__root = __x;
			else
				if (__z->parent->left == __z)
					__z->parent->left = __x;
				else
					__z->parent->right = __x;
			if (__leftmost == __z)
				if (__z->right == 0)       
					__leftmost = __z->parent;
			
				else
					__leftmost = _S_minimum(__x);
			if (__rightmost == __z)
				if (__z->left == 0)         
					__rightmost = __z->parent;
			
				else                      
					__rightmost = _S_maximum(__x);
		}
		if (__y->color != REDCOLOR) {
			while (__x != __root && (__x == 0 || __x->color == BLACKCOLOR))
				if (__x == __x_parent->left) {
				nodeptr __w = __x_parent->right;
				if (__w->color == REDCOLOR) {
					__w->color = BLACKCOLOR;
					__x_parent->color = REDCOLOR;
					_Rb_tree_rotate_left(__x_parent, __root);
					__w = __x_parent->right;
				}
				if ((__w->left == 0 ||
					__w->left->color == BLACKCOLOR) &&
					(__w->right == 0 ||
					__w->right->color == BLACKCOLOR)) {
					__w->color = REDCOLOR;
					__x = __x_parent;
					__x_parent = __x_parent->parent;
				}
				else {
					if (__w->right == 0 ||
						__w->right->color == BLACKCOLOR) {
						if (__w->left) __w->left->color = BLACKCOLOR;
						__w->color = REDCOLOR;
						_Rb_tree_rotate_right(__w, __root);
						__w = __x_parent->right;
					}
					__w->color = __x_parent->color;
					__x_parent->color = BLACKCOLOR;
					if (__w->right) __w->right->color = BLACKCOLOR;
					_Rb_tree_rotate_left(__x_parent, __root);
					break;
				}
				}
				else {                  // same as above, with _M_right <-> _M_left.
					nodeptr __w = __x_parent->left;
					if (__w->color == REDCOLOR) {
						__w->color = BLACKCOLOR;
						__x_parent->color = REDCOLOR;
						_Rb_tree_rotate_right(__x_parent, __root);
						__w = __x_parent->left;
					}
					if ((__w->right == 0 ||
						__w->right->color == BLACKCOLOR) &&
						(__w->left == 0 ||
						__w->left->color == BLACKCOLOR)) {
						__w->color = REDCOLOR;
						__x = __x_parent;
						__x_parent = __x_parent->parent;
					}
					else {
						if (__w->left == 0 ||
							__w->left->color == BLACKCOLOR) {
							if (__w->right) __w->right->color = BLACKCOLOR;
							__w->color = REDCOLOR;
							_Rb_tree_rotate_left(__w, __root);
							__w = __x_parent->left;
						}
						__w->color = __x_parent->color;
						__x_parent->color = BLACKCOLOR;
						if (__w->left) __w->left->color = BLACKCOLOR;
						_Rb_tree_rotate_right(__x_parent, __root);
						break;
					}
				}
				if (__x) __x->color = BLACKCOLOR;
		}
		return __y;
}


template<typename T1, typename T2, typename keycompare, class ALLOC = TreeNodeAlloc<T1, T2>>
void hackerRBTree<T1, T2, keycompare, ALLOC>::_Rb_tree_rebalance(nodeptr __x, nodeptr& __root)
{
	__x->color = REDCOLOR;
	while (__x != __root && __x->parent->color == REDCOLOR) {
		if (__x->parent == __x->parent->parent->left) {
			nodeptr __y = __x->parent->parent->right;
			if (__y && __y->color == REDCOLOR) {
				__x->parent->color = BLACKCOLOR;
				__y->color = BLACKCOLOR;
				__x->parent->parent->color = REDCOLOR;
				__x = __x->parent->parent;
			}
			else {
				if (__x == __x->parent->right) {
					__x = __x->parent;
					_Rb_tree_rotate_left(__x, __root);
				}
				__x->parent->color = BLACKCOLOR;
				__x->parent->parent->color = REDCOLOR;
				_Rb_tree_rotate_right(__x->parent->parent, __root);
			}
		}
		else {
			nodeptr __y = __x->parent->parent->left;
			if (__y && __y->color == REDCOLOR) {
				__x->parent->color = BLACKCOLOR;
				__y->color = BLACKCOLOR;
				__x->parent->parent->color = REDCOLOR;
				__x = __x->parent->parent;
			}
			else {
				if (__x == __x->parent->left) {
					__x = __x->parent;
					_Rb_tree_rotate_right(__x, __root);
				}
				__x->parent->color = BLACKCOLOR;
				__x->parent->parent->color = REDCOLOR;
				_Rb_tree_rotate_left(__x->parent->parent, __root);
			}
		}
	}
	__root->color = BLACKCOLOR;
}

template<typename T1, typename T2, typename keycompare, class ALLOC = TreeNodeAlloc<T1, T2>>
void hackerRBTree<T1, T2, keycompare,ALLOC>::_Rb_tree_rotate_left(nodeptr __x, nodeptr& __root)
{
	nodeptr __y = __x->right;
	__x->right = __y->left;
	if (__y->left != 0)
		__y->left->parent = __x;
	__y->parent = __x->parent;

	if (__x == __root)
		__root = __y;
	else if (__x == __x->parent->left)
		__x->parent->left = __y;
	else
		__x->parent->right = __y;
	__y->left = __x;
	__x->parent = __y;
}

template<typename T1, typename T2, typename keycompare, class ALLOC = TreeNodeAlloc<T1, T2>>
void hackerRBTree<T1, T2, keycompare, ALLOC>::_Rb_tree_rotate_right(nodeptr __x, nodeptr& __root)
{
	nodeptr __y = __x->left;
	__x->left = __y->right;
	if (__y->right != 0)
		__y->right->parent = __x;
	__y->parent = __x->parent;

	if (__x == __root)
		__root = __y;
	else if (__x == __x->parent->right)
		__x->parent->right = __y;
	else
		__x->parent->left = __y;
	__y->right = __x;
	__x->parent = __y;
}

#endif