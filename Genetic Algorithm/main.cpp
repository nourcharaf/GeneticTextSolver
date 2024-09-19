//
//  main.cpp
//  Genetic Algorithm
//
//  Created by Nour Charaf on 1/9/20.
//  Copyright © 2020 Apple. All rights reserved.
//

#include "Parameters.hpp"
#include "GeneticAlgorithm.hpp"

int main() {
    
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Genetic Algorithm
    GeneticAlgorithm *geneticAlgorithm = new GeneticAlgorithm();
    
    // Process Generations
    geneticAlgorithm->processGenerations();
    
    return 0;
}
