# Digital Design 1- Quine-McClusky Logic Minimization 

Designed to use the Quine-McClusky minimization method to optain the Prime Implicants and Essential Prime Implicants of any Boolean function (up to 20 bits) using
its minterms and don't care terms.

## Project Description

-This code is designed to take in a text file as input, process its contents (which include number of variables, minterms, and don't cares), and output the prime implicants and essential prime implicants. This code had been written as part of a university course, for a project, so the purpose of this code is to implement the Quine-McClusky Minimization method and understand exactly how it works.A few difficulties had faced the programmers while implementing the algorithm, and the main one was effectively working with 3D vectors and being able to navigate through them. Another was accounting for the minterms and don't care terms while creating the output for which minterms and don't cares were covered by which Prime Implicants, in addition to which minterms were not covered by Essential Prime Implicants. This project has been created by Basmala Abdelkader, MagdElDin Ahmed, and Mohamed Elshemy: AUC students. 

### Directions

First, you will need to create a text file that contains the number of variables, minterms, and don't care terms. How do you deliver this? Here are some guidelines:

- Create the following file: `input.txt`.

- Inside this file, the first line of the file should contain a single number that describes the number of variables in the function.

- The second line should contain a set of numbers that represent the minterms, separated by commas.

- The third and final line should contain the don't care terms (if any), also separated by commas.

#### Minimum Requirements

-A fully installed and running application for compilation of C++ code (i.e. Visual Studio, Xcode...).
