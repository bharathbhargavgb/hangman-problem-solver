// © 2014 Madras EYE
// All rights reserved

// Authored by

// Bharath Bhargav and Prasath

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdlib>
#include<stack>
#include<windows.h>
using namespace std;

int resultCount;
int guess[26];
int blacklist[26];
bool firstSplit = true;
int i = 0, inputLength = 0;
static int asp = -1;
string input;


class Node {
public:
    char c;
    int min, max;
    bool t;
    vector<Node> children;
    int pointer;
    bool lastchild;
    Node();
    void minMaxAdjust();
    void pushword(string word);
    void print(string prefix);
};

typedef stack<Node> candidate;
vector<candidate> vs(20);
stack<Node*> s;

Node :: Node() {
    min = 1;
    max = 0;
}
                                                                // designating min and max values for each character
void Node :: minMaxAdjust() {
    if(!s.empty()) {
        Node *top = s.top();
        if(this->min+1 < top->min) {
            top->min = this->min + 1;
            Node *temp = s.top();
            s.pop();
            temp->minMaxAdjust();
        } else if(this->max+1 > top->max) {
            top->max = this->max + 1;
            Node *temp = s.top();
            s.pop();
            temp->minMaxAdjust();
        }
    }
}

void Node::pushword(string word) {                                  // trie insertion
    if (word.length() == 0) {
        t = true;
        min = max = 0;
        firstSplit = true;
        while(!s.empty()) s.pop();
    }
    else {
        char first = word[0];
        string rest = word.substr(1, word.length() - 1);
        int index = -1;

        for (int i = 0; i < children.size(); i++) {
            if (children[i].c == first) {
                index = i;
                i = children.size();                            // to exit the loop
            }
        }

        if (index == -1) {                                          // newly created node
            Node n;
            n.c = first;
            n.t = false;
            n.min = n.max = rest.length();
            n.pointer = 0;
            n.lastchild = false;
            children.push_back(n);
            index = children.size() - 1;
            if(firstSplit == true) {
                n.minMaxAdjust();
                firstSplit = false;
            }
        }
        s.push(&children[index]);
        children[index].pushword(rest);                             // tail recursion
    }
}

void Node::print(string prefix) {                                   // trie traversal (function not necessary)
    if (t) {
//        cout << prefix << endl;
    }

    //cout << "prefix: " << prefix << ", children: " << children.size() << endl;

   /* if (children.size() > 0) {
        pointer = nodelist.size();
        children[children.size() - 1].lastchild = true;
    }

    for (int i = 0; i < children.size(); i++) {
        nodelist.push_back(&children[i]);
    }   */

    for (int i = 0; i < children.size(); i++) {
        children[i].print(prefix + children[i].c);
    }
}

void display(Node current) {                                    //function displays the string that matches with the input conditions
    int localASP = asp - 1;
	stack<char> cs;
    cs.push(current.c);
    while(localASP >= 0) {
        current = vs[localASP].top();
        cs.push(current.c);
        localASP--;
    }
	while(!cs.empty()) {
		cout << cs.top();
		//printf("%c", cs.top());                                     // printf and cout brings the output at the same speed... No major difference...!
		guess[cs.top()-65]++;
		cs.pop();
	}
    printf("\n");
}

bool charComp(Node current) {
    if(inputLength-asp-1 < current.min || inputLength-asp-1 > current.max) {            // reduces time complexity according to the word length and the input specification.. Hybrid variety of trie
        return false;
    }
    if(current.c == input[asp] || input[asp] == '_') {
        return true;
    }
    return false;
}

void search(Node current) {                                         // function that searches the trie for the given input
    int n = current.children.size();
    asp++;
    for(int j = 0; j < n; j++) {
        if(charComp(current.children[j])) {
            if(asp+1 == inputLength) {
                resultCount++;
                display(current.children[j]);
            } else {
                vs[asp].push(current.children[j]);
            }
        }
	}
        while(!vs[asp].empty()) {
            current = vs[asp].top();
            search(current);
            vs[asp].pop();
        }
        asp--;
}

void displayHangman() {                                                         // ASCII art

    //system("cls");
    cout << endl;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|11);
    cout << " _   _   ___   _   _ _____ ___  ___  ___   _   _ " << endl;
    cout << "| | | | / _ \\ | \\ | |  __ \\|  \\/  | / _ \\ | \\ | |" << endl;
    cout << "| |_| |/ /_\\ \\|  \\| | |  \\/| .  . |/ /_\\ \\|  \\| |" << endl;
    cout << "|  _  ||  _  || . ` | | __ | |\\/| ||  _  || . ` |" << endl;
    cout << "| | | || | | || |\\  | |_\\ \\| |  | || | | || |\\  |" << endl;
    cout << "\\_| |_/\\_| |_/\\_| \\_/\\____/\\_|  |_/\\_| |_/\\_| \\_/" << endl;
    cout << endl;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0|15);
}

void initialInstructions() {

    cout << "Welcome to Hangman (Version:Madras EYE) " << endl;
    cout << endl;
    cout << "Input a word to the computer as blanks with few hints to get the game started.. :D " << endl;
    cout << endl;
    cout << "Characters should be in UPPERCASE !" << endl;
    cout << endl;
    cout << "Example input: _O__UT_R for the word COMPUTER. " << endl;
    cout << endl;
    cout << "May the odds be ever in your favour...! " << endl;
    cout << endl;
}

