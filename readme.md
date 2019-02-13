Write a C program that compress a file in FASTA file format to a binary file, and decompress the binary file back to the original file. This topic belongs to bioinformatics. In this project, you can practice basic file operations, bitwise operations, and command line arguments.

 

FASTA file format: https://en.wikipedia.org/wiki/FASTA_format

 

The program is run in command prompt. 

 

Suppose that the program name is project.

For compression, we run the program with option "-c"

 

>> project -c input_file compressed_file
 

input_file is the file name of the FASTA file containing the gene data, and compressed_file is the output binary file.

 

For decompression, we run the program with option "-d"

>> project -d compressed_file decompressed_file
It recovers the original file from the compressed file.

 

The size of the compressed file should be smaller than the original file, the smaller the better.

The decompressed file should be identical to the original file.

 

This is an individual project. Each student submits his/her source .c code to csci1140.ierg2080@gmail.com on or before the due date. The name of the source code is project_studentname_studentIDnumber.c. The source code will be compiled by either gcc or Visual Studio's cl. 

 

A sample file in FASTA format is uploaded to

http://www.piazza.com/class_profile/get_resource/j6vz04b1ump2qr/j9csc57bhbz4v2

 

See this post to see how to use command prompt and cl compiler in student's computing lab.

 

 

Grading policy:

 

If the source file cannot be compiled, then the project grade is zero.
You don't need to implement complicated compression algorithm such as LZ77. A simple idea of representing A, G, C and T by two bits can achieve a compression ratio of around 1/4. See for example the first video here.
The compression should be lossless, i.e., the decompressed file is exactly the same as the original file. In Windows command line, you can use the file compare command fc to check whether the two files are identical or not.
Explain briefly about your program by writing some comments at the beginning of your program. Also, write comments on the functions you define in the program, for instance, specify the inputs and outputs of the function. Readability is part of the grading criteria.
You can assume that a line in the FASTA file either
starts with the character > and contains some description after this, or
consists of capital letters. (It is not necessary that all of the letters are 'A', 'G', 'C' and 'T'. There may be some other characters such as 'N'.)
Please do not use any functions that are platform-specific, in order to guarantee portability. Hence, do not include any pre-compiled header file, or windows.h. 
You may discuss with your classmates, but any plagiarism detected beyond reasonable doubt will be reported to the department.
