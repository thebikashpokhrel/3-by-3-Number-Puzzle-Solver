#include "Utilities.h"
#include "sfmlHelpers.h"
#include <queue>
#include <algorithm>
#include <unordered_set>

vector<int> initial = {1,2,3,4,5,6,8,7,0 }; //Initial State
vector<int> goal = { 1, 2, 3, 4, 5, 6, 7, 8, 0 }; //Goal State

class State {
public:
    string direction;
    vector<int> state;
    State* parent;

    State(vector<int> state, State* parent, string direction)
        : state(state), parent(parent), direction(direction){}

    bool isGoalState() {
        return state == goal;
    }

    vector<string> available_moves(int x, int n) {
        vector<string> moves = { "Left", "Right", "Up", "Down" };
        if (x % n == 0) {
            moves.erase(remove(moves.begin(), moves.end(), "Left"), moves.end());
        }
        if (x % n == n - 1) {
            moves.erase(remove(moves.begin(), moves.end(), "Right"), moves.end());
        }
        if (x - n < 0) {
            moves.erase(remove(moves.begin(), moves.end(), "Up"), moves.end());
        }
        if (x + n > n * n - 1) {
            moves.erase(remove(moves.begin(), moves.end(), "Down"), moves.end());
        }
        return moves;
    }

    vector<State*> expand(int n) {
        int x = distance(state.begin(), find(state.begin(), state.end(), 0));
        vector<string> moves = available_moves(x, n);

        vector<State*> children;
        for (string direction : moves) {
            vector<int> temp = state;
            if (direction == "Left") {
                swap(temp[x], temp[x - 1]);
            }
            else if (direction == "Right") {
                swap(temp[x], temp[x + 1]);
            }
            else if (direction == "Up") {
                swap(temp[x], temp[x - n]);
            }
            else if (direction == "Down") {
                swap(temp[x], temp[x + n]);
            }
            children.push_back(new State(temp, this, direction));
        }
        return children;
    }

    vector <string> solution() {
        vector<string> solution;
        solution.push_back(direction);
        State* path = this;
        while (path->parent != nullptr) {
            path = path->parent;
            solution.push_back(path->direction);
        }
        solution.pop_back();
        reverse(solution.begin(), solution.end());
        return solution;
    }
};
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int i : v) {
            hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


//BFS Algorithm
vector<string> BFS(vector<int>& given_state, int n) {
    State* root = new State(given_state, nullptr, "");
    if (root->isGoalState()) {
        return root->solution();
    }

    queue<State*> frontier;
    frontier.push(root);
    unordered_set<vector<int>, VectorHash> explored;

    while (!frontier.empty()) {
        State* current_node = frontier.front();
        frontier.pop();
        explored.insert(current_node->state);

        vector<State*> children = current_node->expand(n);
        for (State* child : children) {
            if (explored.find(child->state) == explored.end()) {
                if (child->isGoalState()) {
                    vector<string> solution = child->solution();
                    delete child;
                    return solution;
                }
                frontier.push(child);
            }
        }
    }
    return {};
}

//Define the Window View Properties
const int windowWidth = 800;
const int windowHeight = 800;
const int gridSize = 3;
const int cellSize = 100;
const int marginX = 150;
const int marginY = 220;

//Screen rendering
void drawBoard(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Font file not found!" << std::endl;
    }

    //Draw the title
    Vector2f position(windowWidth / 2 - 170, 20);
    Color color = Color::Black;
    drawText(window, "3 × 3 Number Puzzle Solver", font, 30, position, color, Text::Bold);

    //Draw the Generate button
    Vector2f pos1{ windowWidth / 2 - 250,100 };
    Vector2f size1{ 250,40 };
    Color c1(12, 45, 87);
    sfButton generateBtn(pos1, size1, c1, Color::Green, Color::White, "Generate Random Board", font, 20);
    generateBtn.draw(window);

    //Draw the BFS Solve button
    Vector2f pos2{ windowWidth / 2 - 250,550 };
    Vector2f size2{ 160,40 };
    Color c2(255, 0, 7);
    sfButton BFSBtn(pos2, size2, c2, Color::Green, Color::White, "Solve with BFS", font, 20);
    BFSBtn.draw(window);


    //Draw the tiles on the screen
  for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            int number = initial[i * gridSize + j];
            if (number == 0) continue;
            RectangleShape rect(Vector2f(cellSize, cellSize));
            Color woodenColor(86, 50, 50);
            rect.setPosition(marginX + j * cellSize, marginY + i * cellSize);
            rect.setFillColor(woodenColor);
            rect.setOutlineThickness(1);
            rect.setOutlineColor(Color::Black);
            window.draw(rect);
            Vector2f position(marginX + j * cellSize + cellSize / 3, marginY + i * cellSize + cellSize / 3);
            Color color = Color::White;
            drawText(window, to_string(number), font, cellSize / 3, position, color, Text::Bold);
        }
    }
}


