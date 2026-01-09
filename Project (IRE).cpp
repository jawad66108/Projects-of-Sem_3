#include <iostream>
#include <cstring>
using namespace std;

const int N = 8;
string cities[N] = {
    "Karachi",     
    "Hyderabad",   
    "Sukkur",      
    "Multan",      
    "Lahore",      
    "Islamabad",   
    "Peshawar",    
    "Murree"       
};

const int MAX_EDGES = 30;
const int INF = 99999;

struct Edge {
    int to;        // destination city index
    int weight;    // distance / cost
    int next;      // index of next edge
};

Edge edges[MAX_EDGES];
int head[N];
int edgeCount = 0;


// ========== FUNCTION DECLARATIONS ==========
void printSeparator(char ch = '=', int width = 60);
void printHeader(string text);
void printSubHeader(string text);
void showCities();
void initGraph();
void addEdge(int u, int v, int w);
void buildGraph();
void BFS(int start, int goal);
void DFS(int start, int goal);
void UCS(int start, int goal);
void Dijkstra(int start, int goal);
void AStar(int start, int goal);
// ============================================



void showCities() {
    cout << "\n  Available Cities:\n";
    cout << "  " << string(50, '-') << "\n";
    for (int i = 0; i < N; i++) {
        cout << "  " << i << ". " << cities[i] << endl;
    }
    cout << "  " << string(50, '-') << "\n";
}

void printSeparator(char ch, int width) {
    for (int i = 0; i < width; i++) {
        cout << ch;
    }
    cout << endl;
}

void printHeader(string text) {
    cout << "\n";
    printSeparator('=', 60);
    cout << "  " << text << endl;
    printSeparator('=', 60);
}

void printSubHeader(string text) {
    cout << "\n  +- " << text << " -+\n" << endl;
}

void initGraph() {
    for (int i = 0; i < N; i++) {
        head[i] = -1;
    }
}

void addEdge(int u, int v, int w) {
    edges[edgeCount].to = v;
    edges[edgeCount].weight = w;
    edges[edgeCount].next = head[u];
    head[u] = edgeCount++;

    edges[edgeCount].to = u;
    edges[edgeCount].weight = w;
    edges[edgeCount].next = head[v];
    head[v] = edgeCount++;
}

void buildGraph() {
    initGraph();

    addEdge(0, 1, 165);   // Karachi - Hyderabad
    addEdge(1, 2, 330);   // Hyderabad - Sukkur
    addEdge(2, 3, 260);   // Sukkur - Multan
    addEdge(3, 4, 340);   // Multan - Lahore
    addEdge(4, 5, 270);   // Lahore - Islamabad
    addEdge(5, 6, 180);   // Islamabad - Peshawar
    addEdge(5, 7, 65);    // Islamabad - Murree
}

int queueArr[50];
int front = 0;
int rear = 0;

void enqueue(int x) {
    queueArr[rear++] = x;
}

int dequeue() {
    return queueArr[front++];
}

bool isEmpty() {
    return front == rear;
}

void BFS(int start, int goal) {
    bool visited[N] = {false};
    int parent[N];

    for (int i = 0; i < N; i++) {
        parent[i] = -1;
    }

    front = rear = 0;

    enqueue(start);
    visited[start] = true;

    while (!isEmpty()) {
        int current = dequeue();

        if (current == goal) {
            break;
        }

        for (int i = head[current]; i != -1; i = edges[i].next) {
            int nextCity = edges[i].to;

            if (!visited[nextCity]) {
                visited[nextCity] = true;
                parent[nextCity] = current;
                enqueue(nextCity);
            }
        }
    }

    cout << "\n ==> BFS Path (Minimum Stops):\n";
    cout << "  " << string(50, '-') << "\n";
    int path[20];
    int count = 0;
    int temp = goal;

    while (temp != -1) {
        path[count++] = temp;
        temp = parent[temp];
    }

    if (parent[goal] == -1 && start != goal) {
        cout << "  No path found!\n";
    } else {
        cout << "  Route: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << cities[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n  Total Stops: " << (count - 1) << endl;
    }
    cout << "  " << string(50, '-') << "\n";
}

bool dfsFound = false;
int dfsParent[8];

