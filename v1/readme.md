# HOW TO USE?
`make` and create `run.sh` and `./run.sh executable`
## run.sh
- must replace `${dynamic libfile name}` to `libft_malloc_${HOST_TYPE}.so`
### MAC
```sh
export DYLD_LIBRARY_PATH=. 
export DYLD_INSERT_LIBRARIES="${dynamic libfile name}"
$@
```
### LINUX
```sh
export LD_LIBRARY_PATH=.
export LD_PRELOAD="{dynamic libfile name}.so"
$@
```

# ft_malloc
- 42 과제로 개발한 malloc() 입니다.
- 해당 버전은 작동하나, 완벽하지 않은 버전입니다.

# 구조

## ARENA

- 멀티스레드 환경을 위해서, `HEAP` 공간을 각 스레드에 묶는 역할을 한다.
- 각 스레드는 `ARENA`에 할당된 `POOL(BIN)`을 통해서 메모리를 사용한다.
- `ARENA`는 `시스템코어 * 8` 만큼 할당된다. `round-robin` 방식으로 각 `ARENA`가 할당되어 사용된다.
    - 할당 이후, 해당 스레드는 본인의 `ARENA`만 이용한다.
    - 스레드가 이보다 많을 경우, 해당하는 `ARENA`에 대해 경합이 일어날 수 있다.
- `static global variable` 로 `ARENA` 의 구조체가 미리 할당이 되어있다.
- 할당을 해제하는 경우, 메모리 블록이 속하는 `ARENA`로 Free node를 반환하여야하는데,  이를 위해서 해당 노드의 `ARENA`를 찾는 과정이 필요할 수도 있다.
    - 이는 바람직한 프로그래밍(user level)은 아니나, 시스템 오류를 막기 위해 구현해야함.
    - 어느 `ARENA`에 속한지 파악하기 위해, 해당 메모리 주소를 통해서 어느 페이지가 어떤 `ARENA`에 속하는지 파악하는 알고리즘이 필요하다.
        - `POOL`의 사이즈는 정해져있다. 따라서,  시작점으로부터 해당 크기만큼의 메모리 주소를 범위로 잡고 탐색한다면, 어느 `ARENA`에 속한지 알 수 있을 것이다.
            - LARGE의 경우 즉시 해제이므로 즉시 해제를 하면 해결할 수 있따.

### member

- `ARENA_NO`
    - 해당 `ARENA` 식별자 (`size_t`)
- `TINY_POOL`
    - `TINY_POOL` 의 `HEAD` 를 가리킴
- `SMALL_POOL`
    - `SMALL_POOL`의 `HEAD` 를 가리킴

## POOL

- `malloc()` 에서 메모리를 보다 효율적으로 사용하고, 시스템 콜 없이 빠르게 메모리를 획득하기 위해서 자체적으로 `pool`을 가지고 있다.
- 42 subject 에서는 `TINY/SMALL/LARGE` 세 가지의 분류로 `malloc()`의 메모리 할당에 사용하는 풀을 별도로 사용하도록 요구하고 있다. 따라서, `TINY/SMALL/LARGE` 3 가지의 종류로 나누어 메모리 할당을 관리할 것이다.
- 32, 64 환경에 따라 각각 `size_t` 가 다르기에 앞으로 align 에 해당하는 바이트는 다음과 같이 표시한다.  `ALIGN_SIZE = sizeof(size_t) * 2`

### SIZE (ZONE)

- 세 가지 분류에 앞서, `TINY`, `SMALL`, `LARGE` 세 가지 크기 타입의 분류를 정해야한다.
- `TINY`, `SMALL` 의 경우 `malloc()`의 첫 call 과 함께 미리 메모리를 확보하고 해당 메모리에서 가용한 블록을 할당한다.
    - 각각의 Pool은 총 128개의 최대 블록이 있을 수 있는 공간만큼 할당한다.
        - `TINY` (in 64bit) 는 1024 * 128 바이트 만큼의 공간이 하나로 할당될 것이다.
    - Pool의 시작 주소에는 현재 Pool의 정보를 담는 메타 데이터가 위치한다.
        - Pool Align
        - Next Pool
- `LARGE` 의 경우 `mmap` 콜을 통해 새로운 `block` 을 생성한다. 이는 `TINY`, `SMALL` 과 달리 별도로 관리되는 블록이다.

### TINY

