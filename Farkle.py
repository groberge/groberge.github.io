from dataclasses import dataclass
from collections import Counter
import random


@dataclass
class Player:
    """Class for holding the player attributes"""
    player_name: str
    reached_10000: bool = False
    player_score: int = 0


def get_num_players():
    """Prompts for the number of players and the name of each player

    :param:
    None
    :return:
    players (list): list of the players in the game
    """

    try:
        num_players = int(input("How many players will be playing?\n"))
    except ValueError:
        num_players = int(input("Invalid input.  Please choose 2 - 5 players.\n How many players will be playing?\n"))

    while num_players < 2 or num_players > 5:
        num_players = int(input("Invalid input.  Please choose 2 - 5 players.\n How many players will be playing?\n"))

    players = []

    for index in range(num_players):
        name = input("What is player" + str(index + 1) + "'s name?\n")
        players.append(Player(name))

    return players


def roll_dice(number_of_dice):
    """Rolls the dice and adds the values to a counter

    Random is used to generate a random value between 1 and 6 for each die
    Counter is then used to count the number of dice with the same value and store each element
    as a dictionary key, which is then returned to the program

    :param:
    number_of_dice (int): the number of dice being rolled
    :return:
    dice (counter): counter of dice with the values that were rolled
    """

    dice = Counter([random.randint(1, 6) for die in range(0, number_of_dice)])
    return dice


def take_turn(player):
    """ Takes a player and processes that player's turn for them

    Player is prompted whether to continue rolling or quit based on the rules of Farkle
    Uses roll_dice() and scoring() functions and displays current roll score
    and current game score for each roll
    Calculates the score for the player's complete turn

    :param:
    player (Player): the current player taking their turn
    :return:
    this_turn_score (int): the score for this turn
    """

    turn_over = False
    roll_over = False
    num_dice = 6
    this_turn_score = 0

    while not turn_over:

        roll = input("Press r to keep rolling, press q to quit\n")

        # If all dice have been saved or their score is less than 1000, player must re-roll
        if num_dice == 0:
            # If current player has already won, no need to force roll
            if player.player_score >= 10000:
                if player == calculate_winner(game_players):
                    roll = 'q'
            else:
                num_dice = 6
                print("\nAll dice have been saved.  You must keep rolling")
                roll = 'r'
        elif roll == 'q' and (player.player_score + this_turn_score < 1000):
            print("\nScore is less than 1000.  Must continue rolling.")
            roll = 'r'

        if roll == 'q':
            print("Your turn is over\n")
            return this_turn_score
        elif roll == 'r' and not roll_over:
            dice = roll_dice(num_dice)
            roll_score, num_dice = scoring(dice, num_dice)
            if not roll_score:
                roll_over = True
                turn_over = True
                this_turn_score = 0
            else:
                this_turn_score += roll_score
                print("Your current roll score is " + str(this_turn_score))
                print("If you stopped now, your total score would be " + str(player.player_score + this_turn_score)
                      + "\n")

    return this_turn_score


