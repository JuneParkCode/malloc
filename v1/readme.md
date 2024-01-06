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