# HOW TO USE?
`make` and create `run.sh` or `run_mac.sh` and `./run.sh ./executable`
## run.sh
- must replace `${dynamic libfile name}` to `libft_malloc_${HOST_TYPE}.so`

## V2
### Pre allocation (Warm cache)
- Cache (Pool) can be relatively slow on the first `malloc()` call if it is not prepared before program startup.
- The cache can be initialized via `__attribute__((constructor))`.
- Conversely, the `__attribute__((destructor))` rule can be used to return resources at program end.
- They are defined as `__CONSTRUCT__` `__DESTRUCT__` respectively.

### Buddy allocator
- 이 malloc은 buddy allocator system 을 사용한다.
- buddy allocator를 통해 external fragmentation의 문제를 해결하였다. 다만, `2^n` 이 아닌 경우에 internal fragmentation의 문제가 발생한다.
- internal fragmentation 문제를 최소화하기 위해 사이즈별로 타입을 분리하였다.
    - TINY
        - TINY 의 경우 1 ~ 256 byte 의 사이즈의 할당 요청에 해당한다.
        - 최소 블록의 사이즈는 8 byte 이며 2 배씩 사이즈가 총 6 개의 class 로 존재한다.
            - 따라서  8 / 16 / 32 / 64 / 128 / 256 이 존재한다.
    - SMALL
        - SMALL 의 경우 257 ~ 4096 byte 의 사이즈 할당 요청에 해당한다.
        - 최소 블록의 사이즈는 512 byte 이며  2 배씩 사이즈가 총 4 개의 class 로 존재한다.
            - 따라서 512 / 1024 / 2048 / 4096
        - SMALL 타입의 경우 1 Page 이내의 큰 할당 요청에 사용하기 위해 제작된 타입이다.
        - Page size = 4 kb 일 때, 각 블록은 동일한 페이지를 사용함을 보장받는다. 
    - LARGE
        - LARGE 의 경우 > 4096 의 할당 요청에 해당한다.
        - mmap 을 통해 Page size aligned 된 크기의 블록을 제공한다.
- buddy allocation 은 TINY, SMALL 타입에서만 해당된다.
- 각 타입의 최대 사이즈 (256, 4096) byte 을 각각 bucket 사이즈로 가지고 있다. bucket 내에서만 split / merge 과정이 일어난다.
- pool 내에서 allocation 시간은 `O(log(max_order))` 이다.
    - split 시간에 해당된다. max_order 는 class의 개수에 해당한다.
- pool 내에서 free 시간은 `O(log(max_order))` 이다.
    - merge 시간에 해당된다. max_order 는 class의 개수에 해당한다.