def scoring(dice, num_dice):
    """ Calculates score for a roll and tracks number of dice left to roll

    Uses the most_common() method to find any triplets and also looks for the number of dice with a value of 1 or 5
    for other scoring values.
    User is prompted whether to save each scoring dice and score is calculated based on input
    Number of dice remaining is decremented as dice are saved.

    :param:
    dice (counter): counter of dice to evaluate
    num_dice (int): number of dice to evaluate
    :return:
    score (int): value of the score for this roll
    num_dice (int): number of dice remaining for next roll
    """

    triplet = 0
    double_triplet = 0  # this is for the case of six of the same value
    second_triplet = 0  # for the case of two different triplets
    # Scoring values
    ones = 0
    fives = 0

    scoring_dice = False
    score = 0

    for die in range(7):
        if dice[die] > 0:
            print("You have " + str(dice[die]) + " " + str(die) + "'s")
    print("\n")

    print("Scoring dice:\n")

    # This uses the most_common() function of collections to avoid going through all possible dice values
    for number, amount in dice.most_common():
        if amount == 6:
            print("You have two triplets of " + str(number) + "s")
            double_triplet = number
            scoring_dice = True
        elif amount >= 3:
            print("You have a triplet of " + str(number) + "s")

            # We only want three in this triplet but other scoring dice of the same value must be scored
            if amount > 3:
                if number == 1:
                    ones = amount - 3
                    print("You have " + str(ones) + " additional " + str(number) + "s")
                if number == 5:
                    fives = amount - 3
                    print("You have " + str(fives) + " additional " + str(number) + "s")

            # We don't want to recount the triplet if they are 1s or 5s
            del dice[number]

            # Check if there are two different triplets
            if triplet == 0:
                triplet = number
            else:
                second_triplet = number
            scoring_dice = True
        elif number == 1:
            ones = amount
            print("You have " + str(ones) + " " + str(number) + "'s")
            scoring_dice = True
        elif number == 5:
            fives = amount
            print("You have " + str(fives) + " " + str(number) + "'s")
            scoring_dice = True

    if not scoring_dice:
        print("You have no scoring dice.  Your turn is over\n")
        score = 0
        return score, num_dice
    else:
        # No good reason not to save two triplets
        if double_triplet:
            score += (2 * triplet_score(double_triplet))
            print("\nSaving both triplets")
            num_dice = 0
        # Prompt player which dice they'd like to save
        else:
            if triplet:
                keep_dice = input("\nWould you like to keep your triplet of " + str(triplet) + "s? y or n\n")
                while keep_dice not in ('y', 'n'):
                    keep_dice = input("Please type either y or n.  Would you like to keep your triplet of "
                                      + str(triplet) + "s? y or n\n")
                if keep_dice == 'y':
                    score += triplet_score(triplet)
                    num_dice -= 3
                    print("You now have " + str(num_dice) + " dice remaining\n")

                if second_triplet:
                    keep_dice = input("\nWould you like to keep your triplet of " + str(second_triplet) + "s? y or n\n")
                    while keep_dice not in ('y', 'n'):
                        keep_dice = input("Please type either y or n.  Would you like to keep your triplet of "
                                          + str(second_triplet) + "s? y or n\n")
                    if keep_dice == 'y':
                        score += triplet_score(second_triplet)
                        num_dice -= 3
                        print("You now have " + str(num_dice) + " dice remaining\n")
            if ones:
                keep_dice = input("\nWould you like to keep your 1s? y or n\n")
                while keep_dice not in ('y', 'n'):
                    keep_dice = input("Please type either y or n. Would you like to keep your 1s? y or n\n")
                if keep_dice == 'y':
                    score += ones * 100
                    num_dice -= ones
                    print("You now have " + str(num_dice) + " dice remaining\n")
            if fives:
                keep_dice = input("\nWould you like to keep your 5s? y or n\n")
                while keep_dice not in ('y', 'n'):
                    keep_dice = input("Please type either y or n.  Would you like to keep your 5s? y or n\n")
                if keep_dice == 'y':
                    score += fives * 50
                    num_dice -= fives
                    print("You now have " + str(num_dice) + " dice remaining\n")

    return score, num_dice


def triplet_score(triplet):
    """ Calculates the scoring value of a triplet of dice

    Score is calculated based on the rules of Farkle where three 1s equals 1000 points.
    Any other triplet equals 100 * number

    :param:
    triplet (int): number of the die with three of a kind
    :return:
    triplet_value (int): scoring value of the three of a kind
    """

    if triplet == 1:
        triplet_value = 1000
    else:
        triplet_value = triplet * 100
    return triplet_value


def calculate_winner(players):
    """ Finds the winner from a list of players

    Iterates through a list of players to find the one with the highest score

    :param:
    players (list): list of players to evaluate
    :return:
    none
    """

    winner = players[0]

    for player in players:
        if player.player_score > winner.player_score:
            winner = player
    print("Congratulations!! " + winner.player_name + " is the winner!!!")

    return 0


if __name__ == '__main__':
    game_over = False

    # Get the number and list of players
    game_players = get_num_players()

    player_number = 0

    while not game_over:
        # If the current player has reached 10000, then all other players have played their final turn
        if not game_players[player_number].reached_10000:
            print("\n" + game_players[player_number].player_name + "'s turn\n")
            turn_score = take_turn(game_players[player_number])

            # Add the current turn's score to the players game score
            game_players[player_number].player_score = game_players[player_number].player_score + turn_score

            for i in range(len(game_players)):
                print(game_players[i].player_name + " has " + str(game_players[i].player_score)
                      + " points")

            if game_players[player_number].player_score >= 10000 and not game_players[player_number].reached_10000:
                game_players[player_number].reached_10000 = True
                reached_10000 = True
                print(game_players[player_number].player_name + " has reached 10000. All other players take their "
                                                                "final turn")
            player_number += 1

            if player_number > len(game_players) - 1:
                player_number = 0
        else:
            game_over = True

    # All players have had their final turn after a player reached 10000 points so calculate winner
    game_winner = calculate_winner(game_players)
