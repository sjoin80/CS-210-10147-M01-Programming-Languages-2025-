/*
    Program Name: Corner Grocer Item Tracker
    Author: Shelby Joiner
    Date: 4/11/2025

    Description:
        This program reads item names from a file and tracks the frequency of each item.
        Users can interact with a menu to:
        1. Search for the frequency of a specific item (case-insensitive),
        2. Display all item frequencies,
        3. Display a histogram representing item purchase counts,
        4. Exit the program.

       ** Item names are stored in their original capitalization for display,
        but all lookups are case-insensitive for user convenience. **

    Input File: (default input)
        CS210_Project_Three_Input_File.txt

*/

#include <iostream>     // For standard input and output operations (e.g., cout, cin)
#include <fstream>      // For file stream operations (reading from or writing to files)
#include <map>          // For using the map container to store key-value pairs (e.g., item names and their frequencies)
#include <string>       // For working with the string data type and its associated functions
#include <cctype>       // For character-handling functions (e.g., tolower(), isalpha())
#include <algorithm>    // For general-purpose algorithms (e.g., transform(), sort(), find())

// Class to manage and track grocery item frequencies
class GroceryTracker {
private:
    // Map to store item names and their frequencies
    std::map<std::string, int> frequencyMap;  // Item -> Frequency

    // Map to store case-insensitive lowercase item names with their original case
    std::map<std::string, std::string> caseMap;  // Lowercase Item -> Original Item

    // File path for input data
    std::string inputFile = "CS210_Project_Three_Input_File.txt";  // Input file containing items

    // Converts string to lowercase
    std::string ToLower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);  // Apply lowercase transformation
        return str;
    }

    // Capitalizes the first letter of the string
    std::string CapitalizeFirstLetter(std::string item) {
        if (!item.empty()) {
            item[0] = std::toupper(item[0]);  // Capitalize the first character
        }
        return item;
    }

    // Trims leading and trailing spaces from a string
    std::string Trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');  // Find first non-space character
        size_t last = str.find_last_not_of(' ');   // Find last non-space character
        return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);  // Return trimmed string
    }

    // Loads item data from the input file and populates frequencyMap and caseMap
    bool LoadFromFile() {
        std::ifstream file(inputFile);  // Open the input file
        if (!file) {
            std::cerr << "Error: Could not open " << inputFile << std::endl;
            return false;  // Return false if file cannot be opened
        }

        std::string item;
        while (file >> item) {  // Read each item from the file
            std::string itemLower = ToLower(item);  // Convert item to lowercase for case-insensitive handling
            if (caseMap.find(itemLower) == caseMap.end()) {
                caseMap[itemLower] = item;  // Store original item in caseMap if it's not already present
            }
            frequencyMap[caseMap[itemLower]]++;  // Increment the frequency of the item
        }

        file.close();  // Close the input file
        return true;  // Return true if loading is successful
    }

    // Validates if the given item name contains only alphabetic characters
    bool IsValidItemName(std::string item) {
        item = Trim(item);  // Trim any spaces from the item name
        if (item.empty()) return false;  // Return false if the item is empty
        for (char c : item) {
            if (!std::isalpha(c)) return false;  // Return false if item contains non-alphabetic characters
        }
        return true;  // Return true if item is valid
    }

    // Validates the user's menu choice (only options 1-5 are valid)
    bool IsValidMenuChoice(const std::string& choice) {
        if (choice.length() == 1 && std::isdigit(choice[0])) {
            return choice[0] >= '1' && choice[0] <= '4';  // Return true if choice is between 1 and 4
        }
        return false;  // Return false if the choice is invalid
    }

    // Saves the frequency data to a file named "frequency.dat"
    void SaveToFile() {
        std::ofstream outFile("frequency.dat");  // Open frequency.dat for writing
        if (!outFile) {
            std::cerr << "Error: Could not open frequency.dat for writing.\n";
            return;  // Return if file cannot be opened
        }

        // Write item names and their corresponding frequencies to the file
        for (const auto& pair : frequencyMap) {
            outFile << pair.first << " " << pair.second << "\n";  // Write item and frequency to the file
        }

        outFile.close();  // Close the output file
        std::cout << "Data successfully saved to frequency.dat\n";  // Notify the user of successful save
    }

