# chess-analysis-program
This repository contains a program for analysing positions on a chessboard. The task of the program is to determine which pieces are under attack and identify potential threats to them. For the convenience of entering the initial state of the game, the ability to load the positions of the pieces from a file is implemented.

Main features:

Data structure:<br>
-An efficient data structure for encoding the location of chess pieces has been developed.

Analysis algorithm:<br>
-An algorithm has been created that determines which pieces are under attack and determines from whom they are threatened.

Input from a file:<br>
-The ability to enter the initial location of the pieces from a file has been implemented for the convenience of the user.

In file you can write figures and their info in such form: [Color], [Figure], [Position].

You can use only two [Color]s: W(White) and B(Black).<br>
You can use only [Figure]s:<br>
P - Pawn;<br>
H - Knight;<br>
R - Rook;<br>
B - Bishop;<br>
Q - Queen;<br>
K - King;<br>
[Position] you can write as in classic chess, for example: A2, F6, G3 etc.

You can also use lowercase letters, and later the program will rewrite and make all lowercase letters capital when it sorts the information about the shapes by color (from W to B).

For example, here is a chessboard input file with information about the pieces:<br>
W H G5<br>
W Q C4<br>
W P E7<br>
B B E3<br>
B R E6<br>
B P B5<br>

And this is the result of executing the program using these inputs:<br>
<p align="center">
  <img src="https://github.com/Senior-Kedro/chess-analysis-program/assets/138604239/0b32e47c-4343-4f68-bb93-4d31a4ea6622)https://github.com/Senior-Kedro/chess-analysis-program/assets/138604239/0b32e47c-4343-4f68-bb93-4d31a4ea6622" alt="Your Image Description">
</p>
