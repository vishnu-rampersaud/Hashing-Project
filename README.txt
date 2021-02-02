Student: Vishnu Rampersaud
Date: November, 05, 2019

All parts of this program were completed. (Parts a, b, and c)

You can compile all files by typing the command
 - "make all"

You can execute each file by using the following commands: 
 - create_and_test_hash
	- ./create_and_test_hash <words file name> <query words file name> 
 - spell_check
	- ./spell_check <document file> <dictionary file> 

Clarifications for Part A and B: 
The number of collissions and probes differ based on different computer architectures. 
My expected output is not the same as in the expected output file. 
I asked the professor about all of my expected outputs and he said that 
they were good, and I won't lose points because of it. 
One thing to note is that in linear probing, for the query: pnca, I got 
"not found in 8 probes". The number of probes seemed high, but the professor checked 
my code and said it was not a problem. 

Clarification for Double hashing: 
I used the hash function hash(x) = R - ( x % R ) as my second hash function. 
I used the R value: R = 37 