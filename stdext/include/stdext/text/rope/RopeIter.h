/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/text/rope/RopeIter.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: RopeIter.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TEXT_ROPE_ROPEITER_H__
#define __STDEXT_TEXT_ROPE_ROPEITER_H__

__NS_STD_BEGIN

// Rope iterators:
// Unlike in the C version, we cache only part of the stack
// for Rope iterators, since they must be efficiently copyable.
// When we run out of cache, we have to reconstruct the iterator
// value.
// Pointers from iterators are not included in reference counts.
// Iterators are assumed to be thread private.  Ropes can
// be shared.

template <class _CharT>
class _Rope_iterator_base
  : public random_access_iterator<_CharT, ptrdiff_t> {
  public:
    typedef _Rope_RopeRep<_CharT> _RopeRep;
        // Borland doesnt want this to be protected.
  protected:
    enum { _S_path_cache_len = 4 }; // Must be <= 9.
    enum { _S_iterator_buf_len = 15 };
    size_t _M_current_pos;
    _RopeRep* _M_root;     // The whole Rope.
    size_t _M_leaf_pos;    // Starting position for current leaf
    const _CharT* _M_buf_start;
                        // Buffer possibly
                        // containing current char.
    const _CharT* _M_buf_ptr;
                        // Pointer to current char in buffer.
                        // != 0 ==> buffer valid.
    const _CharT* _M_buf_end;
                        // One past __last valid char in buffer.
    // What follows is the path cache.  We go out of our
    // way to make this compact.
    // Path_end contains the bottom section of the path from
    // the root to the current leaf.
    const _RopeRep* _M_path_end[_S_path_cache_len];
    int _M_leaf_index;     // Last valid __pos in path_end;
                        // _M_path_end[0] ... _M_path_end[leaf_index-1]
                        // point to concatenation nodes.
    unsigned char _M_path_directions;
                          // (path_directions >> __i) & 1 is 1
                          // iff we got from _M_path_end[leaf_index - __i - 1]
                          // to _M_path_end[leaf_index - __i] by going to the
                          // __right. Assumes path_cache_len <= 9.
    _CharT _M_tmp_buf[_S_iterator_buf_len];
                        // Short buffer for surrounding chars.
                        // This is useful primarily for 
                        // RopeFunctions.  We put the buffer
                        // here to avoid locking in the
                        // multithreaded case.
    // The cached path is generally assumed to be valid
    // only if the buffer is valid.
    static void _S_setbuf(_Rope_iterator_base& __x);
                                        // Set buffer contents given
                                        // path cache.
    static void _S_setcache(_Rope_iterator_base& __x);
                                        // Set buffer contents and
                                        // path cache.
    static void _S_setcache_for_incr(_Rope_iterator_base& __x);
                                        // As above, but assumes path
                                        // cache is valid for previous posn.
    _Rope_iterator_base() {}
    _Rope_iterator_base(_RopeRep* __root, size_t __pos)
      : _M_current_pos(__pos), _M_root(__root), _M_buf_ptr(0) {}
    void _M_incr(size_t __n);
    void _M_decr(size_t __n);
  public:
    size_t index() const { return _M_current_pos; }
    _Rope_iterator_base(const _Rope_iterator_base& __x) {
        if (0 != __x._M_buf_ptr) {
            *this = __x;
        } else {
            _M_current_pos = __x._M_current_pos;
            _M_root = __x._M_root;
            _M_buf_ptr = 0;
        }
    }
};

template<class _CharT>
class _Rope_const_iterator : public _Rope_iterator_base<_CharT> {
    typedef _Rope_iterator_base<_CharT> _Base;
  protected:
    using _Base::_M_current_pos;
    using _Base::_M_root;
    using _Base::_M_buf_ptr;
    using _Base::_M_buf_end;
    using _Base::_M_incr;
    using _Base::_M_decr;
  protected:
    typedef _Rope_RopeRep<_CharT> _RopeRep;
  public:
    typedef _CharT reference;   // Really a value.  Returning a reference
                                // Would be a mess, since it would have
                                // to be included in refcount.
    typedef const _CharT* pointer;

