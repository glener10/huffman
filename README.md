# huffman

 Huffman's algorithm works to rewrite bytes at a lower cost, totaling fewer bits per character,t he more repeated characters, the more efficient the algorithm will be.
 
# How to use
 
1- Enter the file directory.

2- Place the .txt file to be compressed or the .hx file to be uncompressed in it.

3- Run as follows: ./huffman c inputname.txt output.hx to compress OR ./huffman d inputname.hx output.txt to decompress.


# Example


__Negrito__Enter the directory and perform these steps to compress and decompress the sample file "input.txt":

*gcc huffman.c -o test

*./test c input.txt output.hx

__Negrito__At that moment, the output.hx file in compressed format will be created

__Negrito__and now


*./test d output.hx input2.txt

__Negrito__Uncompress the .hx file back to the original file

