//
//  Parameters.hpp
//  Genetic Algorithm
//
//  Created by Nour Charaf on 1/9/20.
//  Copyright © 2020 Apple. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>

// Input Parameters
static std::string allowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678910 ,':;.!?<>";
static std::string inputString = "Experiment 626: Is this text used as an example for this GA? ; Yes it is !";
static std::vector<char> input(inputString.begin(), inputString.end());

// Genetic Algorithm Parameters
static unsigned populationSize = 1000;
static unsigned numberOfGenerations = 10000;
static double elitePercentage = 0.05;
static double survivingPercentage = 0.5;
static double crossoverProbability = 0.5;
static double mutationProbability = 0.01;
static double minimumDesiredFitnessPercentage = 0.99;

static unsigned elitePopulationSize = unsigned(elitePercentage * populationSize);
static unsigned survivingPopulationSize = unsigned(survivingPercentage * populationSize);
static unsigned offspringPopulationSize = populationSize - elitePopulationSize;
static double minimumDesiredFitness = input.size() * minimumDesiredFitnessPercentage;

#endif /* Parameters_hpp */
