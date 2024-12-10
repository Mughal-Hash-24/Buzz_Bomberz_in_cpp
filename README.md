# Buzz Bombers - A Modernized Classic

## Description
This project recreates the classic **Buzz Bombers** game from Intellivision, adding a modern twist while staying true to the original mechanics. Built using **SFML** and **C++**, it features multiple levels of increasing difficulty, a robust scoring system, and dynamic gameplay elements.

The catch: **No classes, structs, or enums were allowed!** This project demonstrates the creative use of fundamental programming constructs like arrays, pointers, and functions to achieve a functional and engaging game.

## Features
### Core Gameplay
- **Spray Can**: Move horizontally and spray bees to transform them into honeycombs.
- **Hummingbird Ally**: Scores bonus points by eating honeycombs.
- **Flowers**: Grow and restrict movement, adding a layer of strategy.
- **Bee Types**:  
  - Worker Bees (Yellow)  
  - Killer Bees (White, faster, turn into Red Honeycombs when hit)

### Phased Development
#### Phase 1
- Three levels of increasing difficulty:
  - Level 1: 20 Worker Bees, 3 Honeycombs
  - Level 2: 15 Worker Bees, 5 Killer Bees, 9 Honeycombs
  - Level 3: 20 Worker Bees, 10 Killer Bees, 15 Honeycombs
- Unique backgrounds and music for each level.
- Basic scoring system and responsive controls.

#### Phase 2
- **Boss Level**:
  - Introduced **Infant Bees** that mature into Red Hunter Bees, utilizing dynamic memory allocation.
- **Power-Ups**:
  - Speed boost/reduction and size adjustments.
- **High Score Tracking**:
  - Records the top 10 scores, prompting for the player’s name.

## Development Constraints
- No object-oriented constructs (classes, structs, enums).
- No external libraries except **SFML**, **cmath**, **ctime**, and **cstdlib**.
- Heavy reliance on arrays, pointers, and dynamic memory allocation.

## Setup and Usage
### Prerequisites
- **SFML** library installed.
- A C++ compiler supporting C++17 or later.

### Running the Game
1. Clone the repository:  
   ```bash
   git clone https://github.com/your_username/buzz-bombers.git
   cd buzz-bombers
2. Compile the project:
   ```bash
	g++ -o buzz buzz.cpp -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
3. Run the executable:
	```bash
	./buzz
## Key Features in Code
- **Dynamic Memory Allocation**: Used to create and manage **Infant Bees** during the boss level.
- **Modular Design**: Functions handle specific tasks like rendering, updating states, and collision detection.
- **File Handling**: Maintains high scores in an external file.

## Challenges Overcome
This project showcases how constraints can fuel creativity, demonstrating advanced game logic without modern programming paradigms like classes and structs.

## Future Improvements
- Adding more intricate power-ups.
- Introducing customizable controls.
- Enhancing graphics and animations.

## Acknowledgements
- Inspired by the original **Buzz Bombers** game from 1983 by Mattel Electronics.
