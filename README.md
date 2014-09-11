cppcon14-tdp
============

Lightning talk at C++Con 2014 on test-driven performance.

This repo contains an example. The example is is a very simplified derivation of a depth-first traversal of a tree with each node containing a random number of children. In this simple example, processing a node is as simple as pushing the number of children it has into a container. The last element in the container is popped and a random number of elements added.

3 containers are tested: list, deque, vector. Can you guess which one is the fastest?

<hr />

|Number of Iterations | List   | Deque         | Vector |
| :-----------------: |:------ |:------------- |:------ |
|20000                | 1.72s  | <b>0.11s</b>  | 0.14s  |
|200000               | 17.34s | <b>1.10s</b>  | 1.12s  |
|2000000              | n/a    | <b>10.30s</b> | 13.54s |

List is a loser. This test could be made friendlier to list, but it will still be a loser. Let's ignore it.

There are many people who assume that vector is always a better solution than deque. Yikes! It is irresponsible to make such sweeping generalizations. Even though most of the time this is correct. There is an important area where deque has better performance, growing.

Vector needs to allocate an entirely new memory segment large enough to support the new size (and then some) and copy (read 'move') all the objects into it. This can be avoided if the maximum number of elements is known at the start. That is not always the case.

Deque only allocates a new slab. The object internally holds a reference to each slab. This means that iterating the container is slower, but this allocation process is significantly faster. For non-POD types for which std::move can be expensive this is a critical difference.

Note: This was compiled on MacOSX 10.9 with:
  Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)
  Target: x86_64-apple-darwin13.3.0
  Thread model: posix
