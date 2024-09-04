# This directory contains Assessment 2 codes for Compiler Design Lab

## Experiment 3:
- Exp3 part 1 : Identify the tokens from simple statement as input stored in a linear array 
- Exp3 part 2 : Identify the tokens from small program (not exceeding 5 lines)  as input stored in a text file [(input.txt)](https://github.com/Blacksword66/Compiler-design/blob/main/compiler-design-assess2/input.txt)
- Exp3 part 3 : Identify the tokens from small program (not exceeding 5 lines)  as input get it from the user [(input.txt)](https://github.com/Blacksword66/Compiler-design/blob/main/compiler-design-assess2/input.txt) and store it  in a text file [(tokens.txt)](https://github.com/Blacksword66/Compiler-design/blob/main/compiler-design-assess2/tokens.txt)

## Experiment 4:
- Construct a lexical analyser using LEX tool. Input: [program.txt](https://github.com/Blacksword66/Compiler-design/blob/main/compiler-design-assess2/program.txt)

## How to execute (for linux and macOS only):
#### Prerequisite:
Please make sure that the input files and output files are in the same directory as the lex code 

Once made sure, type the following in the terminal:

```shell
lex file.l 
gcc lex.yy.c 
./a.out

```
