# BufferAggregate
Buffer Aggregate  ADT is a technique to improve I/O performance in systems.  I/O systems face a lot of performance issues since there are a lot of copies happening across the software stack. This ADT uses immutable data buffers to stora all the data. There is only one physical copy. Thus it eliminates redundant data copying.

This implementation of Buffer Aggregate container is a linked list of buffers. Each buffer has an offset, size and a data buffer. The operations supported are to add a buffer, split a buffer, remove a buffer.