- `TINY` 의 경우 기본 `ALIGN_SIZE`바이트 단위의 메모리 블록을 생성하며, `≤ (1024 - 16) byte` 까지를 `TINY` 블록으로 정의한다.
    - `TINY` 블록의 경우 64비트 환경에서 `16, 32, 48, …, 1008 바이트`의 블록이 될 수 있다.

### SMALLc

- `SMALL` 의 경우 기본 `SMALL_ALIGN_SIZE = TINY_ALIGN_SIZE << 5`  바이트 단위(512byte in 64bit)의 메모리 블록을 생성하며, `≤ (16kb - 32 byte)` 까지를 `SMALL` 블록으로 정의한다.
    - `SMALL` 블록의 경우 64비트 환경에서 `1024, 1536, …, 15872` 바이트의 블록이 될 수 있다.

### LARGE

- `> 16KB`  의 메모리 블록에 대해서는 미리 할당한 메모리가 아닌, 각 새로운 할당에서 별도의 새로운 메모리 매핑을 통해서 할당이 된다. 이는 `TINY`, `SMALL` 과 다른 형태로 할당, 해제, 관리가 이루어진다.
- `LARGE` 의 경우, 이미 제공되는 풀에 의해서 관리되지 않는다. 시스템에 가용한 공간을 효율적으로 관리하기 위함이다. 메모리가 할당이 해제될 경우, 즉시 반환하는 방법을 차용할 것이다.

## BLOCK

- 메모리 블록에는 메타데이터가 포함된다. 이는 빠르게 해당 메모리 블록에 대한 정보를 획득하기 위함이다.
    - 이 방법에는 단점이 존재한다. 사용자가 임의로 해당 영역에 접근하여 정보를 수정한다면 메모리 관리 과정에서 오류가 일어난다.

### Structure

