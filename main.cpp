/* Code written by Kliment Serafimov */

#include <fstream>
#include <iomanip>
#include <iostream>

#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <stack>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#define P push
#define f first
#define s second
#define pb push_back
#define mp make_pair
#define DEC 0.00000001
#define MthisX 2139062143
#define MthisX_63  1061109567
#define MthisXll 9187201950435737471
#define bp(a) __builtin_popcount(a)
#define rand(a, b) ((rand()%(b-a+1))+a)
#define MEM(a, b) memset(a, b, sizeof(a))
#define sort_v(a) sort(a.begin(), a.end())
#define rev_v(a)  reverse(a.begin(), a.end())

//#define fin cin
//#define fout cout
using namespace std;
//ifstream fin(".in");
//ofstream fout(".out");

//K = number of predecessor data strucutres
const int K = 20;

class node
{
public:
	node* parent;
	node* left_child;
	node* right_child;
	node* predecessor[K];
	node* successor[K];
	int batch_bitmask;
	int mins[K], maxs[K];
    int low, high;
    node(int _low, int _high, node* _parent)
    {
        low = _low;
        high = _high;
        batch_bitmask = (1<<K)-1;
        parent = _parent;
        MEM(mins, 63);
        MEM(maxs, 0);
    }
    bool set_exists(int t_i)
    {
        batch_bitmask&=(~(1<<t_i));
    }
	bool exists(int t_i)
	{
		return ((batch_bitmask&(1<<t_i)) == 0);
	}
	node* other_child(node* child)
	{
		if(child == left_child)
		{
			return right_child;
		}
		else
		{
			return left_child;
		}
	}
	void insert(int val, int t_i)
	{
	    cout << val <<" "<<low <<" "<< high <<endl;
	    if(!exists(t_i))
        {
            set_exists(t_i);
            update_pointers(t_i);
        }
        mins[t_i] = min(mins[t_i], val);
        maxs[t_i] = max(maxs[t_i], val);
        if(low == high)
        {
            return;
        }
        int mid = (low+high)/2;
        if(val<=mid)
        {
            if(left_child == NULL)
            {
                left_child = new node(low, mid, this);
            }
            left_child -> insert(val, t_i);
        }
        else
        {
            if(right_child == NULL)
            {
                right_child = new node(mid+1, high, this);
            }
            right_child -> insert(val, t_i);
        }
	}
	int get_most_significant_bit(int mask)
	{
	    ///can be computed in O(1)
	    cout << mask <<endl;
	    int ret = -1;
	    for(int i = 0;i<K;i++)
        {
            if((mask&(1<<i))!=0)
            {
                ret = i;
            }
        }
        return ret;
	}
	void query(int val, int to_be_queried, int predecessor_result[], int successor_result[])
	{
	    cout << val <<" "<< low << " "<< high <<endl;
	    int mask = to_be_queried & batch_bitmask;///most important line in the code
	    cout << "BB : "<< mask <<" "<<batch_bitmask <<" "<< (1<<K)-1<<endl;
	    ///mask has 1s at locations that don't have a node.
	    int most_significant_bit = get_most_significant_bit(mask);
	    while(most_significant_bit != -1)
        {
            node* predecessor = get_level_predecessor(most_significant_bit);
            node* successor = get_level_successor(most_significant_bit);
            if(predecessor!= NULL)
            {
                predecessor_result[most_significant_bit] = predecessor->maxs[most_significant_bit];
            }
            else
            {
                predecessor_result[most_significant_bit] = -1;
            }
            if(successor != NULL)
            {
                successor_result[most_significant_bit] = successor->mins[most_significant_bit];
            }
            else
            {
                successor_result[most_significant_bit] = -1;
            }
            cout << "here: "<< most_significant_bit <<endl;
            mask -= (1<<most_significant_bit);
            assert(mask >=0);
            to_be_queried -= (1<<most_significant_bit);
            most_significant_bit = get_most_significant_bit(mask);
            cout << "again " <<endl;
        }
        if(low == high || to_be_queried == 0)
        {
            return;
        }
        int mid = (low+high)/2;
        if(val<=mid)
        {
            if(left_child == NULL)
            {
                left_child = new node(low, mid, this);
            }
            left_child -> query(val, to_be_queried, predecessor_result, successor_result);
        }
        else
        {
            if(right_child == NULL)
            {
                right_child = new node(mid+1, high, this);
            }
            right_child -> query(val, to_be_queried, predecessor_result, successor_result);
        }
	}
	node *get_level_predecessor(int t_i)
	{
	    if(parent != NULL)
        {
            if(parent->other_child(this) != NULL && parent->other_child(this)->exists(t_i))
            {
                if(parent->other_child(this) == parent->left_child)
                {
                    return parent->left_child;
                }
                else
                {
                    return parent->right_child->predecessor[t_i];
                }
            }
            else
            {
                if(parent->predecessor[t_i]->right_child!=NULL)
                {
                    return parent->predecessor[t_i]->right_child;
                }
                else
                {
                    assert(parent->predecessor[t_i]->left_child!=NULL);
                    return parent->predecessor[t_i]->left_child;
                }
            }
        }
	}
	node *get_level_successor(int t_i)
	{
        if(parent != NULL)
        {
            if(parent->other_child(this) != NULL && parent->other_child(this)->exists(t_i))
            {
                if(parent->other_child(this) == parent->left_child)
                {
                    return parent->left_child->successor[t_i];
                }
                else
                {
                    assert(parent->other_child(this) == parent->right_child);
                    return parent->right_child;
                }
            }
            else
            {
                if(parent->successor[t_i]->left_child!=NULL)
                {
                    return parent->successor[t_i]->left_child;
                }
                else
                {
                    assert(parent->successor[t_i]->right_child!=NULL);
                    return parent->successor[t_i]->right_child;
                }
            }
        }
	}
	void update_pointers(int t_i)
    {
        ///note that there is an invariant that at least one child of every node exists.
        ///base case, when one of the predecessor or successor of this is the other child of the parent of this
        if(parent != NULL)
        {
            if(parent->other_child(this) != NULL && parent->other_child(this)->exists(t_i))
            {
                if(parent->other_child(this) == parent->left_child)
                {
                    predecessor[t_i] = parent->left_child;
                    successor[t_i] = parent->left_child->successor[t_i];
                    if(parent->left_child->successor[t_i] != NULL)
                    {
                        parent->left_child->successor[t_i]->predecessor[t_i] = this;
                    }
                    parent->left_child->successor[t_i] = this;
                }
                else
                {
                    ///symetric as above
                    successor[t_i] = parent->right_child;
                    predecessor[t_i] = parent->right_child->predecessor[t_i];
                    if(parent->right_child->successor[t_i] != NULL)
                    {
                        parent->right_child->predecessor[t_i]->successor[t_i] = this;
                    }
                    parent->right_child->predecessor[t_i] = this;
                }
            }
            ///general case case, when we have to find the predecessor/successor by querying the predecessor/successor of the parents
            else
            {
                if(parent->predecessor[t_i] != NULL)
                {
                    ///check if the predecessor is the right child of the predecessor of the parent
                    if(parent->predecessor[t_i]->right_child != NULL && parent->predecessor[t_i]->right_child->exists(t_i))
                    {
                        predecessor[t_i] = parent->predecessor[t_i]->right_child;
                        if(parent->predecessor[t_i]->right_child->successor[t_i]!=NULL)
                        {
                            successor[t_i] = parent->predecessor[t_i]->right_child->successor[t_i];
                            parent->predecessor[t_i]->right_child->successor[t_i]->predecessor[t_i] = this;
                        }
                        parent->predecessor[t_i]->right_child->successor[t_i] = this;
                    }
                    ///the predecessor is the left child of the predecessor of the parent
                    else
                    {
                        assert(parent->predecessor[t_i]->left_child != NULL);
                        assert(parent->predecessor[t_i]->left_child->exists(t_i));
                        predecessor[t_i] = parent->predecessor[t_i]->left_child;
                        if(parent->predecessor[t_i]->left_child->successor[t_i] != NULL)
                        {
                            successor[t_i] = parent->predecessor[t_i]->left_child->successor[t_i];
                            parent->predecessor[t_i]->left_child->successor[t_i]->predecessor[t_i] = this;
                        }
                        parent->predecessor[t_i]->left_child->successor[t_i] = this;
                    }
                }
                else if(parent->successor[t_i] != NULL)
                {
                    ///check if the successor is the left child of the successor of the parent
                    if(parent->successor[t_i]->left_child->exists(t_i))
                    {
                        successor[t_i] = parent->successor[t_i]->left_child;
                        if(parent->successor[t_i]->left_child->predecessor[t_i]!=NULL)
                        {
                            predecessor[t_i] = parent->successor[t_i]->left_child->predecessor[t_i];
                            parent->successor[t_i]->left_child->predecessor[t_i]->successor[t_i] = this;
                        }
                        parent->successor[t_i]->left_child->predecessor[t_i] = this;
                    }
                    ///the predecessor is the right child of the predecessor of the parent
                    else
                    {
                        assert(parent->successor[t_i]->right_child->exists(t_i));
                        successor[t_i] = parent->successor[t_i]->right_child;

                        if(parent->successor[t_i]->right_child->predecessor[t_i] != NULL)
                        {
                            predecessor[t_i] = parent->successor[t_i]->right_child->predecessor[t_i];
                            parent->successor[t_i]->right_child->predecessor[t_i]->successor[t_i] = this;
                        }
                        parent->successor[t_i]->right_child->predecessor[t_i] = this;
                    }
                }
                else
                {
                    ///no predecessor and successor, thus no update needed.
                }
            }
        }
    }

};

int main()
{
    int n;
    cin >> n;
    assert(log2(n) < K);
    node* root = new node(0, n-1, NULL);
    int num_structures = log2(n)+1;
    while(1)
    {
        int type, to_be_accessed, val;
        cin >> type >> to_be_accessed >> val;
        if(type == 0)
        {
            for(int i = 0;i < num_structures; i++)
            {
                if((to_be_accessed&(1<<i))!=0)
                {
                    root->insert(val, i);
                }
            }
        }
        else if(type == 1)
        {
            ///batch query
            ///t_i has a 1 at locations that you want to query
            int predecessor[K];
            int successor[K];
            root->query(val, to_be_accessed, predecessor, successor);
            for(int i = 0;i < num_structures; i++)
            {
                if((to_be_accessed&(1<<i))!=0)
                {
                    cout << i <<" :: "<< predecessor[i] <<" " << successor[i] <<endl;
                }
            }
        }
    }
    return 0;
}
