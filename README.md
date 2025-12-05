# **Project 06: Flight Plan (Dijkstra's Algorithm)**

## Project Overview

This project implements Dijkstra's Algorithm to determine the optimal flight plans between cities. It parses a graph represented as an adjacency matrix where edge weights contain compound data (Time and Cost). The program processes user queries to find either the Shortest Time or the Cheapest Price between two cities.

### System Environment

**Operating System**: Tested on Linux and Windows

**Compiler**: g++ (GCC) supporting C++11 standard or higher

**Language**: C++

### Compilation Instructions

To compile the source code into the required executable name p06, open a terminal in the project directory and run the following command:

`make`


### Execution

The program requires three command-line arguments: an input graph file, a query file, and an output file name.

Command Syntax:

`./p06 <input_file> <query_file> <output_file>`


Example Run:

`./p06 input2.dat query2.dat output2.dat`


### File Format Specifications

#### **1. Graph Input File** (`input2.dat`)

This file contains the adjacency matrix representing the flight network.

**Line 1**: Integer N (Number of cities).

**Line 2**: Space-separated list of N city names.

**Lines 3 to N+2**: The N x N adjacency matrix.

  - Each entry is formatted as Time|Cost.

  - INF|INF indicates no direct connection.

  - Example: 144|192 represents 144 minutes and $192.

#### **2. Query File** (`query2.dat`)

This file contains the list of flight requests.

- Each line contains: SourceCity DestinationCity Mode

- Mode 'T': Calculates the path with the shortest time.

- Mode 'C': Calculates the path with the cheapest cost.

- Example: `Austin Chicago T`

#### 3. Output File (`output2.dat`)

The program writes the result of each query to this file.

- If a path exists: Prints the total weight (integer).

- If no path exists: Prints "No available path."

### Implementation Details

**Data Parsing**: The program splits the Time|Cost string to build two logical graphs (one for Time, one for Cost) or modifies the Dijkstra relaxation step to select the correct weight based on the query mode.

**Algorithm**: Uses Dijkstra's algorithm with a priority queue (or standard iterative approach) to ensure O(V^2) or O(E + V log V) complexity.
