#include <chrono>
#include <cstddef>
#include <format>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <vector>

using gridT = std::vector<std::vector<char>>;

const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937   randEngine{seed};

constexpr char emptyChar = ' ';

constexpr std::pair<size_t, size_t> directions[8]{
{1, -1},
{1, 0},
{1, 1},
{0, -1},
{0, 1},
{-1, -1},
{-1, 0},
{-1, 1}};

void printHelp()
{
	return;
}

void printGrid(gridT& grid)
{
	std::cout << "\033[4m";
	for (std::vector<char> row : grid)
	{
		for (char character : row)
		{
			std::cout << '|' << character;
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "\033[0m";
}

void printGridStats(gridT& grid)
{
	std::map<char, size_t> stats{};

	for (std::vector<char> row : grid)
	{
		for (char character : row)
		{
			stats[character] += 1;
		}
	}

	for (const auto& [character, amount] : stats)
	{
		std::cout << character << ": " << amount << std::endl;
	}
}

template<typename T>
T readTypedInput(std::string descString, T defaultReturnVal)
{
	T    inputVar;
	bool goodInput{true};

	std::ios_base::iostate temp;

	do
	{


		if (!goodInput)
		{
			std::cout << temp << "\nInvalid input. Try again" << std::endl;
		}

		std::cout << descString;

		std::cin >> inputVar;

		temp = std::cin.rdstate();

		goodInput = std::cin.good();
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (!goodInput);

	return inputVar;
}

void chainCharacters(gridT& grid, std::pair<size_t, size_t> startingPosition, char characterToFill)
{

	size_t tempX, tempY;
	size_t maxX = grid[0].size() - 1;
	size_t maxY = grid.size() - 1;

	std::pair<size_t, size_t>              currentPosition;
	std::vector<std::pair<size_t, size_t>> positionsToTry;
	positionsToTry.push_back(startingPosition);

	while (!positionsToTry.empty())
	{
		currentPosition = positionsToTry.back();
		positionsToTry.pop_back();
		for (std::pair<size_t, size_t> direction : directions)
		{
			tempX = currentPosition.first + direction.first;
			tempY = currentPosition.second + direction.second;

			if (tempX < 0 || tempX > maxX || tempY < 0 || tempY > maxY)
			{
				continue;
			}

			if (grid[tempY][tempX] == emptyChar || grid[tempY][tempX] == characterToFill)
			{
				continue;
			}

			tempX += direction.first;
			tempY += direction.second;

			if (tempX < 0 || tempX > maxX || tempY < 0 || tempY > maxY)
			{
				continue;
			}

			if (grid[tempY][tempX] == emptyChar)
			{
				grid[tempY][tempX] = characterToFill;
				positionsToTry.push_back({tempX, tempY});
			}
		}
	}
}

std::pair<size_t, size_t> getFirstEmptySlot(gridT& grid)
{
	size_t x{0};
	size_t y{0};
	size_t tempx{0};

	while (y < grid.size() && grid[y][x] != emptyChar)
	{
		tempx = x + 1;
		if (tempx == grid[0].size())
		{
			x = 0;
			y = y + 1;
		}
		else
		{
			x = x + 1;
		}
	}

	return {x, y};
}

void sakusAlgorithm(gridT& grid, std::string word)
{
	size_t                                gridHeight         = grid.size();
	size_t                                gridWidth          = grid[0].size();
	size_t                                gridSlotAmount     = gridWidth * gridHeight;
	size_t                                randomLetterAmount = gridSlotAmount / word.size();
	std::uniform_int_distribution<size_t> distribution{0, gridSlotAmount - 1};
	char                                  randomFillChar = word[1];

	size_t randomIndex = distribution(randEngine);

	size_t x = randomIndex % gridWidth;
	size_t y = randomIndex / gridWidth;

	while (randomLetterAmount > 0)
	{
		if (grid[y][x] == randomFillChar)
		{
			randomIndex = distribution(randEngine);

			x = randomIndex % gridWidth;
			y = randomIndex / gridWidth;
			continue;
		}
		grid[y][x] = randomFillChar;
		randomLetterAmount--;
	}

	word.erase(1, 1);

	std::pair<size_t, size_t> startingPosition{getFirstEmptySlot(grid)};

	size_t charIndex = 0;
	size_t i         = 0;

	while (startingPosition.second < gridHeight)
	{
		grid[startingPosition.second][startingPosition.first] = word[charIndex];
		chainCharacters(grid, startingPosition, word[charIndex]);
		i++;
		charIndex        = (charIndex + 1) % word.size();
		startingPosition = getFirstEmptySlot(grid);
	}
}

int main(int argc, char* argv[])
{

	size_t      gridWidth{10};
	size_t      gridHeight{10};
	std::string word{"ABC"};

	gridWidth  = readTypedInput<size_t>(std::format("Width: ", gridWidth), gridWidth);
	gridHeight = readTypedInput<size_t>(std::format("Height: ", gridHeight), gridHeight);
	word       = readTypedInput<std::string>(std::format("Word: ", word), word);

	gridT grid(gridHeight, std::vector<char>(gridWidth, emptyChar));

	sakusAlgorithm(grid, word);

	printGrid(grid);

	printGridStats(grid);

	std::cout << "\nPress ENTER to quit...";
	std::cin.get();

	return 0;
}