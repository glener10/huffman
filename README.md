# huffman

 Huffman's algorithm works to rewrite bytes at a lower cost, totaling fewer bits per character,t he more repeated characters, the more efficient the algorithm will be.
 
# How to use
 
1- Enter the file directory.

2- Place the .txt file to be compressed or the .hx file to be uncompressed in it.

3- Run as follows: ./huffman c inputname.txt output.hx to compress OR ./huffman d inputname.hx output.txt to decompress.


# Example


__Enter the directory and perform these steps to compress and decompress the sample file "input.txt":__

- __gcc huffman.c -o test__

- __./test c input.txt output.hx__

__At that moment, the output.hx file in compressed format will be created__

__and now__


- __./test d output.hx input2.txt__

__Uncompress the .hx file back to the original file__

