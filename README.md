# Regular-Expression-to-NFA Calculator

This program takes a text file of regular expressions in [postfix](https://www.geeksforgeeks.org/prefix-postfix-conversion) form and converts them into an NFA transition table 
## How to run
Compile the code by typing
```
make
```
into the terminal

## Usage
```
./re-nfa textFile.txt
```
Where textFile.txt is a text file filled with regular expressions in postfix form separated by a new line 

### Contents of textFile.txt
```bash
a*b|
ab|*b&
ab|*a&b&a*&*ab|&ab|&
```
### Output after running the program through textFile.txt
```bash
RE: a*b|
Start: q6 
Accept: q7 
(q1, a) -> q2
(q2, E) -> q3
(q3, E) -> q1
(q3, E) -> q7
(q4, b) -> q5
(q5, E) -> q7
(q6, E) -> q3
(q6, E) -> q4

RE: ab|*b&
Start: q7 
Accept: q9 
(q1, a) -> q2
(q2, E) -> q6
(q3, b) -> q4
(q4, E) -> q6
(q5, E) -> q1
(q5, E) -> q3
(q6, E) -> q7
(q7, E) -> q5
(q7, E) -> q8
(q8, b) -> q9

RE: ab|*a&b&a*&*ab|&ab|&
Start: q15 
Accept: q27 
(q1, a) -> q2
(q2, E) -> q6
(q3, b) -> q4
(q4, E) -> q6
(q5, E) -> q1
(q5, E) -> q3
(q6, E) -> q7
(q7, E) -> q5
(q7, E) -> q8
(q8, a) -> q9
(q9, E) -> q10
(q10, b) -> q11
(q11, E) -> q14
(q12, a) -> q13
(q13, E) -> q14
(q14, E) -> q12
(q14, E) -> q15
(q15, E) -> q7
(q15, E) -> q20
(q16, a) -> q17
(q17, E) -> q21
(q18, b) -> q19
(q19, E) -> q21
(q20, E) -> q16
(q20, E) -> q18
(q21, E) -> q26
(q22, a) -> q23
(q23, E) -> q27
(q24, b) -> q25
(q25, E) -> q27
(q26, E) -> q22
(q26, E) -> q24
```
