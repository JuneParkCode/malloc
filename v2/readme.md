# HOW TO USE?
`make` and `./run.sh ./executable`
## run.sh
- must replace `${dynamic libfile name}` to `libft_malloc_${HOST_TYPE}.so`

## LLDB
- SET ENV by `settings set target.env-vars`

## V2
### Pre allocation (Warm cache)
- Cache (Pool) can be relatively slow on the first `malloc()` call if it is not prepared before program startup.
- The cache can be initialized via `__attribute__((constructor))`.
- Conversely, the `__attribute__((destructor))` rule can be used to return resources at program end.
- They are defined as `__CONSTRUCT__` `__DESTRUCT__` respectively.

### Pool (cache)
- `pool` 은 시스템으로부터 메모리를 미리 할당받아, 빠르게 유저에게 메모리를 제공하는데 목적이 있다. 
- `<= page_size` 할당 요청 대상의 경우 `pool` 에 캐시된 블록을 통해 시스템콜 없이 빠르게 유저에게 전달된다.
- `pool` 의 경우 총 2 가지 타입이 있다. `TINY` / `SMALL` 두 가지 타입이 있으며, 아래 [buddy allocator](#buddy-allocator) 에서 자세히 설명되어있다.
- `LARGE` 또한 `pool` 을 통해 관리되나, 이 때의 `pool`은 단순히 메모리 주소 추적을 위한 Tree node 이다.
- `pool` 객체의 경우 별도의 `pool` 에 관련된 메타데이터를 할당하는 `pmalloc` 이라는 할당을 통해서 할당된다. 
    - `pmalloc` 의 경우 `pool` 노드 할당을 위해 내부적으로 활용되는 함수이며, `malloc()` 의 구현과는 다소 다르다.
    - 각 pool 할당을 위해 `96 byte` 의 메모리 소모가 있다. 


```
 해당 malloc의 초기 버전의 경우 linked list를 통해 `pool` 을 관리하였다. 그러나, `pool` 의 개수가 증가하면서 급격하게 `free()` 의 속도가 저하되는 문제가 발생하였다. 문제는 `block` 이 위치한 `pool` 을 탐색하는 과정이 `O(N)` 이었으며, 이 과정에서 `pool` 이 삭제되어야하는 경우 `O(N)` 의 작업이 재차 소요되었다. 따라서, `pool` 탐색 속도를 개선하기 위해 BST 로 `pool` 을 관리할 수 있도록 했다.

 AVL tree / RB tree 의 두 가지 선택지를 대상으로 고려를 하였고, malloc 의 경우 `pool` 삽입 / 삭제 연산보다 탐색 연산이 훨씬 더 많이 일어난다. 따라서, 엄격하게 균형이 유지되는 AVL tree 를 선택하게 되었다.
```

### Buddy allocator
- 이 malloc은 buddy allocator system 을 사용한다.
- buddy allocator를 통해 external fragmentation의 문제를 해결하였다. 다만, `2^n` 이 아닌 경우에 internal fragmentation의 문제가 발생한다.
- internal fragmentation 문제를 최소화하기 위해 사이즈별로 타입을 분리하였다.
    - `TINY`
        - `TINY` 의 경우 1 ~ 256 byte 의 사이즈의 할당 요청에 해당한다.
        - 최소 블록의 사이즈는 8 byte 이며 2 배씩 사이즈가 총 6 개의 class 로 존재한다.
            - 따라서  8 / 16 / 32 / 64 / 128 / 256 이 존재한다.
        - 내부 구현상에서는 7개의 클래스가 더 존재하나, 블록 관리를 위해 사용하는 class 이다.
    - `SMALL`
        - `SMALL` 의 경우 257 ~ 4096 byte 의 사이즈 할당 요청에 해당한다.
        - 최소 블록의 사이즈는 512 byte 이며  2 배씩 사이즈가 총 4 개의 class 로 존재한다.
            - 따라서 512 / 1024 / 2048 / 4096
        - `SMALL` 타입의 경우 1 Page 이내의 큰 할당 요청에 사용하기 위해 제작된 타입이다.
        - Page size = 4 kb 일 때, 각 블록은 동일한 페이지를 사용함을 보장받는다. 
            - 따라서  8 / 16 / 32 / 64 / 128 / 256 이 존재한다.
        - 내부 구현상에서는 7개의 클래스가 더 존재하나, 블록 관리를 위해 사용하는 class 이다.
    - `LARGE`
        - `LARGE` 의 경우 > 4096 의 할당 요청에 해당한다.
        - mmap 을 통해 Page size aligned 된 크기의 블록을 제공한다.
- buddy allocation 은 `TINY`, `SMALL` 타입에서만 해당된다.
- 각 POOL 은 128 개의 최대 사이즈를 담을 수 있는 `userspace` 를 가지고 있다.
- pool 내에서 allocation 시간은 `O(log(pool) + log(max_order))` 이다.
    - split 시간과 pool의 개수에 영향을 받는다. max_order 는 class의 개수에 해당한다.
- pool 내에서 free 시간은 `O(log(pool) + log(max_order))` 이다.
    - merge 시간과 pool의 개수에 영향을 받는다. 해당된다. max_order 는 class의 개수에 해당한다.

### Performance
- Buddy Allocator를 사용하여 `2^n` 사이즈의 블록을 할당하는 경우 메모리 효율이 100%에 가깝다. (metadata로 인한 오버헤드만 존재)
- 반면, 그렇지 않은 경우 internal fragmentation이 발생한다. size 보다 크거나 같은 2^n 만큼이 할당되기 때문이다.
- ~~memory 탐색 과정의 경우 `O(p)` (p == `pool` 개수) 이다.~~
    - ~~`free()` 과정에서 allocation 된 memory를 찾는 과정에서 `O(p)` (p == `pool` 개수) 으로 느린 문제가 있다.~~
    - `pool` 을 AVL 트리를 통해 관리함으로서 해결하였음. `O(logP)` 로 개선.  
- `free()` 이후 block size (`2^n` block) 기준 `pool` 내에서 외부 단편화가 발생하지 않는다.
- `free()` 이후 free pool의 최대 개수는 1개로 제한된다. (미사용 pool은 회수된다.) 


### Metadata
- 해당 Allocator에서는 Malloc metadata를 별도의 공간에 저장한다.
- Buddy allocation system을 사용하는 `TINY` / `SMALL` 사이즈에서는 `Pool` 의 뒤에 메모리 관리를 위한 metadata 가 존재한다.
    - `TINY` 사이즈에서 metadata로 인한 오버헤드는 약 12 % `SMALL` 사이즈에서는 약 1% 이다.
- Buddy allocation system을 사용하지 않는 `LARGE` 사이즈의 경우, `Pool` 에서 관리하기에 사실상 오버헤드가 0% 에 가깝다.
    - `pmalloc()` 을 통해 할당되는 `pool` 노드의 크기만큼만 사용한다. `(96 byte)`
