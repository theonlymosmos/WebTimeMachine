// https://www.linkedin.com/in/mousa123/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Node for our browser history stack
struct HistoryNode {
    string url;         // The webpage URL
    HistoryNode* prev;  // Link to previous page
    HistoryNode* next;  // Link to next page
    HistoryNode(const string& url) : url(url), prev(nullptr), next(nullptr) {}
};

class BrowserHistory {
private:
    HistoryNode* currentPage; // Currently viewed page
    int historySize;          // How many pages in history
    int maxSize;              // Maximum history we can store

public:
    BrowserHistory(int capacity) : currentPage(nullptr), historySize(0), maxSize(capacity) {}
    
    // Clean up memory when browser closes
    ~BrowserHistory() {
        while (!isEmpty()) {
            popPage();
        }
    }
    
    // Check if history is empty
    bool isEmpty() {
        return currentPage == nullptr;
    }
    
    // Check if history is full
    bool isFull() {
        return historySize == maxSize;
    }
    
    // Add a new page to history
    void pushPage(string newUrl) {
        if (isFull()) {
            cout << "Stack is full!" << endl;
            return;
        }
        HistoryNode* newNode = new HistoryNode(newUrl);
        if (isEmpty()) {
            currentPage = newNode;
        } else {
            newNode->prev = currentPage;
            currentPage->next = newNode;
            currentPage = newNode;
        }
        historySize++;
    }
    
    // Remove current page from history (go back)
    string popPage() {
        if (isEmpty()) {
            return "";
        }
        HistoryNode* temp = currentPage;
        string url = currentPage->url;
        currentPage = currentPage->prev;
        if (currentPage != nullptr) {
            currentPage->next = nullptr;
        }
        delete temp;
        historySize--;
        return url;
    }
    
    // Get current page URL
    string getCurrent() {
        if (isEmpty()) {
            return "";
        }
        return currentPage->url;
    }
    
    // Get previous page URL
    string getPrevious() {
        if (isEmpty() || currentPage->prev == nullptr) {
            return "";
        }
        return currentPage->prev->url;
    }
    
    // Get next page URL (if we went back)
    string getNext() {
        if (isEmpty() || currentPage->next == nullptr) {
            return "";
        }
        return currentPage->next->url;
    }
};

int main() {
    ifstream historyFile("URLs.txt");
    if (!historyFile.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    int maxHistorySize;
    historyFile >> maxHistorySize;

    string dummyLine;
    getline(historyFile, dummyLine); // Skip first newline
    getline(historyFile, dummyLine); // Read first URL

    BrowserHistory backHistory(maxHistorySize);
    BrowserHistory forwardHistory(maxHistorySize);

    string currentUrl;
    while (getline(historyFile, currentUrl)) {
        if (currentUrl.empty()) continue;
        if (currentUrl.find("Backward") != string::npos || currentUrl.find("Forward") != string::npos) {
            break;
        }
        backHistory.pushPage(currentUrl);
    }

    string navigationCommands = currentUrl;
    if (navigationCommands.find("Backward") == string::npos && 
        navigationCommands.find("Forward") == string::npos) {
        getline(historyFile, navigationCommands);
    }

    historyFile.close();

    int choiceNumber = 1;
    size_t commandPos = 0;
    while ((commandPos = navigationCommands.find(' ')) != string::npos) {
        string currentCommand = navigationCommands.substr(0, commandPos);
        navigationCommands.erase(0, commandPos + 1);

        cout << "Current page:\n" << backHistory.getCurrent() << "\n\n";
        cout << "Choice " << choiceNumber++ << " [" << currentCommand << "]\n";

        if (currentCommand == "Backward") {
            if (backHistory.isEmpty() || backHistory.getPrevious() == "") {
                cout << "Cannot go backward. Already at the oldest page.\n\n";
                continue;
            }
            string movedUrl = backHistory.popPage();
            forwardHistory.pushPage(movedUrl);

            cout << "The current page is:\n" << backHistory.getCurrent() << "\n\n";
            cout << "The Next one is:\n" << movedUrl << "\n\n";

            string previousUrl = backHistory.getPrevious();
            if (previousUrl != "") {
                cout << "The Previous one is:\n" << previousUrl << "\n\n";
            } else {
                cout << "No previous page available.\n\n";
            }
        } 
        else if (currentCommand == "Forward") {
            if (forwardHistory.isEmpty()) {
                cout << "Cannot go forward. No forward history available.\n\n";
                continue;
            }
            string movedUrl = forwardHistory.popPage();
            backHistory.pushPage(movedUrl);

            cout << "The current page is:\n" << movedUrl << "\n\n";

            string nextUrl = forwardHistory.getCurrent();
            if (nextUrl != "") {
                cout << "The Next one is:\n" << nextUrl << "\n\n";
            } else {
                cout << "No next page available.\n\n";
            }

            string previousUrl = backHistory.getPrevious();
            if (previousUrl != "") {
                cout << "The Previous one is:\n" << previousUrl << "\n\n";
            }
        }
    }

    // Handle the last command (since the while loop stops when no more spaces are found)
    if (!navigationCommands.empty()) {
        cout << "Current page:\n" << backHistory.getCurrent() << "\n\n";
        cout << "Choice " << choiceNumber++ << " [" << navigationCommands << "]\n";

        if (navigationCommands == "Backward") {
            if (backHistory.isEmpty() || backHistory.getPrevious() == "") {
                cout << "Cannot go backward. Already at the oldest page.\n\n";
            } else {
                string movedUrl = backHistory.popPage();
                forwardHistory.pushPage(movedUrl);

                cout << "The current page is:\n" << backHistory.getCurrent() << "\n\n";
                cout << "The Next one is:\n" << movedUrl << "\n\n";

                string previousUrl = backHistory.getPrevious();
                if (previousUrl != "") {
                    cout << "The Previous one is:\n" << previousUrl << "\n\n";
                } else {
                    cout << "No previous page available.\n\n";
                }
            }
        } 
        else if (navigationCommands == "Forward") {
            if (forwardHistory.isEmpty()) {
                cout << "Cannot go forward. No forward history available.\n\n";
            } else {
                string movedUrl = forwardHistory.popPage();
                backHistory.pushPage(movedUrl);

                cout << "The current page is:\n" << movedUrl << "\n\n";

                string nextUrl = forwardHistory.getCurrent();
                if (nextUrl != "") {
                    cout << "The Next one is:\n" << nextUrl << "\n\n";
                } else {
                    cout << "No next page available.\n\n";
                }

                string previousUrl = backHistory.getPrevious();
                if (previousUrl != "") {
                    cout << "The Previous one is:\n" << previousUrl << "\n\n";
                }
            }
        }
    }

    return 0;
}