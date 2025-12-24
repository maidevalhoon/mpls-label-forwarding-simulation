Group 50
Aeyaz Adil (230101007) 
Deval Singhal (230101035) 
Jayant Kumar (230101050) 
Prayansh Kumar (230101081) 

# MPLS Label Forwarding Simulation

This project simulates the working of Multiprotocol Label Switching (MPLS) using a fixed 4-router topology. The assignment demonstrates how routing tables (RIB) and label forwarding tables (LFIB) are created and how a labeled packet is forwarded through the network.

This work is done as part of the Computer Networks Laboratory – Assignment 5.

Objective: To understand MPLS forwarding by computing routing tables using Dijkstra’s algorithm, defining a Forwarding Equivalence Class (FEC), creating label forwarding tables, and simulating packet traversal using label push, swap, and pop operations.

Description: A fixed network topology with four routers (R0, R1, R2, R3) is used. Shortest paths are computed for all routers to build routing tables (RIB). A single FEC is defined for traffic originating from R0 and destined for R3. Based on the shortest path, MPLS label forwarding tables (LFIB) are constructed. A packet is then simulated as it moves through the network, demonstrating label push at the ingress router, label swap at the transit router, and label pop at the egress router.

Topology:
R0 <-> R1 (Cost 10)
R0 <-> R2 (Cost 20)
R1 <-> R3 (Cost 20)
R2 <-> R3 (Cost 10)

Tasks Implemented:
Representation of network topology
Routing table (RIB) computation using Dijkstra’s algorithm
Definition of a Forwarding Equivalence Class (R0 to R3)
Creation of Label Forwarding Information Base (LFIB)
Simulation of MPLS packet forwarding

Label Operations:
Ingress Router R0 pushes label 300 and forwards to R2
Transit Router R2 swaps label 300 with 777 and forwards to R3
Egress Router R3 pops label 777 and delivers the packet

How to Compile and Run (example for C++):
g++ *.cpp -o mpls
./mpls

Repository Contents:
Source code files
Output file showing routing tables and packet simulation
Assignment PDF for reference
