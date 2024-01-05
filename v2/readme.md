# HOW TO USE?
`make` and create `run.sh` and `./run.sh executable`
## run.sh
- must replace `${dynamic libfile name}` to `libft_malloc${HOST_TYPE}.so`
### run.sh in MAC
```sh
export DYLD_LIBRARY_PATH=. 
export DYLD_INSERT_LIBRARIES="${dynamic libfile name}"
export DYLD_FORCE_FLAT_NAMESPACE=1 

### run.sh in LINUX
```sh
export LD_LIBRARY_PATH=.
export LD_PRELOAD="${dynamic libfile name}"
```

## V2
### Pre allocation (Warm cache)
- Cache (Pool) can be relatively slow on the first `malloc()` call if it is not prepared before program startup.
- The cache can be initialized via `__attribute__((constructor))`.
- Conversely, the `__attribute__((destructor))` rule can be used to return resources at program end.
- They are defined as `__CONSTRUCT__` `__DESTRUCT__` respectively.

### Metadata
- 

### Buddy allocator
-
