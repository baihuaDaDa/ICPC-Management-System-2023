# 复杂度分析报告
>ICPC Management System

## 一、关于STL库的一些操作的复杂度说明
*设$N$为队伍总数*
### No.1 map/unordered_map
map是基于红黑树维护的一种有序STL容器，而unorder_map是基于哈希表维护的一种无序STL容器这里我们仅说明某些此报告涉及的map/unordered_map操作的**平均情况下的**时间复杂度：
- map
  - `map.insert()`/`map[key] = value`
  - `map.erase()`
  - `map.find()`

以上操作平均时间复杂度均为O($log N$)
- unordered_map
  - `unordered_map.insert()`/`unordered_map[key] = value`
  - `unordered_map.erase()`
  - `unordered_map.find()`

以上操作平均时间复杂度均为O($1$)
### No.2 set/multiset
set/multiset同样也是基于红黑树维护的一种有序STL容器：
- `set.insert()`/`multiset.insert()`
- `set.erase()`/`multiset.erase()`
- `set.find()`/`multiset.find()`

以上操作平均时间复杂度均为O($log N$)
### No.3 vector
- `vector.push_back()`

以上操作的平均时间复杂度为O($1$)，只有当vector容量不够时才会开辟新的内存空间并将所有元素复制到新的内存空间，此时时间复杂度是O($N$)。因此，除非需要频繁地重新分配内存空间，否则在大多数情况下，std::vector的push_back操作的时间复杂度为O($1$)。
### No.4 iterator
- `end()`
- `begin()`
- `*iter`
- `iter++`
- `iter1 != iter2`

以上操作在基于标准库容器时的平均时间复杂度是O($1$)。
## 二、某些操作的复杂度分析
*设$N$为队伍总数*
### No.1 添加队伍（ADDTEAM）
添加队伍操作中主要包含：
- 向unordered_map插入元素
- 向set中插入元素
- unordered_map的查找

其时间复杂度分别是O($1$)、O($log N$)、O($1$)，且一次ADDTEAM操作至多向set中插入一次元素，故ADDTEAM操作的时间复杂度为O($log N$)。
### No.2 提交题目（SUBMIT）
提交题目操作中主要包含：
- 调用unordered_map中键值对
- set的erase操作
- set的insert操作
- multiset的insert操作

其时间复杂度分别是O($1$)、O($log N$)、O($log N$)、O($log N$)，且每次SUBMIT操作至多执行一次set的insert和erase操作以及multiset的insert操作。此外，使用&运算符引用了unordered_map的键值对的地址，每次SUBMIT操作仅从unordered_map中调用一次键值对，减少了多次复制带来的时间浪费。故SUBMIT操作的时间复杂度是O($log N$)。
### No.3 刷新榜单（FLUSH）
刷新榜单操作中通过迭代器将set遍历了一遍，每次循环中均各有一次：
- 迭代器比较
- 迭代器移动
- 迭代器访问
- 调用unordered_map键值对

故FLUSH操作的时间复杂度是O($N$)。
### No.4 滚榜（SCROLL）
滚榜操作是ICPC管理系统中最复杂的一种操作，主要包含四个部分：
- 输出滚榜前榜单

  此部分主要包含：
  - 一次FLUSH操作
  - $N$次通过下标遍历储存问题信息的vector
  
  其时间复杂度分别为O($N$)，O($kN$)（k表示问题总数）。

- 输出解冻导致的排名变化
  
  此部分主要包含：
  - 不超过$k$次的调用unordered_map中的键值对
  - $k$次执行set的erase和insert函数（$k$代表冻结题目数量）
  - 另外$N$次执行set的erase函数（将题目解冻完的队伍移出排行榜）
  - 最多执行$k$次的multiset的insert函数（$k$代表冻结题目数量）
  
  由于$k$最大为problem_count*$N$，故以上操作的时间复杂度分别为O($N$)、O($Nlog N$)、O($Nlog N$)O($Nlog N$)。
- 将队伍全部加入回set中

  其时间复杂度为O($Nlog N$)。
- 输出滚榜后榜单
  此部分主要包含：
  - 一次FLUSH操作
  - $N$次通过下标遍历储存问题信息的vector
  
  其时间复杂度分别为O($N$)，O($kN$)（k表示问题总数）。

综上，SCROLL的时间复杂度是O($Nlog N$)。
### No.5 查询队伍排名（QUERY_RANKING）
查询队伍排名操作主要包括：
- 一次unordered_map的find函数

其时间复杂度为O($1$)，此外还有：
- 一次迭代器访问

故QUERY_RANKING的时间复杂度是O($1$)。
### No.6 查询队伍提交情况（QUERY_SUBMISSION）
查询队伍提交情况操作主要包括：
- unordered_map的查找
- 访问键值对
- 取字符串子串等操作

其时间复杂度分别为O($1$)、O($1$)、O($k$)（$k$表示子串长度，在子串长度较小时可视作常量级时间复杂度），故QUERY_SUBMISSION操作的时间复杂度是O($1$)。