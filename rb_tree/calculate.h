#ifndef CALCULATE_H
#define CALCULATE_H

template <class T>
int rb_tree<T>::total_size_sbtrs_nodes_ng(const int max) const
{
	if( max<=0 )
		return 0;
	int total = 0;
	rb_tree<T>::total_size_sbtrs_nodes_ng_node(root, max, total);
	return total;
}

template <class T>
int rb_tree<T>::total_size_sbtrs_nodes_ng_node(const rb_tree_node<T> *node, const int max, int &total)
{
	if( node==nullptr )
		return 0;

	int num_values = 1 + rb_tree<T>::total_size_sbtrs_nodes_ng_node(node->left, max, total) +
		rb_tree<T>::total_size_sbtrs_nodes_ng_node(node->right, max, total);

	if( num_values<=max )
		total += num_values;
	return num_values;
}


template <class T>
int rb_tree<T>::total_size_sbtrs_levels_ng(const int max) const
{
	if( max<=0 )
		return 0;
	int total = 0, num_levels;
	rb_tree<T>::total_size_sbtrs_levels_ng_node(root, max, num_levels, total);
	return total;
}

template <class T>
int rb_tree<T>::total_size_sbtrs_levels_ng_node(const rb_tree_node<T> *node, const int max, int &num_levels, int &total)
{
	num_levels = 0;
	if( node==nullptr )
		return 0;

	int num_levels_right;
	int num_values = 1 + rb_tree<T>::total_size_sbtrs_levels_ng_node(node->left, max, num_levels, total) +
		rb_tree<T>::total_size_sbtrs_levels_ng_node(node->right, max, num_levels_right, total);
	if( num_levels_right>num_levels )
		num_levels = num_levels_right;
	num_levels++;

	if( num_levels<=max )
		total += num_values;
	return num_values;
}


template <class T>
int rb_tree<T>::total_size_sbtrs_width_ng(const int max) const
{
	if( max<=0 )
		return 0;
	return rb_tree<T>::total_size_sbtrs_width_ng_node(root, max);
}

template <class T>
int rb_tree<T>::total_size_sbtrs_width_ng_node(const rb_tree_node<T> *node, const int max)
{
	if( node==nullptr )
		return 0;

	int total = 1, level_width;
	for( int level = 2; (level_width = rb_tree<T>::num_values_level_node(node, level))!=0; level++ )
	{
		if( level_width>max )
			return rb_tree<T>::total_size_sbtrs_width_ng_node(node->left, max) +
				rb_tree<T>::total_size_sbtrs_width_ng_node(node->right, max);
		total += level*level_width;
	}

	return total;
}


template <class T>
int rb_tree<T>::num_values_level(const int level) const
{
	if( level<=0 )
		return 0;
	return num_values_level_node(root, level);
}

template <class T>
int rb_tree<T>::num_values_level_node(const rb_tree_node<T> *node, int level)
{
	int num = 0;

	while( node!=nullptr )
	{
		if( level==1 )
		{
			num++;
			break;
		}

		level--;
		num += rb_tree<T>::num_values_level_node(node->left, level);
		node = node->right;
	}

	return num;
}


template <class T>
int rb_tree<T>::total_size_paths_len(const int len) const
{
	if( len<=0 )
		return 0;
	return len*rb_tree<T>::num_leaves_level_node(root, len);
}

template <class T>
int rb_tree<T>::num_leaves_level_node(const rb_tree_node<T> *node, int level)
{
	int num = 0;

	while( node!=nullptr )
	{
		if( level==1 )
		{
			if( node->left==nullptr && node->right==nullptr )
				num++;
			break;
		}

		level--;
		num += rb_tree<T>::num_leaves_level_node(node->left, level);
		node = node->right;
	}

	return num;
}

#endif// CALCULATE_H
