#ifndef PRINT_H
#define PRINT_H

#include <cstdio>

inline void print_tabs(const int num);

template <class T>
void rb_tree<T>::print(const int level) const
{
	if( root==nullptr || level<=0 )
		return;

	print_node(root, level);
}

template <class T>
void rb_tree<T>::print_node(const rb_tree_node<T> *node, const int max_level, int level)
{
	while( node!=nullptr )
	{
		print_tabs(level - 1);
		node->print();

		if( level==max_level )
		{
			if( node->left==nullptr && node->right==nullptr )
				return;
			print_tabs(level);
			printf("...\n");
			return;
		}

		level++;
		print_node(node->left, max_level, level);
		node = node->right;
	}
}

inline void print_tabs(const int num)
{
	for( int i = 0; i<num; i++ )
	{
		printf("\t");
	}
}

#endif// PRINT_H