void DFSUtil(int current, int goal, bool visited[]) {
    visited[current] = true;

    if (current == goal) {
        dfsFound = true;
        return;
    }

    for (int i = head[current]; i != -1; i = edges[i].next) {
        int nextCity = edges[i].to;

        if (!visited[nextCity] && !dfsFound) {
            dfsParent[nextCity] = current;
            DFSUtil(nextCity, goal, visited);
        }
    }
}

void DFS(int start, int goal) {
    bool visited[N] = {false};
    dfsFound = false;

    for (int i = 0; i < N; i++) {
        dfsParent[i] = -1;
    }

    DFSUtil(start, goal, visited);

    cout << "\n  ==> DFS Traversal Path:\n";
    cout << "  " << string(50, '-') << "\n";

    if (!dfsFound && start != goal) {
        cout << "  No path found!\n";
    } else {
        int path[20];
        int count = 0;
        int temp = goal;

        while (temp != -1) {
            path[count++] = temp;
            temp = dfsParent[temp];
        }

        cout << "  Route: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << cities[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n  Total Stops: " << (count - 1) << endl;
    }
    cout << "  " << string(50, '-') << "\n";
}

int findMinCostNode(int cost[], bool visited[]) {
    int minCost = INF;
    int minIndex = -1;

    for (int i = 0; i < N; i++) {
        if (!visited[i] && cost[i] < minCost) {
            minCost = cost[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void UCS(int start, int goal) {
    int cost[N];
    bool visited[N];
    int parent[N];

    for (int i = 0; i < N; i++) {
        cost[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    cost[start] = 0;

    while (true) {
        int current = findMinCostNode(cost, visited);

        if (current == -1)
            break;

        if (current == goal)
            break;

        visited[current] = true;

        for (int i = head[current]; i != -1; i = edges[i].next) {
            int nextCity = edges[i].to;
            int weight = edges[i].weight;

            if (!visited[nextCity]) {
                if (cost[current] + weight < cost[nextCity]) {
                    cost[nextCity] = cost[current] + weight;
                    parent[nextCity] = current;
                }
            }
        }
    }

    cout << "\n  ==> UCS Path (Minimum Cost):\n";
    cout << "  " << string(50, '-') << "\n";
    int path[20];
    int count = 0;
    int temp = goal;

    while (temp != -1) {
        path[count++] = temp;
        temp = parent[temp];
    }

    if (parent[goal] == -1 && start != goal) {
        cout << "  No path found!\n";
    } else {
        cout << "  Route: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << cities[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n  Total Cost: " << cost[goal] << " km\n";
    }
    cout << "  " << string(50, '-') << "\n";
}

int findMinDistanceNode(int dist[], bool visited[]) {
    int minDist = INF;
    int minIndex = -1;

    for (int i = 0; i < N; i++) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void Dijkstra(int start, int goal) {
    int dist[N];
    bool visited[N];
    int parent[N];

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int count = 0; count < N - 1; count++) {
        int current = findMinDistanceNode(dist, visited);

        if (current == -1)
            break;

        visited[current] = true;

        for (int i = head[current]; i != -1; i = edges[i].next) {
            int nextCity = edges[i].to;
            int weight = edges[i].weight;

            if (!visited[nextCity] && dist[current] + weight < dist[nextCity]) {
                dist[nextCity] = dist[current] + weight;
                parent[nextCity] = current;
            }
        }
    }

    cout << "\n  ==> Dijkstra Shortest Path:\n";
    cout << "  " << string(50, '-') << "\n";
    int path[20];
    int countPath = 0;
    int temp = goal;

    while (temp != -1) {
        path[countPath++] = temp;
        temp = parent[temp];
    }

    if (parent[goal] == -1 && start != goal) {
        cout << "  No path found!\n";
    } else {
        cout << "  Route: ";
        for (int i = countPath - 1; i >= 0; i--) {
            cout << cities[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n  Total Distance: " << dist[goal] << " km\n";
    }
    cout << "  " << string(50, '-') << "\n";
}

int heuristic[N] = {
    1400, // Karachi ? Islamabad
    1200, // Hyderabad ? Islamabad
    900,  // Sukkur ? Islamabad
    600,  // Multan ? Islamabad
    300,  // Lahore ? Islamabad
    0,    // Islamabad ? Islamabad
    180,  // Peshawar ? Islamabad
    65    // Murree ? Islamabad
};

int findMinFNode(int gCost[], bool visited[]) {
    int minF = INF;
    int minIndex = -1;

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            int f = gCost[i] + heuristic[i];
            if (f < minF) {
                minF = f;
                minIndex = i;
            }
        }
    }
    return minIndex;
}

void AStar(int start, int goal) {
    int gCost[N];
    bool visited[N];
    int parent[N];

    for (int i = 0; i < N; i++) {
        gCost[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    gCost[start] = 0;

    while (true) {
        int current = findMinFNode(gCost, visited);

        if (current == -1)
            break;

        if (current == goal)
            break;

        visited[current] = true;

        for (int i = head[current]; i != -1; i = edges[i].next) {
            int nextCity = edges[i].to;
            int weight = edges[i].weight;

            if (!visited[nextCity]) {
                if (gCost[current] + weight < gCost[nextCity]) {
                    gCost[nextCity] = gCost[current] + weight;
                    parent[nextCity] = current;
                }
            }
        }
    }

    cout << "\n  A* Path (Smart & Fast):\n";
    cout << "  " << string(50, '-') << "\n";
    int path[20];
    int count = 0;
    int temp = goal;

    while (temp != -1) {
        path[count++] = temp;
        temp = parent[temp];
    }

    if (parent[goal] == -1 && start != goal) {
        cout << "  ? No path found!\n";
    } else {
        cout << "  Route: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << cities[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n  Total Cost: " << gCost[goal] << " km\n";
    }
    cout << "  " << string(50, '-') << "\n";
}

int main() {
    buildGraph();
    
    bool continueLoop = true;
    int searchCount = 0;
    
    while (continueLoop) {
        if (searchCount == 0) {
            printHeader("INTELLIGENT ROUTE ENGINE");
        } else {
            printHeader("FIND ANOTHER ROUTE");
        }
        showCities();
        
        int source, destination;
        cout << "\n   Enter source city index: ";
        cin >> source;

        cout << "   Enter destination city index: ";
        cin >> destination;

        // Validate input
        if (source < 0 || source >= N || destination < 0 || destination >= N) {
            cout << "\n";
            printSeparator('-', 60);
            cout << "  ? Invalid city index! Please enter values between 0-7.\n";
            printSeparator('-', 60);
            cout << "\n";
            continue;
        }

        if (source == destination) {
            cout << "\n";
            printSeparator('-', 60);
            cout << "  ? Source and destination are the same!\n";
            cout << "  Path: " << cities[source] << endl;
            printSeparator('-', 60);
            cout << "\n";
            
            char another;
            cout << "  Wana Find another route? (Y/N): ";
            cin >> another;
            if (another != 'Y' && another != 'y') {
                continueLoop = false;
            }
            cout << "\n";
            searchCount++;
            continue;
        }

        printSubHeader("ROUTE PREFERENCE OPTIONS");
        cout << "  1. Minimum number of stops (Fast traversal)\n";
        cout << "  2. Cheapest route (Minimum cost)\n";
        cout << "  3. Shortest distance (Accurate shortest path)\n";
        cout << "  4. Safest & fast route (Heuristic based)\n";
        cout << "\n  ? Enter your choice (1-4): ";
        int choice;
        cin >> choice;

        cout << "\n";
        printSeparator('-', 60);

        switch (choice) {
            case 1:
                cout << "  ==> You selected: Minimum Stops Route\n";
                printSeparator('-', 60);
                BFS(source, destination);
                break;

            case 2:
                cout << "  ==> You selected: Cheapest Route\n";
                printSeparator('-', 60);
                UCS(source, destination);
                break;

            case 3:
                cout << "  ==> You selected: Shortest Distance Route\n";
                printSeparator('-', 60);
                Dijkstra(source, destination);
                break;

            case 4:
                cout << "  ==> You selected: Safest & Fast Route\n";
                printSeparator('-', 60);
                AStar(source, destination);
                break;

            default:
                cout << "  ? Invalid choice! Please enter 1-4.\n";
                printSeparator('-', 60);
                cout << "\n";
                continue;
        }

        printSeparator('-', 60);
        
        char another;
        cout << "\n  Wana Find another route? (Y/N): ";
        cin >> another;
        
        if (another != 'Y' && another != 'y') {
            continueLoop = false;
        }
        cout << "\n";
        searchCount++;
    }

    printHeader("THANK YOU FOR USING THE SYSTEM");
    cout << "  +-- Total routes searched: " << searchCount <<"  --+"<< endl;
    cout << "  Goodbye! Safe travels! ??\n";
    printSeparator('=', 60);
    cout << "\n";

    return 0;
}
