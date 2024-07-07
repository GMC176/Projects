Description:
	This program simulates a set-associative cache memory based on the provided trace file. It calculates cache hits, cache misses, set-wise total misses, and set-wise total hits for each run.
	this also shows addresses present in particular set,for hexadecimal input programme,32 byte address is depicted in binary
Zip file includes:
1. myobject.java (The program to simulate cache memory for binary input trace file)
2. myobjects.java (The program to simulate cache memory for hexadecimal trace file)
3. four trace files which have address in binary and hexadecimal
bbb in hexadecimal
check1 in binary
check in binary
b in hexadecimal

Usage:
1. Download the source code files.

2. Open a terminal or command prompt and navigate to the project directory.

3. Compile the Java source files using the following command:
->	javac (myobject)/(myobjects).java	

4. Run the program with the required command line arguments:
->	java myobject <Cache size> <Associativity> <Block Size> <Trace file path>

5. The program will simulate the set-associative cache memory based on the provided trace file and display the results.




Simulation Results:
The simulation results include the following information for each run:
	- Set-wise total misses
        - Set-wise total hits
        - Total cache hits
        - Total cache misses
	- present addreses in that set


Trace file conditions:
	1. Addresses must be in straight line with no spaces.
	2. Needs Address in Hexadecimal format or binary format,use programme according to available tracefile.
