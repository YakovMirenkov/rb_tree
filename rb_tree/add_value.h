#ifndef ADD_VALUE_H
#define ADD_VALUE_H

template <class T>
int rb_tree<T>::add_value(T *new_value)
{
	int res_add = rb_tree<T>::add_value_node(root, new_value);

	if( res_add==ALLOC_ERROR )
		return ALLOC_ERROR;
	if( res_add==SUCCESS || res_add==UNKNOWN )
		return SUCCESS;

	if( res_add==TWO_RED_LEFT || res_add==TWO_RED_RIGHT )
	{
		root->color = rb_tree_node<T>::black;
		return SUCCESS;
	}

	if( res_add==ROTATE_RIGHT_MIDDLE )
	{
		rb_tree<T>::rotate_right_middle(root);
		return SUCCESS;
	}
	if( res_add==ROTATE_RIGHT_NOT_MIDDLE )
	{
		rb_tree<T>::rotate_right_not_middle(root);
		return SUCCESS;
	}
	if( res_add==ROTATE_LEFT_MIDDLE )
	{
		rb_tree<T>::rotate_left_middle(root);
		return SUCCESS;
	}
	// res_add==ROTATE_LEFT_NOT_MIDDLE
	rb_tree<T>::rotate_left_not_middle(root);
	return SUCCESS;
}

template <class T>
int rb_tree<T>::add_value_node(rb_tree_node<T> *node, T *new_value)
{
	int res_add;

	if( *node>*new_value )
	{
		if( node->left==nullptr )
		{
			node->left = new rb_tree_node<T>(static_cast<T &&>(*new_value));
			if( node->left==nullptr )
				return ALLOC_ERROR;
			if( node->color==rb_tree_node<T>::red )
				return TWO_RED_LEFT;
			return SUCCESS;
		}

		res_add = rb_tree<T>::add_value_node(node->left, new_value);

		if( res_add==ALLOC_ERROR)
			return ALLOC_ERROR;
		if( res_add==SUCCESS )
			return SUCCESS;
		if( res_add==UNKNOWN )
		{// node->left->color==rb_tree_node<T>::red
			if( node->color==rb_tree_node<T>::red )
				return TWO_RED_LEFT;
			return SUCCESS;
		}

		if( res_add==TWO_RED_LEFT || res_add==TWO_RED_RIGHT )
		{
			if( node->right==nullptr || node->right->color==rb_tree_node<T>::black )
			{
				if( res_add==TWO_RED_LEFT )
					return ROTATE_RIGHT_NOT_MIDDLE;
				return ROTATE_RIGHT_MIDDLE;
			}
			// node->right->color==rb_tree_node<T>::red
			node->left->color = rb_tree_node<T>::black;
			node->right->color = rb_tree_node<T>::black;
			node->color = rb_tree_node<T>::red;
			return UNKNOWN;
		}

		if( res_add==ROTATE_RIGHT_MIDDLE )
		{
			rb_tree<T>::rotate_right_middle(node->left);
			return SUCCESS;
		}
		if( res_add==ROTATE_RIGHT_NOT_MIDDLE )
		{
			rb_tree<T>::rotate_right_not_middle(node->left);
			return SUCCESS;
		}
		if( res_add==ROTATE_LEFT_MIDDLE )
		{
			rb_tree<T>::rotate_left_middle(node->left);
			return SUCCESS;
		}
		// res_add==ROTATE_LEFT_NOT_MIDDLE
		rb_tree<T>::rotate_left_not_middle(node->left);
		return SUCCESS;
	}

	// *new_value>=*node

	if( node->right==nullptr )
	{
		node->right = new rb_tree_node<T>(static_cast<T &&>(*new_value));
		if( node->right==nullptr )
			return ALLOC_ERROR;
		if( node->color==rb_tree_node<T>::red )
			return TWO_RED_RIGHT;
		return SUCCESS;
	}

	res_add = rb_tree<T>::add_value_node(node->right, new_value);

	if( res_add==ALLOC_ERROR )
		return ALLOC_ERROR;
	if( res_add==SUCCESS )
		return SUCCESS;
	if( res_add==UNKNOWN )
	{// node->right->color==rb_tree_node<T>::red
		if( node->color==rb_tree_node<T>::red )
			return TWO_RED_RIGHT;
		return SUCCESS;
	}

	if( res_add==TWO_RED_LEFT || res_add==TWO_RED_RIGHT )
	{
		if( node->left==nullptr || node->left->color==rb_tree_node<T>::black )
		{
			if( res_add==TWO_RED_RIGHT )
				return ROTATE_LEFT_NOT_MIDDLE;
			return ROTATE_LEFT_MIDDLE;
		}
		// node->left->color==rb_tree_node<T>::red
		node->left->color = rb_tree_node<T>::black;
		node->right->color = rb_tree_node<T>::black;
		node->color = rb_tree_node<T>::red;
		return UNKNOWN;
	}

	if( res_add==ROTATE_RIGHT_MIDDLE )
	{
		rb_tree<T>::rotate_right_middle(node->right);
		return SUCCESS;
	}
	if( res_add==ROTATE_RIGHT_NOT_MIDDLE )
	{
		rb_tree<T>::rotate_right_not_middle(node->right);
		return SUCCESS;
	}
	if( res_add==ROTATE_LEFT_MIDDLE )
	{
		rb_tree<T>::rotate_left_middle(node->right);
		return SUCCESS;
	}
	// res_add==ROTATE_LEFT_NOT_MIDDLE
	rb_tree<T>::rotate_left_not_middle(node->right);
	return SUCCESS;
}

#endif// ADD_VALUE_H
