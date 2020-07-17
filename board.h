#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cmath>
#include <exception>
#include <string>

struct Board
{
	void display()
	{
		int lineCounter = 0;
		for (const auto &line : board)
		{
			int charCounter = 0;

			if (lineCounter != 0)
				std::cout << "-----\n";

			for (const auto &character : line)
			{
				if (charCounter == 0)
					std::cout << character;
				else
					std::cout << "|" << character;
				++charCounter;
			}
			std::cout << "\n";
			++lineCounter;
		}
	}
	void input(char character, int position)
	{
		//Position starts at 1 in top left, goes to three from top left to top right, then drops down and repeats again.
		//aka:
		// 1 2 3
		// 4 5 6
		// 7 8 9

		if (character != 'x' && character != 'o')
			throw std::invalid_argument("Must be 'x' or 'o' as a character\n");

		std::size_t lineNo = std::floor((position-1)/3);
		std::size_t charNo = (position-1) % 3;

		if (!positionIsAlreadyFilled(position))
		{
			board[lineNo][charNo] = character;
			++moves;
		}
		else
		{
			std::cout << "Position is already filled\n";
		}
	}
	void input(int position)
	{
		char character;
		if (moves % 2 == 0)
			character = 'x';
		else
			character = 'o';
		input(character, position);
	}
	bool positionIsAlreadyFilled(int position)
	{
		std::size_t lineNo = std::floor((position - 1) / 3);
		std::size_t charNo = (position - 1) % 3;

		char character = board[lineNo][charNo];
		if (character != 'x' && character != 'o')
			return 0;
		else
			return 1;
	}
	bool isGameOver()
	{
		if (moves % 2 == 1)	//Odd moves will always be done by x
		{
			for (int i = 0; i != 3; ++i)	
			{
				if (board[i][0] == 'x' && board[i][1] == 'x' && board[i][2] == 'x') //Check horizontally
				{
					winner = 1;
					return 1;
				}
				if (board[0][i] == 'x' && board[1][i] == 'x' && board[2][i] == 'x') //Check vertically
				{
					winner = 1;
					return 1;
				}
			}
			//Check diagonally
			if (board[0][0] == 'x' && board[1][1] == 'x' && board[2][2] == 'x')
			{
				winner = 1;
				return 1;
			}
			if (board[2][0] == 'x' && board[1][1] == 'x' && board[0][2] == 'x')
			{
				winner = 1;
				return 1;
			}
		}
		if (moves % 2 == 0)	//Even moves will always be done by o
		{
			for (int i = 0; i != 3; ++i)
			{
				if (board[i][0] == 'o' && board[i][1] == 'o' && board[i][2] == 'o') //Check horizontally
				{
					winner = 2;
					return 1;
				}
				if (board[0][i] == 'o' && board[1][i] == 'o' && board[2][i] == 'o') //Check vertically
				{
					winner = 2;
					return 1;
				}
			}
			//Check diagonally
			if (board[0][0] == 'o' && board[1][1] == 'o' && board[2][2] == 'o')
			{
				winner = 2;
				return 1;
			}
			if (board[2][0] == 'o' && board[1][1] == 'o' && board[0][2] == 'o')
			{
				winner = 2;
				return 1;
			}
		}
		if (moves == 9)
		{
			winner = 3;
			return 1;
		}
		return 0;
	}
	std::string winnerIs()
	{
		//Syntax I imagine is "The winner is ______"
		if (winner == 0)
			return std::string("to be determined");
		else if (winner == 1)
			return std::string("player 1 (x)");
		else if (winner == 2)
			return std::string("player 2 (o)");
		else if (winner == 3)
			return std::string("no one");

	}
private:
	std::array<std::array<char, 3>, 3> board =
	{{  {{' ', ' ', ' '}},
		{{' ', ' ', ' '}},
		{{' ', ' ', ' '}}	}};
	int moves = 0;
	int winner = 0;
	//winner == 0 is inprogress
	//winner == 1 is player 1 (x)
	//winner == 2 is player 2 (o)
	//winner == 3 is draw
};



#endif