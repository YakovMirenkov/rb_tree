#ifndef RB_TREE_H
#define RB_TREE_H

#include <cstdio>

#define DEFAULT_LEVEL_PRINT 0

template <class T> class rb_tree;
template <class T> class rb_tree_node;

template <class T>
class rb_tree_node: public T
{
	enum colors
	{
		invalid,
		red,
		black
	};
private:
	rb_tree_node *left = nullptr;
	rb_tree_node *right = nullptr;
	colors color = invalid;
public:
	rb_tree_node() = default;
	rb_tree_node(T &&x): T(static_cast<T &&>(x))
	{
		left = nullptr;
		right = nullptr;
		color = rb_tree_node<T>::red;
	}
	void print() const
	{
		T::print();
		switch( color )
		{
		case red:
			printf(" (red)\n");
			return;
		case black:
			printf(" (black)\n");
			return;
		default:
			printf(" (invalid)\n");
		}
	}

	friend class rb_tree<T>;
};

template <class T>
class rb_tree
{
private:
	rb_tree_node<T> *root = nullptr;
	// root->color==rb_tree_node<T>::black

	void rm() { rb_tree<T>::rm_node(root); root = nullptr; }
	static void rm_node(rb_tree_node<T> *node);

	int add_value(T *new_value);
	static int add_value_node(rb_tree_node<T> *node, T *new_value);
	static void rotate_right_middle(rb_tree_node<T> *&node)
	{
		rb_tree_node<T> *old = node;
		node = node->left->right;
		rb_tree_node<T> *node_old_left = node->left;
		rb_tree_node<T> *node_old_right = node->right;
		node->left = old->left;
		node->right = old;
		node->left->right = node_old_left;
		old->left = node_old_right;
		node->color = rb_tree_node<T>::black;
		old->color = rb_tree_node<T>::red;
	}
	static void rotate_right_not_middle(rb_tree_node<T> *&node)
	{
		rb_tree_node<T> *old = node;
		node = node->left;
		rb_tree_node<T> *node_old_right = node->right;
		node->right = old;
		old->left = node_old_right;
		node->color = rb_tree_node<T>::black;
		old->color = rb_tree_node<T>::red;
	}
	static void rotate_left_middle(rb_tree_node<T> *&node)
	{
		rb_tree_node<T> *old = node;
		node = node->right->left;
		rb_tree_node<T> *node_old_left = node->left;
		rb_tree_node<T> *node_old_right = node->right;
		node->right = old->right;
		node->left = old;
		node->right->left = node_old_right;
		old->right = node_old_left;
		node->color = rb_tree_node<T>::black;
		old->color = rb_tree_node<T>::red;
	}
	static void rotate_left_not_middle(rb_tree_node<T> *&node)
	{
		rb_tree_node<T> *old = node;
		node = node->right;
		rb_tree_node<T> *node_old_left = node->left;
		node->left = old;
		old->right = node_old_left;
		node->color = rb_tree_node<T>::black;
		old->color = rb_tree_node<T>::red;
	}
public:
	enum RETURN_CODES
	{
		SUCCESS,
		TWO_RED_LEFT,
		TWO_RED_RIGHT,
		UNKNOWN,
		ROTATE_RIGHT_MIDDLE,
		ROTATE_RIGHT_NOT_MIDDLE,
		ROTATE_LEFT_MIDDLE,
		ROTATE_LEFT_NOT_MIDDLE,
		ALLOC_ERROR,
		OPEN_ERROR,
		READ_ERROR
	};

	rb_tree() = default;
	~rb_tree() { rm(); }
	int read(const char *filename);
	void print(const int level = DEFAULT_LEVEL_PRINT) const;
	static void print_node(const rb_tree_node<T> *node, const int max_level = DEFAULT_LEVEL_PRINT, int level = 1);

	// Calculate:
	// Total number of values in subtrees with number of nodes non-greater than max
	int total_size_sbtrs_nodes_ng(const int max) const;
	static int total_size_sbtrs_nodes_ng_node(const rb_tree_node<T> *node, const int max, int &total);
	// Total number of values in subtrees with numder of levels non-greater than max
	int total_size_sbtrs_levels_ng(const int max) const;
	static int total_size_sbtrs_levels_ng_node(const rb_tree_node<T> *node, const int max, int &num_levels, int &total);
	// Total number of values in subtrees with width non-greater than max
	int total_size_sbtrs_width_ng(const int max) const;
	static int total_size_sbtrs_width_ng_node(const rb_tree_node<T> *node, const int max);
	// Number of values on level
	int num_values_level(const int level) const;
	static int num_values_level_node(const rb_tree_node<T> *node, int level);
	// Total number of values in paths length len from root to leaf nodes
	int total_size_paths_len(const int len) const;
	// Number of leaves on level
	static int num_leaves_level_node(const rb_tree_node<T> *node, int level);
};


#include "add_value.h"
#include "print.h"
#include "calculate.h"


template <class T>
void rb_tree<T>::rm_node(rb_tree_node<T> *node)
{
	rb_tree_node<T> *tmp;
	while( node!=nullptr )
	{
		rb_tree<T>::rm_node(node->left);
		tmp = node->right;
		delete node;
		node = tmp;
	}
}

template <class T>
int rb_tree<T>::read(const char *filename)
{
	rm();

	FILE *in = fopen(filename, "r");
	if( !in )
		return OPEN_ERROR;

	T *new_value;
	if( (new_value = new T)==nullptr )
	{
		fclose(in);
		return ALLOC_ERROR;
	}

	int res_read;
	if( (res_read = new_value->read(in))!=T::SUCCESS )
	{
		delete new_value;
		fclose(in);
		switch( res_read )
		{
		case T::EOF_ERROR:
			return SUCCESS;
		case T::ALLOC_ERROR:
			return ALLOC_ERROR;
		default:
			return READ_ERROR;
		}
	}

	if( (root = new rb_tree_node<T>(static_cast<T &&>(*new_value)))==nullptr )
	{
		delete new_value;
		fclose(in);
		return ALLOC_ERROR;
	}
	root->color = rb_tree_node<T>::black;

	while( (res_read = new_value->read(in))==T::SUCCESS )
	{
		if( add_value(new_value)!=SUCCESS )
		{
			delete new_value;
			fclose(in);
			return ALLOC_ERROR;
		}
	}
	delete new_value;
	fclose(in);

	if( res_read==T::EOF_ERROR )
		return SUCCESS;
	rm();
	switch( res_read )
	{
	case T::ALLOC_ERROR:
		return ALLOC_ERROR;
	default:
		return READ_ERROR;
	}
}

#endif// RB_TREE_H