int main() {

    string word;
    int count = 0, max = 0, charCount;
    Node root;
    displayHangman();


 //....Easter Egg                           (Remove the comment for the true color... But the program won't run... :P   )
/*
    cout << "   ▄█    █▄       ▄████████ ███▄▄▄▄      ▄██████▄    ▄▄▄▄███▄▄▄▄      ▄████████ ███▄▄▄▄   " << endl;
cout << "      ███    ███     ███    ███ ███▀▀▀██▄   ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███ ███▀▀▀██▄ " << endl;
  cout << "    ███    ███     ███    ███ ███   ███   ███    █▀  ███   ███   ███   ███    ███ ███   ███ " << endl;
 cout << "    ▄███▄▄▄▄███▄▄   ███    ███ ███   ███  ▄███        ███   ███   ███   ███    ███ ███   ███ " << endl;
cout << "    ▀▀███▀▀▀▀███▀  ▀███████████ ███   ███ ▀▀███ ████▄  ███   ███   ███ ▀███████████ ███   ███ " << endl;
  cout << "    ███    ███     ███    ███ ███   ███   ███    ███ ███   ███   ███   ███    ███ ███   ███ " << endl;
  cout << "    ███    ███     ███    ███ ███   ███   ███    ███ ███   ███   ███   ███    ███ ███   ███ " << endl;
  cout << "    ███    █▀      ███    █▀   ▀█   █▀    ████████▀   ▀█   ███   █▀    ███    █▀   ▀█   █▀  " << endl;
*/


    initialInstructions();

    for (int j = 0; j < 3; j++) {                                           //Loading...
        cout << "\rLoading   \rLoading";
        Sleep(300);
        for (int i = 0; i < 3; i++) {
            cout << ".";
            Sleep(300);
        }
    }
    //Dictionary words insertion
    ifstream file("dictWords.txt");
    while(!file.eof()) {
        file >> word;
        if(word.length() > max) max = word.length();        //find the longest word in the dictionary input file
        i++;
        count++;
        //cout << word << endl;
        s.push(&root);                                      //push an empty root to the stack
        root.pushword(word);                                //Trie insertion takes place.... This command takes approximately 5 seconds.
                                                            //But the initial couts statements takes care of the deviation while the trie is being constructed
    }
    file.close();
    /*cout << root.min << " " << root.max << endl;
    cout << "MAXIMUM " << max << endl;
    cout << "Number of Words = " << count << endl;*/
    //root.print("");

    cout << endl << endl;
    char reply;
    do {
        for(int i = 0; i < 26; i++) {                           //blacklist is used to eliminate the already guessed characters and other characters which alreaady appear in the input from the user...
            blacklist[i] = 1;
            guess[i] = 0;
        }
        reply = 'n';
        do {
            cout << "Enter the string: " ;
            char answer = 'n';
            cin >> input;
            //memset(guess, 0, 26);
            charCount = 0;
            for(int i = 0; input[i] != '\0'; i++) {             // Add characters to the blacklist
                if(input[i] >= 65 && input[i] <=90) {
                    int toNumber = input[i] - 65;
                    blacklist[toNumber] = -1;
                    charCount++;
                }
            }

            inputLength = input.length();
            if(charCount == inputLength)                            //if only one word matches the input then break;
                break;

            for(int i = 0; i < 26; i++) {
                    guess[i] = 0;
            }
            resultCount = 0;

            cout << "The solutions are " << endl;
            search(root);                                               //searching the trie for the given input
            if(resultCount == 1 || resultCount == 0) break;

            do {

                int findMax = 0;

                for(int  i = 0; i < 26; i++) {
                    if(blacklist[i] == -1) {
                        guess[i] = -1;
                    }
                }

                for(int i = 0; i < 26; i++) {
                    if(guess[i] > guess[findMax]) {                 //finding the frequency of characters from the obtained results
                        findMax = i;
                    }
                }

                guess[findMax] = -1;
                displayHangman();
                cout << input << endl << endl;

                /*for(int i = 0; i < 26; i++) {                                    //character distribution display
                    cout << i+65 << " " << guess[i] << " " << blacklist[i] << endl;
                }*/

                cout << "Guessing   " << char(findMax+65) << endl << endl;
                cout << "Is the guess correct...? (y/n)  " ;
                cin >> answer;
                cout << endl;
                blacklist[findMax] = -1;

            } while(answer == 'n' || answer == 'N');

            cout << "Enter the input with the guessed characters." << endl;
            cout << endl;

        } while(resultCount > 1);

        if(resultCount == 1 || charCount == inputLength)
            cout << endl << "Mangaatha da...! " << endl;
        else if(resultCount == 0)
            cout << "Sorry...! No such word exist in my dictionary " << endl;

        cout << endl;
        cout << "Wanna play again...? (y/n) " << endl;
        cin >> reply;

        displayHangman();

    } while(reply == 'y' || reply == 'Y');

    cout << "Android app coming soon... ;D " << endl << endl;
    cout << "Bye bye...! " << endl;
    cout << endl;
    return 0;
}
