//@encoding utf8
//@ns NS_STDEXT

// -------------------------------------------------------------------------
// hashOfString

/** 计算字符串的哈希值。
@arg it
	迭代器的第一个元素。
@arg itEnd
	迭代器的最后一个元素。
@return
	返回哈希值。
@*/
template <class Iterator>
inline size_t winx_call hashOfString(Iterator it, Iterator itEnd)；

/**	计算字符串的哈希值。
@arg s
	进行计算哈希值的迭代器。
@return 
	返回哈希值。
@*/
template <class Iterator>
inline size_t winx_call hashOfString(Iterator s)；

// -------------------------------------------------------------------------
// class Hash

/** Hash类提供了计算哈希值的方法。
@*/
template <class ValueT>
class Hash
{
/** 计算哈希值的方法。
@arg val
	需要计算哈希值的参数。
@return
	返回参数的哈希值。
@*/
	size_t winx_call operator()(const ValueT& val) const;
};

// -------------------------------------------------------------------------
// class HashCompare

/** 哈希集合Key需要实现的操作类。
@*/
template <class KeyT>
class HashCompare
{
/** 求Key的hash值
@*/
	typedef hasher;

/** 比较两个Key是否相等
@*/
	typedef key_equal;

/** 判断一个Key是否小于另一个Key
@*/
	typedef key_pred;
};

// -------------------------------------------------------------------------