public:
    // Constructor to initialize and load data from the file
    GroceryTracker() {
        if (!LoadFromFile()) {
            std::cerr << "Program terminated due to file load error.\n";
            exit(1);  // Exit the program if file loading fails
        }
    }

    // Looks up the frequency of a specific item entered by the user
    void LookUpFrequency() {
        std::string input;
        std::cout << "Enter the item name: ";
        std::getline(std::cin, input);  // Get input from the user
        input = Trim(input);  // Trim any spaces

        // Validate if the input contains only alphabetic characters
        if (!IsValidItemName(input)) {
            std::cout << "Invalid input. Only alphabetic characters are allowed.\n";
            return;
        }

        std::string lower = ToLower(input);  // Convert the input to lowercase
        if (caseMap.find(lower) != caseMap.end()) {
            std::string actualItem = caseMap[lower];  // Get the original item name
            std::cout << actualItem << " was purchased " << frequencyMap[actualItem] << " time(s).\n";
        }
        else {
            std::cout << input << " was not found in the records.\n";
        }
    }

    // Displays the frequency of all items
    void DisplayAllItemFrequencies() {
        std::cout << "\nItem Frequencies:\n";
        for (const auto& pair : frequencyMap) {
            std::cout << pair.first << " : " << pair.second << '\n';  // Display item and frequency
        }
    }

    // Displays a histogram of item frequencies using asterisks
    void DisplayHistogram() {
        std::cout << "\nItem Purchase Histogram:\n";
        for (const auto& pair : frequencyMap) {
            std::cout << pair.first << " : ";
            for (int i = 0; i < pair.second; ++i) {
                std::cout << "*";  // Print asterisk for each occurrence of the item
            }
            std::cout << '\n';
        }
    }

    // Main loop to handle user input and program choices
    void Run() {
        std::string choice;
        do {
            std::cout << "\n======= Corner Grocer Item Tracker =======\n";
            std::cout << "1. Look up frequency of a specific item\n";
            std::cout << "2. Display all item frequencies\n";
            std::cout << "3. Display item histogram\n";
            std::cout << "4. Save data to frequency.dat\n";  // Option to save data
            std::cout << "5. Exit program\n";
            std::cout << "Enter your choice (1-5): ";
            std::getline(std::cin, choice);  // Get the user's choice

            // Execute the corresponding function based on the user's choice
            if (IsValidMenuChoice(choice)) {
                if (choice == "1") {
                    LookUpFrequency();
                }
                else if (choice == "2") {
                    DisplayAllItemFrequencies();
                }
                else if (choice == "3") {
                    DisplayHistogram();
                }
                else if (choice == "4") {  // Save data to file
                    SaveToFile();
                }
            }
            else {
                std::cout << "Invalid input. Please enter a number between 1 and 5.\n";
            }

        } while (choice != "5");  // Continue until the user chooses to exit

        std::cout << "Exiting program. Goodbye!\n";  // Goodbye message when exiting
    }
};

// Main function to initialize and run the GroceryTracker
int main() {
    GroceryTracker tracker;  // Create an instance of the GroceryTracker class
    tracker.Run();  // Start the program
    return 0;  // Exit the program
}

/*
* 
Explanation of the comments:

Class-level comments: These describe the purpose and functionality of the class, including the data members.

Function-level comments: These comments explain what each function is doing, such as converting to lowercase or validating input.

Code-level comments: These comments describe individual lines of code, especially in areas that may not be immediately clear, such as the ToLower and Trim functions.

*/