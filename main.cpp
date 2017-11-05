//K = number of predecessor data strucutres

class node
{
	node* parent;
	node* left_child, right_child;
	node* predecessor[K];
	node* successor[K];
	int batch_bitmask;
	int mins[K], maxs[K];

	bool exists(int t_i)
	{
		return ((batch_bitmask&(1<<t_i)) == 0);
	}
	node* other_child(node* A)
	{
		if(A == left_child)
		{
			return right_child;
		}
		else
		{
			return left_child;
		}
	}
};

void update_pointers(node* A, int t_i)
{
	///note that there is an invariant that at least one child of every node exists.

	///base case, when one of the predecessor or successor of A is the other child of the parent of A
	if(A->parent->other_child(A)->exists(t_i))
	{
		if(A->parent->other_child(A) == parent->left_child)
		{
			A->predecessor[t_i] = parent->left_child;
			if(parent->left_child->successor[t_i] != NULL)
            {
				parent->left_child->successor[t_i]->predecessor[t_i] = A
			}
			parent->left_child->successor[t_i] = A
		}
		else
		{
			///symetric as above
			A->successor[t_i] = parent->right_child;
			if(parent->right_child->successor[t_i] != NULL)
			{
				parent->right_child->predecessor[t_i]->successor[t_i] = A
			}
			parent->right_child->predecessor[t_i] = A
		}
	}
	///general case case, when we have to find the predecessor/successor by querying the predecessor/successor of the parents
	else
	{
		if(A->parent->predecessor[t_i] != NULL)
		{
			///check if the predecessor is the right child of the predecessor of the parent
			if(A->parent->predecessor[t_i]->right_child->exists(t_i))
			{
				A->predecessor[t_i] = A->parent->predecessor[t_i]->right_child;
				if(A->parent->predecessor[t_i]->right_child->successor[t_i]!=NULL)
				{
					A->parent->predecessor[t_i]->right_child->successor[t_i]->predecessor[t_i] = A
				}
				A->parent->predecessor[t_i]->right_child->successor[t_i] = A;
			}
			///the predecessor is the left child of the predecessor of the parent
			else
			{
				A->predecessor[t_i] = A->parent->predecessor[t_i]->left_child;
				if(A->parent->predecessor[t_i]->left_child->successor[t_i] != NULL)
				{
					A->parent->predecessor[t_i]->left_child->successor[t_i]->predecessor[t_i] = A
				}
				A->parent->predecessor[t_i]->left_child->successor[t_i] = A;
			}
		}
		else if(A->parent->successor[t_i] != NULL)
		{
			///symetric as above
			///...
		}
		else
		{
			///no predecessor and successor, thus no update needed.
		}
	}
}
