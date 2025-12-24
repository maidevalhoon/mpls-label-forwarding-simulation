
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iomanip> // Used for formatting cout

using namespace std;

// --- Constants ---
const int NUM_ROUTERS = 4;
const int INF = 9999;       // Represents infinity for link costs
const int POP_LABEL = -1;   // Special value for "pop" operation
const int NO_LABEL = 0;     // Represents an unlabeled packet

// --- Data Structures (as Classes) ---

/**
 * Represents a Forwarding Equivalence Class (FEC).
 * For this assignment, it's defined by a source and destination.
 */
class FEC {
public:
    int source;
    int destination;

    // Default constructor
    FEC() : source(0), destination(0) {}

    // Parameterized constructor
    FEC(int src, int dst) : source(src), destination(dst) {}

    /**
     * Comparison operator is required to use FEC as a key in std::map.
     * It defines how to sort FECs.
     */
    bool operator<(const FEC &other) const {
        if (source != other.source) {
            return source < other.source;
        }
        return destination < other.destination;
    }
};

/**
 * Represents a Label Operation (the value in an LFIB).
 * Tells the router the outgoing label and the next hop.
 */
class LabelOp {
public:
    int outLabel;
    int nextHop;

    // Default constructor
    LabelOp() : outLabel(0), nextHop(-1) {}

    // Parameterized constructor
    LabelOp(int label, int hop) : outLabel(label), nextHop(hop) {}
};

/**
 * Represents a network packet.
 */
class Packet {
public:
    int source;
    int destination;
    int label;

    Packet(int src, int dst) : source(src), destination(dst), label(NO_LABEL) {}
};

// --- Network State (Global Variables) ---

// Adjacency matrix for the network topology
vector<vector<int>> topologyMatrix;

// Ingress LFIB: Maps a FEC to a label operation
map<FEC, LabelOp> ingressLFIB;

// Transit/Egress LFIBs: Maps an In-Label to a label operation
map<int, LabelOp> transitLFIB[NUM_ROUTERS];

// --- Function Prototypes ---
void processR2(Packet &packet);

void processR3(Packet &packet);

void printRoutingTable(int sourceRouter, const vector<int> &prev, const vector<int> &dist);

// --- Task 1: Represent the Topology ---

/**
 * Initializes the global topologyMatrix with fixed costs.
 */
void initializeTopology() {
    topologyMatrix.assign(NUM_ROUTERS, vector<int>(NUM_ROUTERS, INF));

    for (int i = 0; i < NUM_ROUTERS; ++i) {
        topologyMatrix[i][i] = 0;
    }
    // R0 <-> R1 (Cost: 10)
    topologyMatrix[0][1] = 10;
    topologyMatrix[1][0] = 10;
    // R0 <-> R2 (Cost: 20)
    topologyMatrix[0][2] = 20;
    topologyMatrix[2][0] = 20;
    // R1 <-> R3 (Cost: 20)
    topologyMatrix[1][3] = 20;
    topologyMatrix[3][1] = 20;
    // R2 <-> R3 (Cost: 10)
    topologyMatrix[2][3] = 10;
    topologyMatrix[3][2] = 10;
}

// --- Task 2: Compute Routing Tables (RIB) ---

/**
 * Implements Dijkstra's algorithm to compute the RIB for a router.
 */
void runDijkstra(int sourceRouter) {
    vector<int> dist(NUM_ROUTERS, INF);
    vector<int> prev(NUM_ROUTERS, -1);
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<>> pq;

    dist[sourceRouter] = 0;
    pq.push({0, sourceRouter});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < NUM_ROUTERS; ++v) {
            if (topologyMatrix[u][v] != INF) {
                int newDist = d + topologyMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    pq.push({newDist, v});
                }
            }
        }
    }

    // Print the header
    cout << "Routing Table for Router R" << sourceRouter << ":" << endl;
    cout << left;
    cout << " " << setw(12) << "Destination" << "|";
    cout << " " << setw(9) << "Next Hop" << "|";
    cout << " " << setw(10) << "Total Cost" << endl;
    cout << "------------------------------------" << endl;

    printRoutingTable(sourceRouter, prev, dist);
}

