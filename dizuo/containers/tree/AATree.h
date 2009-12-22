
#ifndef _DUOLUOXIANZHI_AATREE_H_
#define _DUOLUOXIANZHI_AATREE_H_

#include "../fixpool.h"

_DUOLUO_DLO_BEGIN_

template <class NdTts>
class AAnode   {
public :
	AAnode ()
	{
		left = right = NULL ;
		level = 0 ;
	}

	typedef typename NdTts::value aanode_value ;
	typedef typename NdTts::key   aanode_key ;

	aanode_value& getValue ()
	{ return node_trait.getValue () ; }

	const aanode_value& getValue () const
	{ return node_trait.getValue () ; }

	aanode_key&   getKey ()
	{ return node_trait.getKey () ; }

	const aanode_key&   getKey () const
	{ return node_trait.getKey () ; }

	NdTts   node_trait ;
	int		level ;

	AAnode* left ;
	AAnode* right ;
	AAnode* father ;
	bool	isOnRight ;

	AAnode* next ;
	AAnode* prev ;
} ;

template <class Traits , 
		  class Alloc = FixPool< AAnode<Traits> > >
class AATree   {
public :
	typedef		     AAnode<Traits>*      node ;
	typedef typename AAnode<Traits>::aanode_value tree_value ;
	typedef typename AAnode<Traits>::aanode_key   tree_key ;

public :
	AATree ()
	{ initialize () ; }

	virtual ~AATree ()
	{}

	void insert ( const Traits& t )
	{
		if ( head != nullNode )   {
			node p = getHead () ;
			while ( p != nullNode )   {
				if ( p -> getKey() < t.getKey() )   {
					if ( p -> right != nullNode )   {
						p = p -> right ;
					}
					else   {
						initNode ( &( p -> right ) , t ) ;
						p -> right -> father = p ;
						p -> right -> isOnRight = true ;
						addThreadNode ( p -> right ) ;
						++cnt ;
						break ;
					}
				}
				else if ( t.getKey() < p -> getKey() )   {
					if ( p -> left != nullNode )   {
						p = p -> left ;
					}
					else   {
						initNode ( &( p -> left ) , t ) ;
						p -> left  -> father = p ;
						p -> right -> isOnRight = false ;
						addThreadNode ( p -> left ) ;
						++cnt ;
						break ;
					}
				}
				else
					return ;
			}

			up_percolate ( p ) ;
		}
		else   {
			initNode ( &head , t ) ;
			addThreadNode ( head ) ;
			++cnt ;
		}
	}

	node findNode ( const tree_key& k ) const
	{
		node p = getHead () ;

		while ( p != nullNode )   {
			if ( p -> getKey() < k )
				p = p -> right ;
			else if ( k < p -> getKey() )
				p = p -> left ;
			else
				return p ;
		}

		return p ;
	}

	void earse ( const tree_key& k )
	{
		node p = findNode ( k ) ;
		if ( p != nullNode )
			earse ( p ) ;
	}

	void earse ( node p )
	{
		node d_ptr = findMaxNode ( p ) ;
		cp_link ( p , d_ptr ) ;

		p = d_ptr ;
		head = d_ptr = d_ptr -> father ;
		
		if ( p -> isOnRight )
			d_ptr -> right = nullNode ;
		else
			d_ptr -> left  = nullNode ;

		allocator.free_obj ( p ) ;
		rmvThreadNode ( p ) ;
		--cnt ;

		while ( d_ptr != nullNode )   {
			if ( d_ptr -> left ->  level < d_ptr -> level - 1 ||
				 d_ptr -> right -> level < d_ptr -> level - 1 )   {
			    if ( d_ptr -> right -> level > --( d_ptr -> level ) )
					d_ptr -> right -> level = d_ptr -> level ;

				d_ptr = skew ( d_ptr ) ;
				d_ptr -> right = skew ( d_ptr -> right ) ;
				d_ptr -> right -> right = skew ( d_ptr -> right -> right ) ;
				d_ptr = split ( d_ptr ) ;
				d_ptr -> right = split ( d_ptr -> right ) ;
			}

			head = d_ptr ;
			d_ptr = d_ptr -> father ;
		}
	}

	const size_t size () const
	{ return cnt ; }

	const bool empty () const
	{ return cnt == 0 ; }

	void clear ()
	{
		node thread = threadHead ;

		while ( thread )   {
			node tmp = thread ;
			thread = thread -> next ;
			allcator.free_obj ( tmp ) ;
		}

		initialize () ;
	}

	node getThreadHead () const
	{ return threadHead ; }

