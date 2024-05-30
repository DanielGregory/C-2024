#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// Define a struct to hold two numbers
struct Pokemon {
public:
    double name;
      int stats;

      Pokemon() : name(0), stats(0){}
    
    Pokemon(double nameValue, int statsValue) : name(nameValue), stats(statsValue) {}
};


void insertionSort(vector<Pokemon>& vec, int& comparisons) {
    int size = vec.size();
    for (int step = 1; step < size; step++) {
        Pokemon key = vec[step];
        int j = step - 1;

        // Compare key with each element on the left of it until an element smaller than
        // it is found.
        // For descending order, change key < vec[j] to key > vec[j].
        while (j >= 0 && key.stats < vec[j].stats) {
            vec[j + 1] = vec[j];
            --j;
            comparisons++;
        }
        vec[j + 1] = key;
    }
}

// Merge two sorted subvectors vec[p..q] and vec[q+1..r]
void merge(vector<Pokemon>& vec, int p, int q, int r, int& comparisons) {
    int n1 = q - p + 1;
    int n2 = r - q;

    vector<Pokemon> L(n1);
    vector<Pokemon> M(n2);

    // Copy data to temp vectors L[] and M[]
    for (int i = 0; i < n1; i++)
        L[i] = vec[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = vec[q + 1 + j];

    // Merge the temp vectors back into vec[p..r]
    int i = 0, j = 0, k = p;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i].stats <= M[j].stats) {
            vec[k] = L[i];
            i++;
        }
        else {
            vec[k] = M[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of M[], if any
    while (j < n2) {
        vec[k] = M[j];
        j++;
        k++;
    }
}

// Merge sort function to sort the elements of a vector
void mergeSort(vector<Pokemon>& vec, int l, int r, int& comparisons) {
    if (l < r) {
        // Find the middle point
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(vec, l, m, comparisons);
        mergeSort(vec, m + 1, r, comparisons);

        // Merge the sorted halves
        merge(vec, l, m, r, comparisons);
    }
}
// Function to swap elements in a vector
void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

// Function to rearrange vector elements and find the partition point
int partition(vector<Pokemon>& vec, int low, int high, int& comparisons) {
    // Select the rightmost element as pivot
    Pokemon pivot = vec[high];
    // Pointer for the greater element
    int i = low - 1;

    // Traverse each element of the vector, compare them with the pivot
    for (int j = low; j < high; j++) {
        comparisons++;
        if (vec[j].stats <= pivot.stats) {
            // If an element smaller than the pivot is found, swap it with the greater element pointed by i
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Swap pivot with the greater element at i
    swap(vec[i + 1], vec[high]);

    // Return the partition point
    return (i + 1);
}

// Function to perform quicksort on a vector
void quickSort(vector<Pokemon>& vec, int low, int high, int& comparisons) {
    if (low < high) {
        // Find the pivot element such that elements smaller than pivot are on the left of pivot,
        // and elements greater than pivot are on the right of pivot
        int pi = partition(vec, low, high, comparisons);

        // Recursive call on the left of pivot
        quickSort(vec, low, pi - 1, comparisons);

        // Recursive call on the right of pivot
        quickSort(vec, pi + 1, high, comparisons);
    }
}



// Driver code
int main() {
        vector<string> fileNames = {
            "pokemonRandomLarge.csv",
            "pokemonRandomMedium.csv",
            "pokemonRandomSmall.csv",
            "pokemonReverseSortedLarge.csv",
            "pokemonReverseSortedMedium.csv",
            "pokemonReverseSortedSmall.csv",
            "pokemonSortedLarge.csv",
            "pokemonSortedMedium.csv",
            "pokemonSortedSmall.csv"
        };
        int comparisons = 0;
        vector<Pokemon> pokemons;
        
        // Iterate over each file
        for (const string& fileName : fileNames) {
            // Open the CSV file
            ifstream file(fileName);
            if (!file.is_open()) {
                std::cout << "Error opening file: " << fileName << endl;
                continue; // Continue to next file
            }

            // Read the CSV file line by line
            string line = "";
            getline(file, line);
            line = "";
            while (getline(file, line)) // read whole line into line
            {
                double name;
                int stats;
                string tempString;

                stringstream inputString(line);

                //Get name of pokemon
                getline(inputString, tempString, ',');
                name = atof(tempString.c_str());

                //Get stats of pokemon
                tempString = "";
                getline(inputString, tempString, ',');
                stats = atoi(tempString.c_str());

                Pokemon pokemon(name, stats);

               
                pokemons.push_back(pokemon);
                line = "";
            }
            //creates a temporary vector Pokemon 
            vector<Pokemon> temp;
            temp = pokemons;
           
            file.close(); // Close the file
            std::cout << "File Name: " << fileName << "\n"<<endl;
            std::cout << "Initial Order: ";
            for (const Pokemon& pokemon : pokemons) {
                std::cout << pokemon.name << ", ";
            }
            std::cout << endl;
            
            // Perform insertion sort
            insertionSort(pokemons, comparisons);
            std::cout << "\nOrder after Insertion Sort: ";
            for (const Pokemon& pokemon : pokemons) {
                std::cout << pokemon.name << ", ";
            }
            std::cout << "\n";
            std::cout << "\nNumber of Comparisons Insertion Sort: " << comparisons << endl;
            comparisons = 0;
            pokemons.clear();
            pokemons = temp;

            //Perform Merge Sort
            mergeSort(pokemons, 0, pokemons.size()-1, comparisons);
            std::cout << "\nOrder after Merge Sort: ";
            for (const Pokemon& pokemon : pokemons) {
                std::cout << pokemon.name << ", ";
            }
            std::cout << "\n";
            std::cout << "\nNumber of Comparisons Merge Sort: " << comparisons << endl;
            comparisons = 0;
            pokemons.clear();
            pokemons = temp;

            //Perform QuickSort
            quickSort(pokemons, 0, pokemons.size()-1, comparisons);
            std::cout << "\nOrder after QuickSort: ";
            for (const Pokemon& pokemon : pokemons) {
                std::cout << pokemon.name << ", ";
            }
            std::cout << "\n";
            std::cout << "\nNumber of Comparisons QuickSort: " << comparisons << endl;
            pokemons.clear();
            temp.clear();
            std::cout << "\n" << endl;
            comparisons = 0;
        }
        return 0;
  
}
