//
//  GeneticAlgorithm.hpp
//  Genetic Algorithm
//
//  Created by Nour Charaf on 1/9/20.
//  Copyright © 2020 Apple. All rights reserved.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include "Chromosome.hpp"

class GeneticAlgorithm{
    
public:
    
    std::vector<Chromosome *> chromosomes;
    
    double randomNumber();
    unsigned selectWithProbability(double probability);
    template<typename Iter> Iter selectRandomly(Iter start, Iter end);
    
    void processGenerations();

    void initializePopulation();
    
    void randomizeGenes(Chromosome *chromosome);
    char randomGene();
    
    void processGeneration(unsigned generationNumber);
    
    void calculatePopulationFitness();
    void calculateChromosomeFitness(Chromosome *chromosome);
    
    void sortPopulation();
    
    std::vector<Chromosome *> selectElitePopulation();
    
    std::vector<Chromosome *> selectSurvivingPopulation();
    
    std::vector<Chromosome *> crossover(std::vector<Chromosome *> survivingChromosomes);
    
    void mutation(std::vector<Chromosome *> offspringChromosomes);
    
    void logGenerationResults(unsigned generationNumber, Chromosome *chromosome);
};

#endif /* GeneticAlgorithm_hpp */
