/*
Daniel Gregory
4/10/2024
CS 315 Project 3
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    string destination;
    int weight;

    Edge(const string& dest, int w) : destination(dest), weight(w) {}
};

// Function to read vertices from MiddleEarthVertices.txt
vector<string> readVertices(const string& filename) {
    vector<string> vertices;
    ifstream file("MiddleEarthVertices.txt");
    if (file.is_open()) {
        string vertex;
        while (getline(file, vertex)) {
            vertices.push_back(vertex);
        }
        file.close();
    }
    return vertices;
}

// Function to read edges from MiddleEarthEdges.txt and construct the graph
unordered_map<string, vector<Edge>> constructGraph(const vector<string>& vertices, const string& filename) {
    unordered_map<string, vector<Edge>> graph;
    ifstream file("MiddleEarthEdges.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string source, dest;
            int weight;
            size_t pos = line.find(',');
            source = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(',');
            dest = line.substr(0, pos);
            line.erase(0, pos + 1);
            weight = stoi(line);

            // Add edge from source to destination
            graph[source].push_back(Edge(dest, weight));
            // Add edge from destination to source for undirected graph
            graph[dest].push_back(Edge(source, weight));
        }
        file.close();
    }
    return graph;
}
class MinHeap {
private:
    vector<Edge> heap; // Redefined Edge struct here

    // Helper function to maintain heap property by swapping elements upwards
    void heapifyUp(int index) {
        while (index > 0 && heap[parent(index)].weight > heap[index].weight) {
            swap(heap[parent(index)], heap[index]);
            index = parent(index);
        }
    }

    // Helper function to maintain heap property by swapping elements downwards
    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        if (left < heap.size() && heap[left].weight < heap[smallest].weight) {
            smallest = left;
        }
        if (right < heap.size() && heap[right].weight < heap[smallest].weight) {
            smallest = right;
        }
        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    // Constructor
    MinHeap() {}

    // Insert a new element into the heap
    void insert(Edge value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // Extract the minimum element from the heap
    Edge extractMin() {
        if (heap.empty()) {
            cout << "Heap is empty!" << endl;
            return Edge("", -1); // or throw an exception
        }
        Edge minVal = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return minVal;
    }

    // Check if the heap is empty
    bool isEmpty() {
        return heap.empty();
    }

private:
    // Helper functions to get parent, left child, and right child indices
    int parent(int index) {
        return (index - 1) / 2;
    }

    int leftChild(int index) {
        return 2 * index + 1;
    }

    int rightChild(int index) {
        return 2 * index + 2;
    }
};
// Function to perform Breadth First Search and print shortest path from source to destination
void BFS(unordered_map<string, vector<Edge>>& graph, const string& source, const string& destination) {
    queue<string> q;
    unordered_map<string, bool> visited;
    unordered_map<string, string> parent;

    // Initialize visited map
    for (auto& vertex : graph) {
        visited[vertex.first] = false;
    }

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == destination) {
            stack<string> path;
            string node = destination;
            while (node != source) {
                path.push(node);
                node = parent[node];
            }
            path.push(source);
            cout << "Shortest path from " << source << " to " << destination << " using BFS: \n";
            while (!path.empty()) {
                cout << path.top();
                path.pop();
                if (!path.empty()) cout << " -> ";
            }
            cout << endl;
            return;
        }

        for (const auto& edge : graph[current]) {
            if (!visited[edge.destination]) {
                q.push(edge.destination);
                visited[edge.destination] = true;
                parent[edge.destination] = current;
            }
        }
    }

    cout << "No path exists from " << source << " to " << destination << endl;
}
// Function to find the shortest path from source to all other vertices using Dijkstra's algorithm
void dijkstra(unordered_map<string, vector<Edge>>& graph, const string& source, const string& destination) {
    // Initialize distance map with infinite distance for all vertices except source
    unordered_map<string, int> distance;
    unordered_map<string, string> predecessor; // Store predecessors of vertices in shortest path
    for (const auto& vertex : graph) {
        distance[vertex.first] = numeric_limits<int>::max();
    }
    distance[source] = 0;

    // Priority queue to store vertices with minimum distance
    MinHeap minHeap;
    minHeap.insert(Edge(source, 0));

    // Loop until priority queue becomes empty
    while (!minHeap.isEmpty()) {
        // Extract the vertex with minimum distance from priority queue
        Edge minEdge = minHeap.extractMin();
        string current = minEdge.destination;
        int dist = minEdge.weight;

        // Break if the destination is reached
        if (current == destination) break;

        // Iterate over the neighbors of current vertex
        for (const auto& edge : graph[current]) {
            // Calculate the new distance via current vertex
            int newDist = dist + edge.weight;

            // If new distance is shorter than the current distance, update it and store predecessor
            if (newDist < distance[edge.destination]) {
                distance[edge.destination] = newDist;
                predecessor[edge.destination] = current;
                minHeap.insert(Edge(edge.destination, newDist));
            }
        }
    }

    // Print the shortest path from source to destination
    if (distance[destination] == numeric_limits<int>::max()) {
        cout << "No path exists from " << source << " to " << destination << endl;
    }
    else {
        // Reconstruct the shortest path using predecessors
        stack<string> path;
        string node = destination;
        while (node != source) {
            path.push(node);
            node = predecessor[node];
        }
        path.push(source);

        // Print the shortest path with arrows
        cout << "Shortest Path from " << source << " to " << destination << " using Dijkstra's:\n";
        while (!path.empty()) {
            cout << path.top();
            path.pop();
            if (!path.empty()) cout << " -> ";
        }
        cout << endl;
    }
}

// Function to find the minimum spanning tree using Prim's algorithm
unordered_map<string, string> prim(unordered_map<string, vector<Edge>>& graph, const string& source) {
    // Initialize a set to keep track of visited vertices
    unordered_set<string> visited;
    // Initialize a priority queue to store edges with minimum weight
    priority_queue<pair<int, pair<string, string>>, vector<pair<int, pair<string, string>>>, greater<pair<int, pair<string, string>>>> pq;
    // Initialize the predecessor hash table
    unordered_map<string, string> predecessor;

    // Mark the source vertex as visited and initialize predecessor for Hobbiton
    visited.insert(source);
    predecessor[source] = "Null";

    // Add all edges from the source vertex to the priority queue
    for (const auto& edge : graph[source]) {
        pq.push({ edge.weight, {source, edge.destination} });
    }

    // Iterate until all vertices are visited
    while (visited.size() < graph.size()) {
        // Extract the edge with minimum weight from the priority queue
        int weight = pq.top().first;
        string from = pq.top().second.first;
        string to = pq.top().second.second;
        pq.pop();

        // If the destination vertex is not visited, add the edge to the minimum spanning tree
        if (visited.find(to) == visited.end()) {
            predecessor[to] = from;
            visited.insert(to);
            // Add all edges from the destination vertex to the priority queue
            for (const auto& edge : graph[to]) {
                pq.push({ edge.weight, {to, edge.destination} });
            }
        }
    }

    return predecessor;
}
int main() {
    // Read vertices from file
    vector<string> vertices = readVertices("MiddleEarthVertices.txt");

    // Construct the graph
    unordered_map<string, vector<Edge>> graph = constructGraph(vertices, "MiddleEarthEdges.txt");

    // Print vertices and their adjacent vertices
    cout << "Graph of Middle Earth:" << endl;
    for (const auto& vertex : vertices) {
        cout << vertex << ": ";
        for (const auto& edge : graph[vertex]) {
            cout << edge.destination << ", ";
        }
        cout << endl;
    }

    // Calculate and print the shortest path from Hobbiton to Mt. Doom using BFS
    string hobbiton = "Hobbiton";
    string mtDoom = "MountDoom";
    cout << endl;
    BFS(graph, hobbiton, mtDoom);
    cout << endl;

    // Calculate and print the shortest path from Hobbiton to Mt. Doom using Dijkstra's algorithm
    dijkstra(graph, "Hobbiton", "MountDoom");

    // Calculate and print the predecessor hash table for Prim's algorithm
    cout << endl <<"Predecessor hash table for Prim's" << endl;
    unordered_map<string, string> predecessorTable = prim(graph, "Hobbiton");
    for (const auto& pair : predecessorTable) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}