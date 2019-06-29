#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

int** matrixMultiply(int** a, int ** b, int n);
void deleteMatrix(int** matrix, int n);
struct VerticesWalks {
    string vertex1;
    string vertex2;
    int numWalks;
    bool operator<(const VerticesWalks& other) const {
        if(numWalks > other.numWalks) {
            return true;
        }
        if(numWalks == other.numWalks) {
            if(vertex1 < other.vertex1) {
                return true;
            }
            else if(vertex1 == other.vertex1) {
                if(vertex2 < other.vertex2) {
                    return true;
                }
            }
        }
        return false;
    }
    bool operator==(const VerticesWalks& other) const {
        return numWalks == other.numWalks && vertex1 == other.vertex1 && vertex2 == other.vertex2;
    }
};

int main(int argc, char* argv[]) {
    ifstream infile(argv[1]);
    string line;
    unordered_map<string, int> stringToIndexMapping; // key: vertex string, value: index in adj matrix
    vector<string> indexToStringMapping; //key: index in adj matrix, value: vertex string
    int index = 0;
    // Load each edge into a hashmap where the edges name is the key and its index
    // in the adjacency matrix is the value (necessary to support string vertex names)
    while (getline(infile, line)) {
        istringstream iss(line);
        string vertex1, vertex2;
        if (!(iss >> vertex1 >> vertex2)) { break; }
        if(stringToIndexMapping.find(vertex1) == stringToIndexMapping.end()) {
            stringToIndexMapping[vertex1] = index;
            index++;
        }
        if(stringToIndexMapping.find(vertex2) == stringToIndexMapping.end()) {
            stringToIndexMapping[vertex2] = index;
            index++;
        }
        if(find(indexToStringMapping.begin(), indexToStringMapping.end(), vertex1) == indexToStringMapping.end())
            indexToStringMapping.push_back(vertex1);
        if(find(indexToStringMapping.begin(), indexToStringMapping.end(), vertex2) == indexToStringMapping.end())
            indexToStringMapping.push_back(vertex2);
    }
    int n = stringToIndexMapping.size();
    
    // Initialize NxN adjacency matrix filled with zeros
    int** matrix = new int*[n];
    for(int i = 0; i < n; i++)
        matrix[i] = new int[n];
        
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            matrix[i][j] = 0;
    
    infile.clear();                 
    infile.seekg(0, std::ios::beg);
    // Go through the file again, this time looking up the two vertices
    // in the hashmap and setting the value in the matrix at 1
    while (getline(infile, line)) {
        istringstream iss(line);
        string vertex1, vertex2;
        if (!(iss >> vertex1 >> vertex2)) { break; }
        int index1 = stringToIndexMapping[vertex1], index2 = stringToIndexMapping[vertex2];
        matrix[index1][index2] = 1;
        matrix[index2][index1] = 1;    
    }
    
    // Perform two matrix multiplications to take the matrix to the third power (n*n)*n
    
    int** temp = matrixMultiply(matrix, matrix, n);
    int** resultMatrix = matrixMultiply(matrix, temp, n);
    deleteMatrix(temp, n);
    deleteMatrix(matrix, n);
    
    // Put the results in an edge list
    vector<VerticesWalks> outputEdgelist;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            int numWalks = resultMatrix[i][j];
            if(numWalks > 0) {
                string vertex1 = indexToStringMapping[i];
                string vertex2 = indexToStringMapping[j];
                if(vertex1 > vertex2) {
                    vertex1 = vertex2;
                    vertex2 = indexToStringMapping[i];
                }
                VerticesWalks walks = {vertex1, vertex2, numWalks};
                outputEdgelist.push_back(walks);
            }
        }     
    }
    
    deleteMatrix(resultMatrix, n);
    sort(outputEdgelist.begin(), outputEdgelist.end());
    outputEdgelist.erase(unique(outputEdgelist.begin(), outputEdgelist.end()), outputEdgelist.end());
    
    for(int i = 0; i < outputEdgelist.size(); i++) {
        VerticesWalks vw = outputEdgelist[i];
        cout << vw.vertex1 << "\t" << vw.vertex2 << "\t" << vw.numWalks << endl;
    }
}

void deleteMatrix(int** matrix, int n) {
    for(int i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;
}
int** matrixMultiply(int** a, int ** b, int n) 
{ 
    int** mul = new int*[n];
    for(int i = 0; i < n; i++)
        mul[i] = new int[n];
        
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) { 
            mul[i][j] = 0; 
            for (int k = 0; k < n; k++) 
                mul[i][j] += a[i][k]*b[k][j]; 
        }
    }
    return mul;
            
}