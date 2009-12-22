
#ifndef _DUOLUOXIANZHI_BINARYTREE_H_
#define _DUOLUOXIANZHI_BINARYTREE_H_

#include "../fixpool.h"

_DUOLUO_DLO_BEGIN_

template <class T , class K>
class BinaryTreeNode   {
public :
	BinaryTreeNode ()
	{
		father = left = right = NULL ; 
		isOnRight = true ;
	}

	T               data ;
	K               key ;

	bool			isOnRight ;
	BinaryTreeNode* father ;
	BinaryTreeNode* left ;
	BinaryTreeNode* right ;
} ;

template <class Ty, class KEY>
class BinaryTree   {
public :
	BinaryTree ()
	{ head = NULL ; }

	virtual ~BinaryTree ()
	{}

	void insert ( const Ty& data , const KEY& key )
	{
		if ( head )   {
			BinaryTreeNode<Ty , KEY>* p = getHead () ;

			while ( p )   {
				if ( p -> key < key )   {
					if ( p -> right )   {
						p = p -> right ;
					}
					else   {
						initNode ( &( p -> right ) , data , key ) ;
						p -> right -> father = p ;
						p -> isOnRight = true ;
						break ;
					}
				}
				else if ( p -> key > key )   {
					if ( p -> left )   {
						p = p -> left ;
					}
					else   {
						initNode ( &( p -> left ) , data , key ) ;
						p -> left -> father = p ;
						p -> isOnRight = false ;
						break ;
					}
				}
				else
					return ;
			}
		}
		else   {
			BinaryTreeNode<Ty , KEY>* first ;
			initNode ( &first , data , key ) ;
			initNode ( &head , data , key ) ;
			first -> father = head ;
			head -> right   = first ;
		}
	}

	void erase ( const KEY& key )
	{
		BinaryTreeNode<Ty , KEY>* p = findNode ( key ) ;
		if ( p )   {
			while ( p -> right && p -> left )   {
				BinaryTreeNode<Ty , KEY>* rightMin = findMinNode ( p -> right ) ;
				swap ( p , rightMin ) ;
				p = rightMin ;
			}

			skip ( p ) ;
			pool.free_obj ( p ) ;
		}
	}

	Ty* find ( const KEY& key ) const
	{
		BinaryTreeNode<Ty , KEY>* p = findNode ( key ) ;
		if ( p )
			return &( p -> data ) ;

		return NULL ;
	}

protected :
	BinaryTreeNode<Ty , KEY>* getHead () const
	{ return head -> right ; }

	BinaryTreeNode<Ty , KEY>* findNode ( const KEY& key ) const
	{
		BinaryTreeNode<Ty , KEY>* p = getHead () ;

		while ( p )   {
			if ( p -> key < key )
				p = p -> right ;
			else if ( p -> key > key )
				p = p -> left ;
			else
				return p ;
		}

		return p ;
	}

	BinaryTreeNode<Ty , KEY>* findMaxNode ( BinaryTreeNode<Ty , KEY>* node ) const
	{
		while ( node )   {
			if ( node -> right )
				node = node -> right ;
			else
				break ;
		}

		return node ;
	}

	BinaryTreeNode<Ty , KEY>* findMinNode ( BinaryTreeNode<Ty , KEY>* node ) const
	{
		while ( node )   {
			if ( node -> left )
				node = node -> left ;
			else
				break ;
		}

		return node ;
	}

	void skip ( BinaryTreeNode<Ty , KEY>* node )
	{
		if ( node -> right )   {
			if ( node -> isOnRight )
				node -> father -> right = node -> right ;
			else
				node -> father -> left  = node -> right ;

			node -> right -> father = node -> father ;
		}
		else   {
			if ( node -> isOnRight )
				node -> father -> right = node -> left ;
			else
				node -> father -> left  = node -> left ;

			if ( node -> left )
				node -> left -> father = node -> father ;
		}
	}

	void swap ( BinaryTreeNode<Ty , KEY>* lhs , BinaryTreeNode<Ty , KEY>* rhs )
	{
		BinaryTreeNode<Ty , KEY>* tmpFather = lhs -> father ;
		BinaryTreeNode<Ty , KEY>* tmpLeft   = lhs -> left ;
		BinaryTreeNode<Ty , KEY>* tmpRight  = lhs -> right ;
		bool                      isOnRight = lhs -> isOnRight ;

		lhs -> father    = rhs -> father ;
		lhs -> left      = rhs -> left ;
		lhs -> right     = rhs -> right ;
		lhs -> isOnRight = rhs -> isOnRight ;

		rhs -> father    = tmpFather ;
		rhs -> left      = tmpLeft ;
		rhs -> right     = tmpRight ;
		rhs -> isOnRight = isOnRight ;
	}

private :
	BinaryTree ( const BinaryTree& rhs ) ;
	BinaryTree& operator = ( const BinaryTree& rhs ) ;

	FixPool< BinaryTreeNode<Ty , KEY> >	pool ;
	BinaryTreeNode<Ty , KEY>*		    head ;

	void initNode ( BinaryTreeNode<Ty , KEY>** p , const Ty& data , const KEY& key )
	{
		( *p )          = :: new ( pool.alloc_obj () ) BinaryTreeNode<Ty , KEY>() ;
		( *p ) -> data  = data ;
		( *p ) -> key   = key ;
	}
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_BINARYTREE_H_ */