  public:
    _Rope_const_iterator() {};
    _Rope_const_iterator(const _Rope_const_iterator& __x) : _Base(__x) { }
    _Rope_const_iterator(const _RopeRep* __root, size_t __pos):
		_Base(const_cast<_RopeRep*>(__root), __pos) {}
    _Rope_const_iterator(_RopeRep* __r, size_t __pos) : _Base(__r, __pos) {}
    _Rope_const_iterator& operator= (const _Rope_const_iterator& __x) {
        if (0 != __x._M_buf_ptr) {
            *(static_cast<_Rope_iterator_base<_CharT>*>(this)) = __x;
        } else {
            _M_current_pos = __x._M_current_pos;
            _M_root = __x._M_root;
            _M_buf_ptr = 0;
        }
        return(*this);
    }
    reference operator*() {
        if (0 == _M_buf_ptr) _S_setcache(*this);
        return *_M_buf_ptr;
    }
    _Rope_const_iterator& operator++() {
        const _CharT* __next;
        if (0 != _M_buf_ptr && (__next = _M_buf_ptr + 1) < _M_buf_end) {
            _M_buf_ptr = __next;
            ++_M_current_pos;
        } else {
            _M_incr(1);
        }
        return *this;
    }
    _Rope_const_iterator& operator+=(ptrdiff_t __n) {
        if (__n >= 0) {
            _M_incr(__n);
        } else {
            _M_decr(-__n);
        }
        return *this;
    }
    _Rope_const_iterator& operator--() {
        _M_decr(1);
        return *this;
    }
    _Rope_const_iterator& operator-=(ptrdiff_t __n) {
        if (__n >= 0) {
            _M_decr(__n);
        } else {
            _M_incr(-__n);
        }
        return *this;
    }
    _Rope_const_iterator operator++(int) {
        size_t __old_pos = _M_current_pos;
        _M_incr(1);
        return _Rope_const_iterator<_CharT>(_M_root, __old_pos);
        // This makes a subsequent dereference expensive.
        // Perhaps we should instead copy the iterator
        // if it has a valid cache?
    }
    _Rope_const_iterator operator--(int) {
        size_t __old_pos = _M_current_pos;
        _M_decr(1);
        return _Rope_const_iterator<_CharT>(_M_root, __old_pos);
    }
    reference operator[](size_t __n) {
        return Rope<_CharT,_Alloc>::_S_fetch(_M_root, _M_current_pos + __n);
    }
};

