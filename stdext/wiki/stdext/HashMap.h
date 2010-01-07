//@encoding utf8
//@ns NS_STDEXT

// -------------------------------------------------------------------------
// class HashMap

/** 哈希表。这是一种存储Key-Value对为元素的容器，可通过查询Key快速得到对应的Value。
@arg KeyT
	哈希表中Key-Value对中Key的类型。
@arg DataT
	哈希表中Key-Value对中Value的类型。
@arg HashCompT
	哈希表Key需要实现的操作类。其规格为：
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // 求Key的hash值
				typename key_equal; // 比较两个Key是否相等
				typename key_pred; // 判断一个Key是否小于另一个Key
			};
		[[/code]]
	默认 HashCompT 取 NS_STDEXT::HashCompare<KeyT>。
@arg AllocT
	该Hash表的内存分配操作类。默认 AllocT 取 NS_STDEXT::DefaultAlloc。
@*/
template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
	>
class HashMap : public PHashMap<KeyT, DataT, HashCompT, AllocT>
{
	
/**	哈希表初始化
@arg alloc
	哈希表使用的内存分配器。
@arg n
	根据输入的n值，计算哈希表的存储桶数量。
@*/
	HashMap(AllocT& alloc, size_type n = 100);

/**	哈希表初始化
@arg alloc
	哈希表使用的内存分配器。
@arg first
	从迭代器向哈希表输入的第一个元素。
@arg last
	从迭代器向哈希表输入的最后一个元素。
@arg n
	哈希表的存储桶数量。
@*/
	template <class Iterator>
	HashMap(AllocT& alloc, Iterator first, Iterator last, size_type n = 100);
};

// -------------------------------------------------------------------------
// class HashMultiMap

/** 多值哈希表。与普通哈希表不同的是，这种类型的哈希表一个Key可以对应多个Value。
@arg KeyT
	哈希表中Key-Value对中Key的类型。
@arg DataT
	哈希表中Key-Value对中Value的类型。
@arg HashCompT
	哈希表Key需要实现的操作类。其规格为：
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // 求Key的hash值
				typename key_equal; // 比较两个Key是否相等
				typename key_pred; // 判断一个Key是否小于另一个Key
			};
		[[/code]]
	默认 HashCompT 取 NS_STDEXT::HashCompare<KeyT>。
@arg AllocT
	该Hash表的内存分配操作类。默认 AllocT 取 NS_STDEXT::DefaultAlloc。
@*/
template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
	>
class HashMultiMap : public PHashMultiMap<KeyT, DataT, HashCompT, AllocT>
{

/** 多值哈希表初始化
@arg alloc
	多值哈希表使用的内存分配器。
@arg n
	多值哈希表的存储桶数量。
@*/
	HashMultiMap(AllocT& alloc, size_type n = 100);

/** 多值哈希表初始化
@arg alloc
	多值哈希表使用的内存分配器。
@arg first
	从迭代器向多值哈希表输入的第一个元素。
@arg last
	从迭代器向多值哈希表输入的最后一个元素。
@arg n
	多值哈希表的存储桶数量。
@*/
	template <class Iterator>
	HashMultiMap(AllocT& alloc, Iterator first, Iterator last, size_type n = 100);
};
