# export LD_LIBRARY_PATH=.
# export LD_PRELOAD="libft_malloc_x86_64_Linux.so"
export DYLD_LIBRARY_PATH=. 
export DYLD_INSERT_LIBRARIES="libft_malloc_x86_64_Darwin.so"
$@
