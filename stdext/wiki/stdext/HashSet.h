//@encoding utf8
//@ns NS_STDEXT

// -------------------------------------------------------------------------
// class HashSet

/** 哈希集合。这是一种存储Key的容器，该容器存储的Key是唯一的。哈希集合可以用来消除重复的Key。
@arg ValT
	哈希集合中存放的Key数据类型。
@arg HashCompT
	哈希集合Key需要实现的操作类。其规格为：
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // 求Key的hash值
				typename key_equal; // 比较两个Key是否相等
				typename key_pred; // 判断一个Key是否小于另一个Key
			};
		[[/code]]
	默认 HashCompT 取 NS_STDEXT::HashCompare<ValT>。
@arg AllocT
	该哈希集合的内存分配操作类。默认 AllocT 取 NS_STDEXT::DefaultAlloc。
@*/
template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashSet : public PHashSet<ValT, HashCompT, AllocT>
{

/** 哈希集合的初始化。
@arg alloc
	哈希集合使用的内存分配器。
@arg n
	根据输入的n值，计算存储桶数量。
@*/
	explicit HashSet(AllocT& alloc, size_type n = 100)
		: Base(alloc, n) {}

/** 哈希集合的初始化
@arg alloc
	哈希集合使用的内存分配器。
@arg first
	从迭代器向哈希集合输入的第一个元素。
@arg last
	从迭代器向哈希集合输入的最后一个元素。
@arg n
	根据输入的n值，计算存储桶数量。
@*/
	template <class Iterator>
	HashSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(alloc, first, last, n) {}
};

// -------------------------------------------------------------------------
// class HashMultiSet

/** 多值哈希集合。这是一种存储Key的容器，与普通哈希集合不同的是，可以有多个相同的Key。
@arg ValT
	哈希集合中存放的Key数据类型。
@arg HashCompT
	哈希集合Key需要实现的操作类。其规格为：
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // 求Key的hash值
				typename key_equal; // 比较两个Key是否相等
				typename key_pred; // 判断一个Key是否小于另一个Key
			};
		[[/code]]
	默认 HashCompT 取 NS_STDEXT::HashCompare<ValT>。
@arg AllocT
	该哈希集合的内存分配操作类。默认 AllocT 取 NS_STDEXT::DefaultAlloc。
@*/
template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashMultiSet : public PHashMultiSet<ValT, HashCompT, AllocT>
{

/** 多值哈希集合的初始化
@arg alloc
	多值哈希集合使用的内存分配器。
@arg n
	根据输入的n值，计算存储桶数量。
@*/
	explicit HashMultiSet(AllocT& alloc, size_type n = 100)
		: Base(alloc, n) {}

/** 多值哈希集合的初始化
@arg alloc
	多值哈希集合使用的内存分配器。
@arg first
	从迭代器向多值哈希集合输入的第一个元素。
@arg last
	从迭代器向多值哈希集合输入的最后一个元素。
@arg n
	根据输入的n值，计算存储桶数量。
@*/
	template <class Iterator>
	HashMultiSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(alloc, first, last, n) {}
};