<h1 align="center">Huffman</h1>

<p align="center"> 🚀 Huffman's algorithm works to rewrite bytes at a lower cost, totaling fewer bits per character,t he more repeated characters, the more efficient the algorithm will be.</p>

<img src="/img/execution.jpg" alt="example"/>

Table of Contents

===================

<!--ts-->

- [Environment](#Environment)
- [Demo](#Demo)

<!--te-->

===================

# Environment

Execution environment used and tested:

**SO**: Ubuntu 20.04 **Kernel**: 5.8.0-63-generic

# Demo

1- Enter the file directory.

2- Place the .txt file to be compressed or the .hx file to be uncompressed in it.

3- Compiling and executing:

```bash
# To compiling
$ gcc huffman.c -o huffman
```

```bash
# To compress
$ ./huffman c inputname.txt output.hx

# In Windows
$ huffman.exe c inputname.txt output.hx
```

```bash
# To decompress
$ ./huffman d inputname.hx output.txt

# In Windows
$ huffman.exe d output.hx inputname.txt
```