### Performance
- Buddy Allocator를 사용하여 `2^n` 사이즈의 블록을 할당하는 경우 메모리 효율이 100%에 가깝다. (metadata로 인한 오버헤드만 존재)
- 반면, 그렇지 않은 경우 internal fragmentation이 발생한다. size 보다 크거나 같은 2^n 만큼이 할당되기 때문이다.
- memory 탐색 과정의 경우 `O(p)` (p == `pool` 개수) 이다.
    - `free()` 과정에서 allocation 된 memory를 찾는 과정에서 `O(p)` (p == `pool` 개수) 으로 느린 문제가 있다.
    - [해결중](#TODO)
- `free()` 이후 block size (`2^n` block) 기준 `pool` 내에서 외부 단편화가 발생하지 않는다.
- `free()` 이후 free pool의 최대 개수는 1개로 제한된다. (미사용 pool은 회수된다.) 


### Metadata
- 해당 Allocator에서는 Malloc metadata를 별도의 공간에 저장한다.
- Buddy allocation system을 사용하는 `TINY` / `SMALL` 사이즈에서는 `Pool` 의 뒤에 메모리 관리를 위한 metadata 가 존재한다.
    - `TINY` 사이즈에서 metadata로 인한 오버헤드는 약 12 % `SMALL` 사이즈에서는 약 1% 이다.
- Buddy allocation system을 사용하지 않는 `LARGE` 사이즈의 경우, `Pool` 에서 관리하기에 사실상 오버헤드가 0% 에 가깝다.


# TODO
## 탐색 속도 개선
- `block` 에 해당 하는 `pool`을 찾는데 시간이 오래 걸리는 문제를 해결하고자한다.
- 현재 구조에서는 `block` 의 주소는 `pool` 객체의 주소와 관련이 없다.
    - 따라서, `pool`을 관리하고 있는 객체 속에서 순회하며 `block`이 속한 `pool`을 찾아야한다.
    - 이 순회 속도가 탐색 속도에 영향을 미친다.
- `pool` 관리를 위해 linked list를 사용하고 있는데, 적절한 `pool`의 위치를 찾는 시간이 `O(N)` 으로 `pool`의 개수가 많은 상황에서 탐색 속도가 급격히 저하되는 문제가 있다.
- `pool` 을 관리하는 방식을 linked list 에서 balanced binary tree로 전환하면 `O(logN)` 으로 낮출 수 있다.
    - 메모리 주소의 경우 새로 만들 때 이전 주소값에 비해 증가하기 때문에 일반 binary tree로 만들게 될 경우, 선형 구조가 될 위험이 있다.
- `manager` 의 경우, 각 타입마다 별도의 linked list를 가지고 있었는데, 새로운 구조에서는 통합된 tree 에 함께 관리하는 것이 적합하다.

### binary tree
- AVL tree 를 활용한다.
    -  RB-tree 보다 엄격하게 tree 의 balance 가 유지되어 탐색이 빈번한 현재 상황에 적합하다.
- node 는 `pool` 구조체를 그대로 사용한다.
- tree 의 key는 `pool->addr` 이다.

### 변경사항
- 변경될 요소들은 다음과 같다.
    - `t_pool` struct 의 내부 요소
        - tree node 화를 위한 `parent` / `left` / `right`
        - 기존 `free_list` 삭제
            - `O(1)` 할당을 위함. `free_list` 관리를 `manager` 단에서 수행
        - 그 결과는 다음과 같다.
        ```C
        // pool sturcture
        typedef struct s_pool {
            struct s_pool *parent;	// parent node
            struct s_pool *left;	// left node
            struct s_pool *right;	// right node
            size_t height;			// height of tree
            void *addr;				// pool addr
            t_metadata *metadata;	// information data space address in pool
            POOL_TYPE type;			// type of pool
            size_t size;			// pool size (user space + metadata)
            size_t allocated_size;	// actually allocated size in pool
            size_t user_space_size; // maximum allocation size
        } t_pool;
        ```
    - `t_mmanger` struct 의 내부 요소
        - `tiny_pool_head` / `small_pool_head` / `large_pool_head` 가 삭제된다.
            - 대신, `t_pool *pool_head` 로 통합되어 관리된다.
        - `bool tiny_has_free_pool` / `bool small_has_free_pool`
            - type 별로 `pool` 이 관리되고 있지 않기 때문에, 각 type 에서 `free_pool` 이 이미 존재하는 경우, 새로운 `free_pool` 의 경우 즉시 반환하도록 한다.
        - `t_block *tiny_free_list[MAX_TINY_ORDER]`
        - `t_block *small_free_list[MAX_SMALL_ORDER]`
            - `TINY` / `SMALL` 에 대해서 `free_list` 를 `manager` 에서 관리한다.
            - `free_list` 가 포화될 위험은 낮다. `free_pool` 이 최대 한 개 존재할 수 있기 때문이다.
        - 그 결과는 다음과 같다.
        ```C
        // ptr space
        typedef struct s_pool_space {
            t_pool *head;
            t_block *tiny_free_list[MAX_ORDER_TINY];
            t_block *small_free_list[MAX_ORDER_SMALL];
            bool tiny_has_free_pool;
            bool small_has_free_pool;
            t_pmalloc_space *pmalloc_space;
        } t_mmanager;
        ```
### 기대효과
- **NOTE: P = POOL**
- `free_block` 탐색 속도 개선
    - O(P) -> O(1)
    - 기존의 경우 `pool` 을 순회하며 `free_block을` 찾아야했다.
        - 높은 확률로 앞쪽 pool 에 `free_block` 이 있으나, 그렇지 않은 경우 문제가 발생한다.
- `block` 에 해당하는 `pool` 찾는 속도 개선
    - O(P) -> O(logP)
- `free()` 속도 개선
    - O(P) -> O(logP)
    - 특히 `free()` 의 경우 이미 많은 `pool` 이 존재할 경우에 `free()` N번의 요청에 O(N * P) 로 급격하게 증가하였으며, `shrink_pool` 까지 여러 번 일어나는 케이스에서는 더 악화되는 문제가 있었다.