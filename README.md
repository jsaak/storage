# Storage
```
Two double linked lists on one static heap.
The first is called data, the other is called empty.
Initially data does not have any elements, empty has them all.
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
counter: 00 fd: 0x000000  ld: 0x000000  fe: 0x402340  le: 0x4023d6
  loc: 0x402340  prev: 0x000000  next: 0x402372  data: 
  loc: 0x402372  prev: 0x402340  next: 0x4023a4  data: 
  loc: 0x4023a4  prev: 0x402372  next: 0x4023d6  data: 
  loc: 0x4023d6  prev: 0x4023a4  next: 0x000000  data: 
counter: 01 fd: 0x402340  ld: 0x402340  fe: 0x402372  le: 0x4023d6
  loc: 0x402340  prev: 0x000000  next: 0x000000  data: Hello 1
  loc: 0x402372  prev: 0x000000  next: 0x4023a4  data: 
  loc: 0x4023a4  prev: 0x402372  next: 0x4023d6  data: 
  loc: 0x4023d6  prev: 0x4023a4  next: 0x000000  data: 
...

$ make clean
```

# Licensing
[GPL v3.0](LICENSE)
