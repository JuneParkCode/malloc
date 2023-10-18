# HOW TO USE?
`make` and create `run.sh` and `./run.sh executable`
## run.sh
- must replace `${dynamic libfile name}` to `libft_malloc${HOST_TYPE}.so`
### MAC
```sh
export DYLD_LIBRARY_PATH=. 
export DYLD_INSERT_LIBRARIES="${dynamic libfile name}"
export DYLD_FORCE_FLAT_NAMESPACE=1 
```
### LINUX
```sh
export LD_PRELOAD="${dynamic libfile name}"
```


# 고려사항 (v2에서 개발할 내용)
## Warm Cache
- Cache (Pool) 이 프로그램 시작 전에 미리 준비되지 않을 경우 첫 malloc() 호출에서 상대적으로 느려질 수 있음.
- `__attribute__((contructor))` 를 통해서 cache 에 대한 initialization 이 가능함.
- 반대로 `__attribute__((destructor))` 룰 통해서 프로그램 종료시 자원을 돌려주는 것도 가능함.
## Dynamic Library external / internal functions
- 동적 라이브러리 생성시 내부적으로 사용되는 심볼에 대해서 사용자가 접근하지 못하도록 제어해야함
- `__attribute__((visibility("default")))` 와 같이 설정하여 visable 하게 설정 가능
- 감추는 심볼에 대해서는 `__attribute__((visibility("hidden")))` 을 사용하여 사용자가 접근할 수 없게 만들 수 있음.

## Thread local stroage
### Cleanup
- `pthread_key_create ` 를 이용하여 `cleanup` 함수 등록
- `cleanup` 함수에서는 `arena`로 `free block` 이동시킬 수 있음.
## Allocating strategy
-  `Defragmentation`, `Fast`, `Thread-safe` 에 대해서 모두 고려한 설계가 필요함
#### **slab allocator**
- 미리 지정된 사이즈의 블록 풀을 유지함
  - 16 bytes : 100 blocks
  - 32 bytes : 100 blocks
  - 48 bytes : 100 blocks
  - ...
  - 512 bytes : 100 blocks
- `merge` 과정이 생략될 수 있음.
- 할당이 되고, 안된 것은 `bitmap`으로 단순화해서 표현할 수 있음.
- 과제 요구 조건을 만족하기 위해서는 `TYPE_MAX` 사이즈의 블록이 100 개 이상 미리 존재해야하는데, slab allocator 방식에서는 굉장히 비효율적임
- 특정 사이즈의 `pool`을 `expand` 하는 과정도 고려해야함.
- 구현에 있어 `merge` 하지 않아도 되며, 이미 존재하는 블록에 대해서 다루기 때문에 빠르고 관리가 간편하다. 또한 multi-thread 상황에서 관리가 보다 유용하다는 장점이 있음.
- 현대 allocator들이 많이 취하고 있는 방식임.
#### **buddy allocator**
- 블록의 개수를 `2^n` 개로 유지함
- 작은 블록을 만들 때 절반씩 쪼개면서 블록을 생성함
  - 512 byte 공간에 16 byte 블록을 할당 할 때
  - 512
  - 256L / 256R 
  - (128L / 128R) / 256R
  - ((64L / 64R) / 128R) / 256R
  - (((32L / 32R) / 64R) / 128R) / 256R
  - ((((16L / 16R) / 32R) / 64R) / 128R) / 256R
  - 결과적으로 16L 이 할당되고 나머지는 `Free block`으로 남음
- `merge` 하는 과정이 단순히 좌우를 비교하면서 해결할 수 있어서 간단한 편
- 블록에 대한 정보를 기록하기 위해서 별도의 공간을 반드시 고려해야함
#### **각 타입의 MAX 짜리의 블록으로 관리**
- `arena`에서 블록을 관리할 때, 최대 크기의 블록을 단위로 관리하고, 여기서 다시 쪼개서 사용하는 방식. 최소 크기의 블록까지 쪼개질 수 있으며, 합쳐질 때는 원래 형태의 블록으로만 합쳐질 수 있음.
- 여전히 `fragmentation`이 일어날 수 있음. 16 / 496 블록만 잔뜩 남으면 가용 사이즈는 넘치는데 아무것도 하지 못함.
#### **Split & Merge**
- `TYPE_MAX` 사이즈의 `block` 을 100 개 이상 담을 수 있는 공간을 하나의 블록으로 사용하고, 요구되는 사이즈를 배수 크기만큼만 split 해서 사용함.
- 반대로 free 시에는 인접한 `block` 을 조사하여 merge 함
- 구현의 난이도가 낮으나, `defragmentation` 에서 다소 불리함.
- slab allocator에 비해서 속도의 문제와, multi-thread 상황에서 관리가 어려움.