/*
template<class _CharT>
class _Rope_iterator : public _Rope_iterator_base<_CharT> {
    typedef _Rope_iterator_base<_CharT> _Base;
  protected:
    using _Base::_M_current_pos;
    using _Base::_M_root;
    using _Base::_M_buf_ptr;
    using _Base::_M_buf_end;
    using _Base::_M_incr;
    using _Base::_M_decr;
	typedef _Rope_RopeRep<_CharT> _RopeRep;
  protected:
    Rope<_CharT,_Alloc>* _M_root_rope;
        // root is treated as a cached version of this,
        // and is used to detect changes to the underlying
        // Rope.
        // Root is included in the reference count.
        // This is necessary so that we can detect changes reliably.
        // Unfortunately, it requires careful bookkeeping for the
        // nonGC case.
    _Rope_iterator(Rope<_CharT,_Alloc>* __r, size_t __pos)
      : _Rope_iterator_base<_CharT,_Alloc>(__r->_M_tree_ptr, __pos),
        _M_root_rope(__r) 
       { if (!(__r -> empty()))_S_setcache(*this); }

    void _M_check();
  public:
    typedef _Rope_char_ref_proxy<_CharT,_Alloc>  reference;
    typedef _Rope_char_ref_proxy<_CharT,_Alloc>* pointer;

  public:
    Rope<_CharT,_Alloc>& container() { return *_M_root_rope; }
    _Rope_iterator() {
        _M_root = 0;  // Needed for reference counting.
    };
    _Rope_iterator(const _Rope_iterator& __x) :
        _Rope_iterator_base<_CharT,_Alloc>(__x) {
        _M_root_rope = __x._M_root_rope;
    }
    _Rope_iterator(Rope<_CharT,_Alloc>& __r, size_t __pos);
    _Rope_iterator& operator= (const _Rope_iterator& __x) {
        _RopeRep* __old = _M_root;
        if (0 != __x._M_buf_ptr) {
            _M_root_rope = __x._M_root_rope;
            *(static_cast<_Rope_iterator_base<_CharT,_Alloc>*>(this)) = __x;
        } else {
            _M_current_pos = __x._M_current_pos;
            _M_root = __x._M_root;
            _M_root_rope = __x._M_root_rope;
            _M_buf_ptr = 0;
        }
        return(*this);
    }
    reference operator*() {
        _M_check();
        if (0 == _M_buf_ptr) {
            return _Rope_char_ref_proxy<_CharT,_Alloc>(
               _M_root_rope, _M_current_pos);
        } else {
            return _Rope_char_ref_proxy<_CharT,_Alloc>(
               _M_root_rope, _M_current_pos, *_M_buf_ptr);
        }
    }
    _Rope_iterator& operator++() {
        _M_incr(1);
        return *this;
    }
    _Rope_iterator& operator+=(ptrdiff_t __n) {
        if (__n >= 0) {
            _M_incr(__n);
        } else {
            _M_decr(-__n);
        }
        return *this;
    }
    _Rope_iterator& operator--() {
        _M_decr(1);
        return *this;
    }
    _Rope_iterator& operator-=(ptrdiff_t __n) {
        if (__n >= 0) {
            _M_decr(__n);
        } else {
            _M_incr(-__n);
        }
        return *this;
    }
    _Rope_iterator operator++(int) {
        size_t __old_pos = _M_current_pos;
        _M_incr(1);
        return _Rope_iterator<_CharT,_Alloc>(_M_root_rope, __old_pos);
    }
    _Rope_iterator operator--(int) {
        size_t __old_pos = _M_current_pos;
        _M_decr(1);
        return _Rope_iterator<_CharT,_Alloc>(_M_root_rope, __old_pos);
    }
    reference operator[](ptrdiff_t __n) {
        return _Rope_char_ref_proxy<_CharT,_Alloc>(
          _M_root_rope, _M_current_pos + __n);
    }

#if defined(__STL_MEMBER_TEMPLATES) && defined(__STL_FUNCTION_TMPL_PARTIAL_ORDER)
    template<class _CharT2, class _Alloc2>
    friend bool operator==
        (const _Rope_iterator<_CharT2,_Alloc2>& __x,
         const _Rope_iterator<_CharT2,_Alloc2>& __y);
    template<class _CharT2, class _Alloc2>
    friend bool operator<
        (const _Rope_iterator<_CharT2,_Alloc2>& __x,
         const _Rope_iterator<_CharT2,_Alloc2>& __y);
    template<class _CharT2, class _Alloc2>
    friend ptrdiff_t operator-
        (const _Rope_iterator<_CharT2,_Alloc2>& __x,
         const _Rope_iterator<_CharT2,_Alloc2>& __y);
    template<class _CharT2, class _Alloc2>
    friend _Rope_iterator<_CharT2,_Alloc2> operator-
        (const _Rope_iterator<_CharT2,_Alloc2>& __x,
         ptrdiff_t __n);
    template<class _CharT2, class _Alloc2>
    friend _Rope_iterator<_CharT2,_Alloc2> operator+
        (const _Rope_iterator<_CharT2,_Alloc2>& __x,
         ptrdiff_t __n);
    template<class _CharT2, class _Alloc2>
    friend _Rope_iterator<_CharT2,_Alloc2> operator+
        (ptrdiff_t __n,
         const _Rope_iterator<_CharT2,_Alloc2>& __x);
#else
    friend bool operator== __STL_NULL_TMPL_ARGS
        (const _Rope_iterator<_CharT,_Alloc>& __x,
         const _Rope_iterator<_CharT,_Alloc>& __y);
    friend bool operator< __STL_NULL_TMPL_ARGS
        (const _Rope_iterator<_CharT,_Alloc>& __x,
         const _Rope_iterator<_CharT,_Alloc>& __y);
    friend ptrdiff_t operator- __STL_NULL_TMPL_ARGS
        (const _Rope_iterator<_CharT,_Alloc>& __x,
         const _Rope_iterator<_CharT,_Alloc>& __y);
    friend _Rope_iterator<_CharT,_Alloc> operator- __STL_NULL_TMPL_ARGS
        (const _Rope_iterator<_CharT,_Alloc>& __x,
         ptrdiff_t __n);
    friend _Rope_iterator<_CharT,_Alloc> operator+ __STL_NULL_TMPL_ARGS
        (const _Rope_iterator<_CharT,_Alloc>& __x,
         ptrdiff_t __n);
    friend _Rope_iterator<_CharT,_Alloc> operator+ __STL_NULL_TMPL_ARGS
        (ptrdiff_t __n,
         const _Rope_iterator<_CharT,_Alloc>& __x);
#endif
};

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1375
#endif

// Set buf_start, buf_end, and buf_ptr appropriately, filling tmp_buf
// if necessary.  Assumes _M_path_end[leaf_index] and leaf_pos are correct.
// Results in a valid buf_ptr if the iterator can be legitimately
// dereferenced.
template <class _CharT, class _Alloc>
void _Rope_iterator_base<_CharT,_Alloc>::_S_setbuf( 
  _Rope_iterator_base<_CharT,_Alloc>& __x)
{
    const _RopeRep* __leaf = __x._M_path_end[__x._M_leaf_index];
    size_t __leaf_pos = __x._M_leaf_pos;
    size_t __pos = __x._M_current_pos;

    switch(__leaf->_M_tag) {
	case _RopeRep::_S_leaf:
	    __x._M_buf_start = 
	      ((_Rope_RopeLeaf<_CharT,_Alloc>*)__leaf)->_M_data;
	    __x._M_buf_ptr = __x._M_buf_start + (__pos - __leaf_pos);
	    __x._M_buf_end = __x._M_buf_start + __leaf->_M_size;
	    break;
	case _RopeRep::_S_function:
	case _RopeRep::_S_substringfn:
	    {
		size_t __len = _S_iterator_buf_len;
		size_t __buf_start_pos = __leaf_pos;
		size_t __leaf_end = __leaf_pos + __leaf->_M_size;
		CharProducer<_CharT>* __fn =
			((_Rope_RopeFunction<_CharT>*)__leaf)->_M_fn;

		if (__buf_start_pos + __len <= __pos) {
		    __buf_start_pos = __pos - __len/4;
		    if (__buf_start_pos + __len > __leaf_end) {
			__buf_start_pos = __leaf_end - __len;
		    }
		}
		if (__buf_start_pos + __len > __leaf_end) {
		    __len = __leaf_end - __buf_start_pos;
		}
		(*__fn)(__buf_start_pos - __leaf_pos, __len, __x._M_tmp_buf);
		__x._M_buf_ptr = __x._M_tmp_buf + (__pos - __buf_start_pos);
		__x._M_buf_start = __x._M_tmp_buf;
		__x._M_buf_end = __x._M_tmp_buf + __len;
	    }
	    break;
	default:
	    __stl_assert(0);
    }
}

// Set path and buffer inside a Rope iterator.  We assume that 
// pos and root are already set.
template <class _CharT, class _Alloc>
void _Rope_iterator_base<_CharT,_Alloc>::_S_setcache
(_Rope_iterator_base<_CharT,_Alloc>& __x)
{
    const _RopeRep* __path[_RopeRep::_S_max_rope_depth+1];
    const _RopeRep* __curr_rope;
    int __curr_depth = -1;  // index into path
    size_t __curr_start_pos = 0;
    size_t __pos = __x._M_current_pos;
    unsigned char __dirns = 0; // Bit vector marking right turns in the path

    __stl_assert(__pos <= __x._M_root->_M_size);
    if (__pos >= __x._M_root->_M_size) {
	__x._M_buf_ptr = 0;
	return;
    }
    __curr_rope = __x._M_root;
    for(;;) {
	++__curr_depth;
	__stl_assert(__curr_depth <= _RopeRep::_S_max_rope_depth);
	__path[__curr_depth] = __curr_rope;
	switch(__curr_rope->_M_tag) {
	  case _RopeRep::_S_leaf:
	  case _RopeRep::_S_function:
	  case _RopeRep::_S_substringfn:
	    __x._M_leaf_pos = __curr_start_pos;
	    goto done;
	  case _RopeRep::_S_concat:
	    {
		_Rope_RopeConcatenation<_CharT,_Alloc>* __c =
			(_Rope_RopeConcatenation<_CharT,_Alloc>*)__curr_rope;
		_RopeRep* __left = __c->_M_left;
		size_t __left_len = __left->_M_size;
		
		__dirns <<= 1;
		if (__pos >= __curr_start_pos + __left_len) {
		    __dirns |= 1;
		    __curr_rope = __c->_M_right;
		    __curr_start_pos += __left_len;
		} else {
		    __curr_rope = __left;
		}
	    }
	    break;
	}
    }
  done:
    // Copy last section of path into _M_path_end.
      {
	int __i = -1;
	int __j = __curr_depth + 1 - _S_path_cache_len;

	if (__j < 0) __j = 0;
	while (__j <= __curr_depth) {
	    __x._M_path_end[++__i] = __path[__j++];
	}
	__x._M_leaf_index = __i;
      }
      __x._M_path_directions = __dirns;
      _S_setbuf(__x);
}

// Specialized version of the above.  Assumes that
// the path cache is valid for the previous position.
template <class _CharT, class _Alloc>
void _Rope_iterator_base<_CharT,_Alloc>::_S_setcache_for_incr
(_Rope_iterator_base<_CharT,_Alloc>& __x)
{
    int __current_index = __x._M_leaf_index;
    const _RopeRep* __current_node = __x._M_path_end[__current_index];
    size_t __len = __current_node->_M_size;
    size_t __node_start_pos = __x._M_leaf_pos;
    unsigned char __dirns = __x._M_path_directions;
    _Rope_RopeConcatenation<_CharT,_Alloc>* __c;

    __stl_assert(__x._M_current_pos <= __x._M_root->_M_size);
    if (__x._M_current_pos - __node_start_pos < __len) {
	// More stuff in this leaf, we just didn't cache it.
	_S_setbuf(__x);
	return;
    }
    __stl_assert(__node_start_pos + __len == __x._M_current_pos);
    //  node_start_pos is starting position of last_node.
    while (--__current_index >= 0) {
	if (!(__dirns & 1)) // Path turned left 
	  break;
	__current_node = __x._M_path_end[__current_index];
	__c = (_Rope_RopeConcatenation<_CharT,_Alloc>*)__current_node;
	// Otherwise we were in the right child.  Thus we should pop
	// the concatenation node.
	__node_start_pos -= __c->_M_left->_M_size;
	__dirns >>= 1;
    }
    if (__current_index < 0) {
	// We underflowed the cache. Punt.
	_S_setcache(__x);
	return;
    }
    __current_node = __x._M_path_end[__current_index];
    __c = (_Rope_RopeConcatenation<_CharT,_Alloc>*)__current_node;
    // current_node is a concatenation node.  We are positioned on the first
    // character in its right child.
    // node_start_pos is starting position of current_node.
    __node_start_pos += __c->_M_left->_M_size;
    __current_node = __c->_M_right;
    __x._M_path_end[++__current_index] = __current_node;
    __dirns |= 1;
    while (_RopeRep::_S_concat == __current_node->_M_tag) {
	++__current_index;
	if (_S_path_cache_len == __current_index) {
	    int __i;
	    for (__i = 0; __i < _S_path_cache_len-1; __i++) {
		__x._M_path_end[__i] = __x._M_path_end[__i+1];
	    }
	    --__current_index;
	}
	__current_node =
	    ((_Rope_RopeConcatenation<_CharT,_Alloc>*)__current_node)->_M_left;
	__x._M_path_end[__current_index] = __current_node;
	__dirns <<= 1;
	// node_start_pos is unchanged.
    }
    __x._M_leaf_index = __current_index;
    __x._M_leaf_pos = __node_start_pos;
    __x._M_path_directions = __dirns;
    _S_setbuf(__x);
}

template <class _CharT, class _Alloc>
void _Rope_iterator_base<_CharT,_Alloc>::_M_incr(size_t __n) {
    _M_current_pos += __n;
    if (0 != _M_buf_ptr) {
        size_t __chars_left = _M_buf_end - _M_buf_ptr;
        if (__chars_left > __n) {
            _M_buf_ptr += __n;
        } else if (__chars_left == __n) {
            _M_buf_ptr += __n;
            _S_setcache_for_incr(*this);
        } else {
            _M_buf_ptr = 0;
        }
    }
}

template <class _CharT, class _Alloc>
void _Rope_iterator_base<_CharT,_Alloc>::_M_decr(size_t __n) {
    if (0 != _M_buf_ptr) {
        size_t __chars_left = _M_buf_ptr - _M_buf_start;
        if (__chars_left >= __n) {
            _M_buf_ptr -= __n;
        } else {
            _M_buf_ptr = 0;
        }
    }
    _M_current_pos -= __n;
}

template <class _CharT, class _Alloc>
void _Rope_iterator<_CharT,_Alloc>::_M_check() {
    if (_M_root_rope->_M_tree_ptr != _M_root) {
        // _Rope was modified.  Get things fixed up.
        _M_root = _M_root_rope->_M_tree_ptr;
        _M_buf_ptr = 0;
    }
}

template <class _CharT, class _Alloc>
inline 
_Rope_const_iterator<_CharT, _Alloc>::_Rope_const_iterator(
  const _Rope_iterator<_CharT,_Alloc>& __x)
: _Rope_iterator_base<_CharT,_Alloc>(__x) 
{ }

template <class _CharT, class _Alloc>
inline _Rope_iterator<_CharT,_Alloc>::_Rope_iterator(
  Rope<_CharT,_Alloc>& __r, size_t __pos)
: _Rope_iterator_base<_CharT,_Alloc>(__r._M_tree_ptr, __pos), 
  _M_root_rope(&__r)
{
}
*/

// -------------------------------------------------------------------------
// $Log: RopeIter.h,v $

__NS_STD_END

#endif /* __STDEXT_TEXT_ROPE_ROPEITER_H__ */
