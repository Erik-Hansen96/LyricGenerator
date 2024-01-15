# Lyrics Generator Project

This C++ project generates lyrics based on an inputted text file. It uses a graph structure to represent relationships between words and offers functionalities to explore the structure and generate lyrics!

##Technlogoies:
  - C++: Generate lyrics using Markov chain
  - Bridges: A library to visualize data

## Features
- **Graph Display**: Show all the vertices and their edges.
- **Lyrics Generation**: Generate new lyrics based on the inputted text file.
- **Graph Visualization with Bridges**: Visualize the relationships between lyrics on [Bridges](https://bridges-cs.herokuapp.com).

- ## Example
```bash

Please enter a text file to open:
beatles.txt
```
Choose an option
```bash
1. Print Graph and Quit
2. Generate Random Lyrics
3. View on BRIDGES
1
```
All vertices will dislplay. Here is a snippet using the entirety of The Beatles lyrics:
```bash
Vertex HURRICAN - Count: 1 TEW: 0 Start Count: 0 End Count: 1 Comma Count: 0 Edges:
Vertex JUBILEE - Count: 1 TEW: 0 Start Count: 0 End Count: 1 Comma Count: 0 Edges:
Vertex JOKEY - Count: 1 TEW: 1 Start Count: 0 End Count: 0 Comma Count: 0 Edges:
    FOLKS: 1
Vertex FOLKS - Count: 2 TEW: 2 Start Count: 0 End Count: 0 Comma Count: 0 Edges:
    THEY: 1
    DANCING: 1
Vertex JAMBOREE - Count: 1 TEW: 0 Start Count: 0 End Count: 1 Comma Count: 0 Edges:
Vertex BREW - Count: 1 TEW: 1 Start Count: 0 End Count: 0 Comma Count: 0 Edges:
    FROM: 1
Vertex DANCING - Count: 1 TEW: 1 Start Count: 0 End Count: 0 Comma Count: 0 Edges:
    GOT: 1
```
Option 2:
```bash
How many sentences do you wish to make?
10
How many sentences do you wish to make?
12345678
```
Output:
```bash
I'm down, don't need you still need me apart.
Jai guru deva oh, yeah, yeah, yeah.
Getting better, life there was a tan from the party, party.
Hello.
I'm gonna have always shouts out on any jobber got a week.
I'm right.
One sweet dreams by stealing the queen.
Mr h will return, yes, you how the light, that you, say i was haven't sent.
I've got to day to do.
That's new.
```