int main() {
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sliding Puzzle");
window.clear(sf::Color::White);
sf::Font font;
if (!font.loadFromFile("arial.ttf")) {
std::cerr << "Font file not found!" << std::endl;
}

//Render screen for the first time
drawBoard(window);
window.display();

auto duration = 0; //To keep track of time required for the BFS to execute
int emptyIndex; // Index of the empty cell
int isSolving = false; //Whether the puzzle is being solved or not
vector<string> soln; //Array of sequence of moves to solve the puzzle

while (window.isOpen()) {
// Handle events
sf::Event event;
while (window.pollEvent(event))
{
    if (event.type == sf::Event::Closed)
        window.close();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        //Check if the Generate Button is clicked
        if (isBtnClicked(vector<int>{150, 400, 100, 140}, mousePos) && !isSolving) {
            //Render the random initial state by shuffling the goal state
            initial = shuffleVector(goal);
            window.clear(sf::Color::White);
            drawBoard(window);
            window.display();
        }

//Check if SOLVE with BFS Button is Clicked
if (isBtnClicked(vector<int>{150, 310, 550, 590}, mousePos) && initial != goal) {
//Find the set of moves using BFS
isSolving = true;
if (isSolvable(inv_num(initial))) {
    //Draw isSOlving State
    Vector2f position2(windowWidth / 2 - 250, 620);
    window.clear(sf::Color::White); // Set background color to white
    Color color2(51, 58, 115);
    drawText(window, "Finding the minmum set of moves...", font, 22, position2, color2, Text::Bold);
    drawBoard(window);
    window.display();

    //Track the duration of execution
    auto start = chrono::steady_clock::now();
    soln = BFS(initial, gridSize);
    auto end = chrono::steady_clock::now();
    duration = chrono::duration_cast<std::chrono::seconds>(end - start).count();
}
else {
    soln = {};
}
if (!soln.empty()) {
    cout << "Solution found:" << endl;
    for (string& move : soln) {
        cout << move << " ";
    }
    cout << endl;
    for (int i = 0; i < soln.size(); i++) {
        emptyIndex = distance(initial.begin(), find(initial.begin(), initial.end(), 0));
        if (soln[i] == "Left") {
            std::swap(initial[emptyIndex], initial[emptyIndex - 1]);
            emptyIndex = emptyIndex - 1;
        }
        if (soln[i] == "Right") {
            std::swap(initial[emptyIndex], initial[emptyIndex + 1]);
            emptyIndex = emptyIndex + 1;
        }
        if (soln[i] == "Down") {
            std::swap(initial[emptyIndex], initial[emptyIndex + gridSize]);
            emptyIndex = emptyIndex + gridSize;
        }
        if (soln[i] == "Up") {
            std::swap(initial[emptyIndex], initial[emptyIndex - gridSize]);
            emptyIndex = emptyIndex + gridSize;
        }
        window.clear(sf::Color::White); // Set background color to white
        drawBoard(window);
        //Draw isSOlving State
        Vector2f position2(windowWidth / 2 - 250, 620);
        Color color2(51, 58, 115);
        drawText(window, "Solving...", font, 22, position2, color2, Text::Bold);

        //Code to Generate the Progress BAR
        int progress = 300 / soln.size() * (i+1);
        showProgress(window, progress, windowWidth / 2 - 250);
        //Progress Bar Label
        Vector2f position(500, 680);
        Color color = Color::Black;
        drawText(window, to_string(i + 1) + " / " + to_string(soln.size()) + " moves", font, 25, position, color, Text::Bold);

        //Diplay Moves name
        Vector2f position3(windowWidth / 2 - 250, 750);
        drawText(window, "Current Move: " + soln[i], font, 22, position3, color2, Text::Bold);
        window.display();
        delay(200);
    }
    window.clear(sf::Color::White);
    //Draw the solved state text
    Vector2f position2(windowWidth / 2 - 250, 620);
    Color color2(51, 58, 115);
    drawText(window, "Solved in " + to_string(soln.size()) + " moves (Computational Time:" + to_string(duration) + " seconds)", font, 22, position2, color2, Text::Bold);
    drawBoard(window);
    showProgress(window, 300, windowWidth / 2 - 250);
    //Progress Bar Label
    Vector2f position(500, 680);
    Color color = Color::Black;
    drawText(window, to_string(soln.size()) + " / " + to_string(soln.size()) + " moves", font, 25, position, color, Text::Bold);
    window.display();
    isSolving = false;
}
            else {
                cout << "Not solvable!" << endl;
                window.clear(sf::Color::White);
                //Draw the isSolving state text
                Vector2f position2(windowWidth / 2 - 250, 620);
                Color color2(51, 58, 115);
                drawText(window, "Puzzle is not solvable.", font, 22, position2, color2, Text::Bold);
                drawBoard(window);
                window.display();
                isSolving = false;
            }
        }

    }
}
}

return 0;
}
