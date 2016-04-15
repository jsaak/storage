# Storage
```
Two double linked lists on one static heap.
The first is called data, the other is called empty.
At first data does not have any elements, empty has them all.
When you insert a data element, it gets removed from empty list, and vice versa.

The library does not allocate memory.
So it can be used in restrained environments such as a microcontroller.

There is no safety net.
For example when the queue is full inserting will silently fail.

Currently these functions are implemented:

storage_init()
storage_push()
storage_insert_front() 
storage_remove()

inserting between two nodes is not implemented, but should not be very hard
```

# Example code
[test.c](test.c)
You can inspect the library internals by running it
```
$ make
$ ./test
$ make clean
```

# Licensing
[GPL v3.0](gpl.txt)
