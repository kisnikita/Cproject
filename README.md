# Project 2-working with data structures

You can work on a project in a team of 3-5 members. In Week 1.-5.11. get acquainted with the assignment of the project and form a team in which you want to work on the project. Team members must belong to the same group of laboratory exercises.

# Motivation of the project

In the first project we tried simple work with text strings and basic cycles. In this project, we will combine knowledge of programming and Discrete Mathematics. The project includes input processing, dynamic allocation and algorithms for passes in Data Structures.

# Project description

The goal of the project is to create a program that will implement basic mathematical operations on sets and binary relationships. The input of the program will be text data representing sets and Sessions, and specifying operations. The program will print the result of the processing to the standard output.

# Detailed specifications

Implement the Program in the source file " setcal.c " (Set Calculator). The input data will be read from a file whose name will be specified by a command line argument. The Program performs the operations specified in the file line by line in one pass. The program prints the result of processing each line to the standard output (that is, the number of lines in the output corresponds to the number of lines in the input file).

# Translate and upload source file

Surrender: surrender the source file " setcal.c " through the information system.

Translation: translate the Program with the following arguments

<pre>$ gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal</pre>

# Startup Syntax

The Program runs as follows: (./ setcal stands for the location and name of the program):

<pre>./setcal FILE</pre>

# Input file format

The text file consists of three consecutive parts:
- Definition of the universe-one line starting with " U " and continuing with a space separated by elements,
- Definition of sets and relationships-one or more lines starting with " S " or " R "and continuing with a space separated by elements (the line starting with" S "indicates the definition of the set," R " is used for the definition of the relationship),
- Commands over sets and Sessions-one or more lines starting with " C " and continuing with the command identifier.
# Universe

Elements of the universe can be</s><span style= "color: red">are</span> strings containing lowercase and uppercase letters of the English alphabet. The length of the string is a maximum of 30 characters. Universe elements must not contain command identifiers (see below) and the keywords true and false. All elements in sets and relations must belong to the universe. Example:
<pre>U Apple Lemon Orange Banana Peach</pre>

# Sets

Each set is defined on one line by a space separated by elements from the universe. The identifier of the set is the number of the line on which the set is defined (since the first line of the set is the universe, so the identifiers of the sets begin with the number 2). Set identifiers are used in operations (see below). Example of a set definition:
<pre>with Apple Banana Peach</pre>

# Relations

Each session is defined by an enumeration of pairs. The pair is bounded by parentheses, the first and second elements of the pair are separated by a space. Individual pairs are separated by a space. Example:
<pre>R (Apple Banana) (Apple Peach) (Apple Apple)</pre>


# Commands

Each command is defined on one line, begins with the command identifier, and the command arguments are separated by a space (both from the identifier and among themselves). The arguments of the command are numeric identifiers of sets and relations (positive integers, the number 1 identifies the set of the universe). Example:
<pre>C minus 1 2</pre>

# Commands over sets

The command works on sets, and its result is either a set (in which case it prints the set in the same format as expected in the input file, i.e. it starts with an " S " and continues with a space separated by elements) or a truth value (in which case it prints true or false on a separate line) or a natural number (which prints on a separate line).

* <code>empty and</code> - prints true or false depending on whether the set defined on line A is empty or non-empty.
* <code>card A</code> - prints the number of elements in the set A (defined on line A).
* <code>complement A</code> - prints the complement of set A.
* <code>union a B</code> - prints Union of Sets A and B.
* <code>intersect a B</code> - prints the intersection of Sets A and B.
* <code>minus A B</code> - prints the difference of Sets A \ B.
* <code>subseteq and B</code> - prints true or false depending on whether Set A is a subset of Set B.
* <code>subset A B</code> - prints true or false if Set A is a proper subset of Set B.
* <code>equals A B</code> - prints true or false if the sets are equal.

=== Commands over Sessions ===

The command works on Sessions, and its result is either a truth value (prints true or false), or a set (prints a set in the format as in the input file).

* <code>reflexive R</code> - prints true or false if the session is reflexive.
* <code>symmetric R</code> - prints true or false if the session is symmetric.
* <code>antisymmetric R</code> - prints true or false if the session is antisymmetric.
* <code>transitive R</code> - prints true or false if the session is transitive.
* <code>function R</code> - prints true or false If session R is a function.
* <code>domain R</code> - prints the definition scope of the R function (can also be applied to Sessions-the first elements of pairs).
* <code>codomain R</code> - prints the range of values of the function R (can also be applied to relations-the second elements of pairs).
* <code>injectable R <span style="color:red" >A B</span></code> - prints true or false if R is injectable. <span style= "color:red">A and B are sets; a&#8712; A, b & #8712; B, (a, b)&#8712; R.</span>
* <code>surjective R <span style= "color:red">A B< / span></code> - prints true or false if R is surjective. <span style= "color:red">A and B are sets; a&#8712; A, b & #8712; B, (a, b)&#8712; R.</span>
* <code>bijective R <span style= "color:red">A B< / span></code> - prints true or false if R is bijective. <span style= "color:red">A and B are sets; a&#8712; A, b & #8712; B, (a, b)&#8712; R.</span>

=== Implementation details ===

* The maximum supported number of lines is 1000.
* The order of the elements in the set and in the session does not matter in the output.
* All elements of sets and in relations must belong to the universe. If an element in a set or a pair in a session repeats, this is an error.

== Examples of use  ==

<pre>$ cat sets.txt
U a b c x y z
S a b c x
S x y z
C intersect 2 3
C minus 2 3</pre>
<pre>$ ./setcal sets.txt
U a b c x y z
S a b c x
S x y z
S x
S a b c</pre>

<pre>$ cat rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
C codomain 2
C function 3</pre>
<pre>$ ./setcal rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
S boy girl
true</pre>