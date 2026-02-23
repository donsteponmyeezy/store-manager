#include <iostream>
#include <string>
#include "simulation/simulation.hpp"

void runTest(const std::string &filename)
{
    std::cout << "========================================" << std::endl;
    std::cout << "        RUNNING FILE: " << filename << std::endl;
    std::cout << "========================================" << std::endl;

    Simulation sim;

    // Load the input file with variables
    if (sim.loadParameters(filename))
    {
        // Compute and display all values from analytical model
        sim.runAnalyticalModel();

        // Run simulation and process until total_events have been processed
        sim.runSimulation();

        // Display simulation measures for comparison
        sim.printResults();
    }
    else
    {
        std::cerr << "Failed to run simulation for " << filename << ". Check if the file exists." << std::endl;
    }
}

int main()
{
    // Read and process test1.txt and test2.txt
    runTest("test1.txt");

    std::cout << "\n";

    runTest("test2.txt");

    return 0;
}