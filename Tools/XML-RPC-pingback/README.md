# XML-RPC attack script

Please note: this is old, crappy, code.

A complete refactor and different take on a traditional attack script for exploiting XML-RPC pingback(s) on WordPress. I've completely revamped the way that this is traditionally done, the old code/base used for attack scripts was horrible, contained memory leaks and was just overall disgusting. I'll release a guide on how to adapt this code for other attack vectors.

How to build?
```
gcc *.c -D DEBUG -pthread -o xml
```