	node getThreadTail () const
	{ return threadTail ; }

protected :
	Alloc allocator ;
	node  head ;
	node  nullNode ;
	node  threadHead ;
	node  threadTail ;
	size_t cnt ;

	node getHead () const
	{ return head ; }

	node findMaxNode ( node p ) const
	{
		while ( p != nullNode )   {
			if ( p -> right != nullNode )
				p = p -> right ;
			else
				break ;
		}

		return p ;
	}

	void swap_content ( node lhs , node rhs )
	{
		Trait tmp = rhs -> node_trait ;
		rhs -> node_trait = lhs -> node_trait ;
		lhs -> node_trait = tmp ;
	}

	void cp_content ( node lhs , node rhs )
	{ lhs -> node_trait = rhs -> node_trait ; }

	void cp_link ( node& lhs , node& rhs )
	{
		node left      = rhs -> left ;
		node right     = rhs -> right ;
		node father    = rhs -> father ;
		node next      = rhs -> next ;
		node prev      = rhs -> prev ;

		bool isOnRight = rhs -> isOnRight ;
		int  level     = rhs -> level ;

		lhs = rhs ;
		lhs -> left      = left ;
		lhs -> right     = right ;
		lhs -> father    = father ;
		lhs -> next      = next ;
		lhs -> prev      = prev ;
		lhs -> isOnRight = isOnRight ;
		lhs -> level     = level ;
	}

private :
	AATree ( const AATree& rhs ) ;
	AATree& operator = ( const AATree& rhs ) ;

	void initNode ( node* p , const Traits& t )
	{
		( *p )          = allocator.alloc_obj () ;
		( *p ) -> node_trait = t ;

		( *p ) -> left   = nullNode ;
		( *p ) -> right  = nullNode ;
		( *p ) -> father = nullNode ;
		( *p ) -> level  = 1 ;

		( *p ) -> next   = NULL ;
		( *p ) -> prev   = NULL ;
	}

	void initialize ()
	{
		nullNode           = allocator.alloc_obj () ;
		nullNode -> left   = nullNode ;
		nullNode -> right  = nullNode ;
		nullNode -> father = nullNode ;
		nullNode -> level  = 0 ;
		nullNode -> next   = NULL ;
		nullNode -> prev   = NULL ;

		head = nullNode ;
		cnt  = 0 ;

		threadHead         = allocator.alloc_obj () ;
		threadTail         = nullNode ;
		threadHead -> next = threadTail ;
		threadHead -> prev = NULL ;
		threadTail -> next = NULL ;
		threadTail -> prev = threadHead ;
	}

	void up_percolate ( node p )
	{
		while ( p != nullNode )   {
			p = skew  ( p ) ;
			p = split ( p ) ;

			head = p ;
			p = p -> father ;
		}
	}


	node skew ( node p )
	{
		if ( p -> left -> level == p -> level )   {
			node tmp      = p -> left ;
			tmp -> isOnRight = p -> isOnRight ;
			p -> left     = tmp -> right ;
			p -> left -> isOnRight = false ;
			if ( p -> left != nullNode )
				p -> left -> father = p ;
			tmp -> right  = p ;
			tmp -> father = p -> father ;
			if ( p -> father != nullNode )   {
				if ( p -> isOnRight )
					p -> father -> right = tmp ;
				else
					p -> father -> left  = tmp ;
			}

			p -> isOnRight = true ;
			p -> father = tmp ;
			p = tmp ;
		}

		return p ;
	}

	node split ( node p )
	{
		if ( p -> right -> right -> level == p -> level )   {
			node tmp      = p -> right ;
			tmp -> isOnRight = p -> isOnRight ;
			p -> right    = tmp -> left ;
			p -> right -> isOnRight = true ;
			if ( p -> right != nullNode )
				p -> right -> father = p ;
			tmp -> left   = p ;
			tmp -> father = p -> father ;
			if ( p -> father != nullNode )   {
				if ( p -> isOnRight )
					p -> father -> right = tmp ;
				else
					p -> father -> left  = tmp ;
			}

			p -> isOnRight = false ;
			p -> father   = tmp ;
			p             = tmp ;

			++( p -> level ) ;
		}

		return p ;
	}

	void addThreadNode ( node p )
	{
		p -> next = threadHead -> next ;
		p -> prev = threadHead ;
		threadHead -> next -> prev = p ;
		threadHead -> next = p ;
	}

	void rmvThreadNode ( node p )
	{
		p -> next -> prev = p -> prev ;
		p -> prev -> next = p -> next ;
	}
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_AATREE_H_ */