/**
 * Prints the formatted rows for a router's RIB.
 */

void printRoutingTable(int sourceRouter, const vector<int> &prev,
                       const vector<int> &dist) {
    cout << left; // Left-align text

    for (int i = 0; i < NUM_ROUTERS; ++i) {
        cout << " " << setw(12) << ("R" + to_string(i));
        cout << "|";
        if (dist[i] == INF) {
            cout << " " << setw(9) << "-";
            cout << "|";
            cout << " " << setw(10) << "INF" << endl;
            continue;
        }
        if (i == sourceRouter) {
            cout << " " << setw(9) << ("R" + to_string(i));
            cout << "|";
            cout << " " << setw(10) << 0 << endl;
            continue;
        }
        int curr = i;
        while (prev[curr] != sourceRouter && prev[curr] != -1) {
            curr = prev[curr];
        }
        cout << " " << setw(9) << ("R" + to_string(curr));
        cout << "|";
        cout << " " << setw(10) << dist[i] << endl;
    }
    cout << endl;
}



// --- Task 4: Create Label Forwarding Tables (LFIB) ---

/**
 * Initializes the global LFIBs with the fixed values.
 */
void initializeLFIB() {
    // R0 (Ingress)
    // FEC (R0->R3) -> Push Label 300, Next Hop R2
    ingressLFIB[FEC(0, 3)] = LabelOp(300, 2);

    // R2 (Transit)
    // In-Label 300 -> Swap for Label 777, Next Hop R3
    transitLFIB[2][300] = LabelOp(777, 3);

    // R3 (Egress)
    // In-Label 777 -> Pop Label
    transitLFIB[3][777] = LabelOp(POP_LABEL, -1);
}

// --- Task 5: Simulate Packet Forwarding ---

/**
 * Simulates packet processing at the Ingress Router (R0).
 */
void processR0(Packet &packet) {
    // We now use the FEC class constructor
    FEC fec(packet.source, packet.destination);

    LabelOp operation = ingressLFIB.at(fec);
    int newLabel = operation.outLabel;
    int nextHop = operation.nextHop;

    packet.label = newLabel; // Push operation

    cout << "[R0] Packet for R3 (FEC: R0->R3). "
         << "Pushing Label " << packet.label
         << ". Sending to R" << nextHop << "." << endl;

    processR2(packet);
}

/**
 * Simulates packet processing at the Transit Router (R2).
 */
void processR2(Packet &packet) {
    int inLabel = packet.label;

    LabelOp operation = transitLFIB[2].at(inLabel);
    int newLabel = operation.outLabel; // Swap operation
    int nextHop = operation.nextHop;

    packet.label = newLabel; // Apply the swap

    cout << "[R2] Received packet with In-Label " << inLabel
         << ". Swapping for Out-Label " << packet.label
         << ". Sending to R" << nextHop << "." << endl;

    processR3(packet);
}

/**
 * Simulates packet processing at the Egress Router (R3).
 */
void processR3(Packet &packet) {
    int inLabel = packet.label;

    LabelOp operation = transitLFIB[3].at(inLabel);
    int action = operation.outLabel;

    if (action == POP_LABEL) {
        packet.label = NO_LABEL; // Pop operation
    }

    cout << "[R3] Received packet with In-Label " << inLabel
         << ". Popping label. Packet delivered." << endl;
}

// --- Main Function ---

int main() {
    // Task 1: Initialize topology
    initializeTopology();

    // Task 2: Compute and print all RIBs
    for (int i = 0; i < NUM_ROUTERS; ++i) {
        runDijkstra(i);
    }

    // Task 4: Set up the LFIBs
    initializeLFIB();

    // Task 5: Simulate packet forwarding
    cout << "Simulating packet from R0 to R3..." << endl;

    // Create the packet
    Packet packet(0, 3);

    // Start the simulation
    processR0(packet);

    return 0;
}
