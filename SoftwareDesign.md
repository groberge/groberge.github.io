## Software Design and Engineering

**Original artifact**

[Original C++ Farkle](https://github.com/groberge/groberge.github.io/tree/main/Original_Farkle)

**Enhanced artifact**

[Farkle in Python](https://github.com/groberge/groberge.github.io/blob/main/Farkle.py)

	
The artifact that was chosen for this enhancement is a game called Farkle that was originally written for the IT-312 course that was taken in 2017.  Through this enhancement, the program will be translated from the current C++ version to a new Python version of the game. The original program includes a couple user-defined classes called Player and Gameplay as well as a struct datatype called Dice and some of the standard C++ libraries.  Within the Player class, there are attributes to hold the player’s name, score and whether they are the winning player along with the appropriate getter and setter functions.  The Gameplay class creates the struct for the dice, which holds attributes such as the value, number, scoring value and many Booleans such as whether the dice is a scoring dice or has been saved, matched or calculated.  This class also creates many methods for the functionality of the game which will be referenced throughout this page.  
  
The main application instantiates the Gameplay class, an alias called PlayerVector representing a vector of the players and initializes the combination of the srand and time functions to ensure random values are achieved during the rolling of the dice.  It then displays the rules using a function from the Gameplay class that displays a file to the screen and runs another Gameplay function called numberPlayers which prompts the user for the number of players who will be playing.  

After requesting each player’s name from the users and adding it to the vector, the game begins by displaying the current player and initiating the takeTurn function for that player.  This function uses a reference to the current player and prompts the user to roll or quit and then performs a few checks according to the rules of Farkle to see if a roll of the dice should be enforced.  If the player chooses to quit, then the score for the roll is returned to the program.  If they choose to roll, then another alias is created called DiceVector, representing a vector of dice, and the program loops through the vector performing the diceRoll function on each instance of Dice by returning a random value between one and six, setting the value and displaying each one to the screen.  Once this is completed, the scoringDice function is used to see if there are any sets of three matching values using the threeOfAKind function, or if there are ones or fives in the vector as those are all worth points in the game.  If there are any dice that are worth points, the user is prompted to input which Dice they’d like to save, and the appropriate points are calculated using the calculateRollScore function.  Any Dice that are saved are removed from the number of Dice for the next roll and the play continues until the player has no scoring dice on a given roll or they choose to quit rolling.  

At the end of each turn, a function is used to check if the current player’s score is over 10,000 and if so, the bit is set to mark them as the winning player.  Once this occurs, the game loops through the other players and allows them to take a final turn until this player is reached again.  At this point, the player with the highest score is calculated using the calculateWinner function and the winner is congratulated.

This program was chosen because it allows the opportunity to take a fully functioning program that was written in one language and transfer it into another.  There were parts of the artifact that used C++ objects that don’t exist in Python such as some data structures (structs) and case statements.  This presented a good opportunity to do some research to find suitable replacements for these objects and be able to use them in the new version of the program.  By importing Counter from the collections module, the time and complexity of the threeOfAKind() function will be improved from the original C++ version and that functionality has been included in the scoring() function in Python.  Additionally, the dataclass data structure has been used for the players rather than a class.  This data structure creates many of the dunder methods automatically (Python Software Foundation, n.d.), helping to limit manual creation of these functions.

These improvements along with the complete transfer of the program from C++ to Python helped to showcase my skills in using well-founded and innovative techniques, skill, and tools to implement solutions that deliver value and accomplish goals.  They also helped display skills in evaluating computing solutions to solve a given problem using algorithmic principles while managing the trade-offs involved in the choices.  Throughout the process, GitHub was used to track the versions of this project at groberge/Farkle: Version of Farkle written in Python for SNHU Capstone (github.com) which would allow collaborative development when opened to other programmers.  With these skills, the course objectives that were planned to meet with this milestone have been met.
  
The process of transferring this program into another language was an interesting project.  Not only does a developer sometimes have to find different ways in each language to result in the same outcome, but they also need to compare the different options within the same language to find the best and most efficient choice.  It was also found that different languages include modules or libraries to perform functions that may need to be written out in another language, such as the most_common method that was used from the collections module in Python.  There weren’t any similar options in C++ that would perform the same function, and this was a huge help when writing this program.  The only challenges that were faced dealt with learning the modules that hadn’t previously been used, but the documentation and online examples were very helpful in allowing the new functionality to be learned.


**Resources**

Python Software Foundation. (n.d.). _dataclasses - Data Classes - Python 3.9.7 documentation._ Python. &nbsp;&nbsp;&nbsp;&nbsp;https://docs.python.org/3/library/dataclasses.html
 