```
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
+  HEADER +                User Space                   +
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- 각 블록은 할당 상태에 관계없이, 공통 `sizeof(size_t)` 만큼의 공통 메타데이터 구간이 존재한다.
- **공통 메타데이터(HEADER)**는 다음과 같은 구조를 갖는다.
    - `Assigned flag` `(0b01)`
    - `Prev used flag (0b10)`
    - `Thread flag` `(0b100)`
        - bonus 이식을 위한 플래그이며, 해당 mandatory에서는 사용하지 않음.
    - `Block size` `(sizeof(size_t) - flag bit)`
- `Free block`의 경우 추가 정보를 더 가지고 있다. 이는 User Space에 저장한다.
    - `Prev Free Block Ptr`
    - `Next Free Block Ptr`
    - `sizeof(size_t) * 4` 만큼의 메타데이터 크기를 가진다.
- `TINY`의 경우 `ALIGN_SIZE`로 아주 작은 블록이기 때문에, 별도의 처리를 한다.
    - `Next Free Block Ptr`만 가지고 있음.
    - 이 경우, 메타데이터의 크기는 `ALIGN_SIZE` 와 일치함.
- `SMALL` 의 경우 효율적인 병합 과정을 위해, 블록의 끝에 해당 블록의 사이즈를 명시한다.

## BIN

- 해당 `malloc`에서 `POOL==BIN` 이다. `POOL`에서 사용하는 빈 메모리를 담는 역할을 하고 있으며, `POOL`은 `BIN`이라는 이름으로 구현될 것이다.
- `Free list`를 별도로 관리하기 위한 객체이다. 해당 `malloc()` 에서는 앞서 정의한 2가지의 `Pool`에 대해서 `Free list`를 유지하여 사용할 것이다.
- `Pool`을 유지하는 `TINY`,`SMALL` 의 경우 초기화 이후 각각의 `BIN` 에 해당 메모리가 Free 하기 때문에 통채로 `BIN` 에 노드가 할당된다.
- 할당 이후, 해제된 `block` 은 각 사이즈에 맞는 `BIN` 으로 다시 옮겨진다.

### TINY_BIN

- `TINY` 사이즈의 `block` 의 경우, `free block` 의 메타 데이터를 담을 수 있는 사이즈 자체의 한계가 있다. 따라서,  `single-linked-list`를 이용할 것이다.
- `TINY` 사이즈의 메모리 할당이 일어날 때, 해당 BIN 에서 요청 사이즈보다 크거나 같은 블록 중 가장 사이즈에 가까운 블록이 할당에 사용될 것이다.
    - 만약, 요청 사이즈보다 크다면, 해당 블록의 나머지는 split 되어 `TINY_BIN`에 재편입된다.
    - 맞는 사이즈를 찾지 못하는 경우는, `Pool` 이 꽉 찬 경우이다. 해당 경우, 새로운 `Pool`을 하나 더 생성하고, `TINY_BIN` 에 편입시킨 이후 다시 할당 과정을 수행한다.

### SMALL_BIN

- `TINY_BIN` 과 동작은 유사하나, 메타 데이터를 담을 수 있는 사이즈의 여유가 있기 때문에 `double-linked-list`를 이용한다.
- 메모리 할당의 동작은 `TINY_BIN` 과 동일하다.

## ASSIGNMENT

- 메모리의 할당은 다음과 같이 이루어진다.

### find zone

- 할당 요청을 받은 메모리 크기를 사이즈에 따라 분류한다. 앞서 정의한 `TINY`, `SMALL`, `LARGE` 타입 중 무엇인지를 결정하는 과정이다.
- 타입에 맞춰, 할당해야하는 `block`의 사이즈를 결정한다.
- `LARGE` 의 경우 바로 `set meta data` 과정으로 넘어간다.

### get block from bin

- `TINY`, `SMALL` 의 경우 `POOL` 에서 적절한 블록을 조회하여 반환해야한다.
- `POOL` 에 존재하는 `Free Block`은 `BIN` 에 존재한다. 따라서, 해당하는 `BIN` 의 `Free block` 을 순회한다.
- ‘적절한’ 블록은 `요청한 블록의 사이즈 + HEADER 크기` 보다 크거나 같은 블록 중 가장 작은 블록을 선정한다.
    - 두 가지 전략을 취해볼 수 있다. best fit, first fit.
    - `SMALL`의 경우에는 Best fit 을 사용하고, `TINY`의 경우에는 first fit을 사용한다.
        - `TINY` 를 조금 더 최적화하기 위해서는 sub category로 나누어 별도로 저장해두면 더 빠르게 접근하여 사용할 수 있다. (사이즈별 캐싱)

### set meta data

- 할당 받은 메모리 블록에 메타 데이터를 기록한다.
- 기록 이후, 메모리 블록에서 메타 데이터 이후의 공간(user space) 주소를 반환한다.

## FREE

- 메모리의 할당을 해제하는 과정은 다음과 같이 이루어진다.

### find memory

- 해당 주소값에 해당하는 메타 데이터를 조회하는 과정이다. 해당 과정에서 할당된 메모리의 사이즈를 획득하고, 어떤 방식으로 메모리를 할당해제할 지에 대해서 결정한다.
    - 이 때, 반드시 해당 메모리가 이미 free 되어있는 메모리인지 확인을 해야한다. (double free 문제)
- 메타 데이터의 저장방식을 고려할 때, `(size_t *)data - 1` 부분에 해당 정보를 획득할 수 있을 것이다.
- 만약, 할당된 메모리가 `LARGE` 사이즈라면, 즉시 `munmap` 을 통해서 해제하고, `free()` 과정을 종료한다.
- 그렇지 않다면, 아래의 프로세스를 진행한다.

### merge free space

- 할당 해제된 메모리 공간의 옆에 `free block`이 있다면 해당 공간을 하나의 `free block`으로 합치는 과정이다. 이는 메모리 파편화를 막기 위함이다. 병합이 끝난 후, 각자 크기에 맞는 `BIN` 으로 이동한다.
- 해당 과정은 `TINY` 의 경우, 되도록 일어나지 않는다. 새 페이지의 할당이 필요한 경우, `TINY_BIN` 을 순회하며, 연속된 `Free Block` 를 병합한다. 이는 `TINY` 의 경우 빠른 할당과 해제를 전제로 사용하기 때문이다. 이전 `block`의 정보를 담을만큼 메타 데이터의 여유가 없기 때문에, 병합 과정이 복잡하다.
- `SMALL` 의 경우 할당이 해제되면, 주변의 블록 정보를 조회하여 병합을 진행한다. 병합 과정에서는 `HEADER` 의 `PREV_INUSE` 와 끝에 존재하는 `이전 블록의 크기` 메타 데이터가 활용된다.

### link to bin

- `Free block` 을 적절한 `BIN` 으로 옮기는 과정이다.
- 가장 최근에 해제된 블록이 `BIN`의 맨 앞에 위치할 수 있도록 한다.
- 필요한 메타 데이터를 메모리 블록에 기록하는 과정도 포함된다.