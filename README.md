## Custom Memory Allocator Lib
###### Created by Guilherme Teres

This Library was created to allow as much contiguous data blocks (from certain data types) as possible aiming a fast iterationg, which is ideal for **game engines**.

## Testing (Performance)

The performance of the code was tested compared to a traditional approach of allocating and freeing data in C++ (using `new` and `delete`).

For this, an array of pointers was created and, for each element a new memory space was required/allocated. Later on, the array was filled with the index some hundreds of times (to verify the iteration time) and, finally, each element was freed.

Each test was executed ten times and created an array of a hundred thousands elements.

### Results
The allocating and freeing time was considerally slower than the default approach but the iteration time was almost two times faster.

![](https://i.ibb.co/0rrTg43/image.png)
 
**Allocation Time**
```
Before: 0,00444598s
After:  0,00455874s
```

**Iteration Time**
```
Before: 0,00846252s
After:  0,00460063s
```

**Deallocation Time**
```
Before: 0,00260740s
After:  0,00514917s
```
  