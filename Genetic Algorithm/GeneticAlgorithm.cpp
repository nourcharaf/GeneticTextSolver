//
//  GeneticAlgorithm.cpp
//  Genetic Algorithm
//
//  Created by Nour Charaf on 1/9/20.
//  Copyright © 2020 Apple. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

std::random_device randomDevice;
std::mt19937 mt19937(randomDevice());

void GeneticAlgorithm::processGenerations(){
    
    // Initialize Population
    GeneticAlgorithm::initializePopulation();
    
    for (unsigned i = 0; i < numberOfGenerations; ++i){
        
        // Process Generation
        processGeneration(i);
        
        // Check if reached desired fitness
        Chromosome *bestChromosome = chromosomes[0];
        if (bestChromosome->fitness >= minimumDesiredFitness){
            std::cout << "Success: Reached Minimum Desired Fitness" << std::endl << std::endl;
            break;
        }
    }
}

void GeneticAlgorithm::initializePopulation(){
    
    for (unsigned i = 0; i < populationSize; ++i){
        
        Chromosome *chromosome = new Chromosome();
        chromosomes.push_back(chromosome);
        
        // Initialize each chromosome with randomized genes
        GeneticAlgorithm::randomizeGenes(chromosome);
    }
}

void GeneticAlgorithm::randomizeGenes(Chromosome *chromosome){
    
    for (unsigned i = 0; i < input.size(); ++i) {
        
        char randomGene = GeneticAlgorithm::randomGene();
        
        chromosome->genes.push_back(randomGene);
    }
}

char GeneticAlgorithm::randomGene(){
    
    unsigned randomIndex = unsigned(randomNumber()*allowedCharacters.length());
    
    return allowedCharacters[randomIndex];
}

double GeneticAlgorithm::randomNumber(){
    return rand()/double(RAND_MAX);
}

void GeneticAlgorithm::processGeneration(unsigned generationNumber){
    
    // Fitness: Calculate Population Fitness
    calculatePopulationFitness();
    
    // Sort: Sort Population based on Fitness
    sortPopulation();
    
    // Elitism: Select Elite Population
    std::vector<Chromosome *> eliteChromosomes = selectElitePopulation();
    
    // Selection: Select Surviving Population: Roulette Wheel Selection
    std::vector<Chromosome *> survivingChromosomes = selectSurvivingPopulation();
    
    // Crossover: Crossover Surviving Population using Uniform Crossover to Generate Offspring Population
    std::vector<Chromosome *>offspringChromosomes = crossover(survivingChromosomes);
    
    // Mutatation: Mutate Offspring Chromosomes
    mutation(offspringChromosomes);
    
    // Replace: Replace existing population with elite population and offspring population
    chromosomes.clear();
    chromosomes.insert(chromosomes.end(), eliteChromosomes.begin(), eliteChromosomes.end());
    chromosomes.insert(chromosomes.end(), offspringChromosomes.begin(), offspringChromosomes.end());
    
    // Keep track of Best Chromosome
    Chromosome *bestChromosome = chromosomes[0];
    
    // Log Results of Best Chromosome
    logGenerationResults(generationNumber, bestChromosome);
}

void GeneticAlgorithm:: calculatePopulationFitness(){
    
    // Fitness
    double sumOfFitness = 0;
    for (unsigned i = 0; i < chromosomes.size(); ++i){
        Chromosome *chromosome = chromosomes[i];
        calculateChromosomeFitness(chromosome);
        sumOfFitness += chromosome->fitness;
    }
    
    // Probability
    for (unsigned i = 0; i < chromosomes.size(); ++i){
        Chromosome *chromosome = chromosomes[i];
        double fitnessValue = chromosome->fitness;
        double probability = (fitnessValue == 0 || sumOfFitness == 0) ? 0.001 : fitnessValue/sumOfFitness;
        chromosome->probability = probability;
    }
}

void GeneticAlgorithm::calculateChromosomeFitness(Chromosome *chromosome){
    
    unsigned sumOfCorrectChars = 0;
    
    for (unsigned i = 0; i < input.size(); ++i){
        
        char inputChar = input[i];
        char geneChar = chromosome->genes[i];
        
        if (geneChar == inputChar){
            sumOfCorrectChars += 1;
        }
    }
    
    // Set Fitness
    chromosome->fitness = sumOfCorrectChars;
}

void GeneticAlgorithm::sortPopulation(){
    std::sort(chromosomes.begin(), chromosomes.end(), [](Chromosome *a, Chromosome *b) {
        return a->fitness > b->fitness;
    });
}

std::vector<Chromosome *> GeneticAlgorithm::selectElitePopulation(){
    
    std::vector<Chromosome *> eliteChromosomes;
    
    for (unsigned i = 0; i < elitePopulationSize; ++i){
        Chromosome *chromosome = chromosomes[i];
        eliteChromosomes.push_back(chromosome);
    }
    
    return eliteChromosomes;
}

std::vector<Chromosome *> GeneticAlgorithm::selectSurvivingPopulation(){
    
    std::vector<Chromosome *> survivingChromosomes;
    
    while (1){
        
        bool didReachDesiredSize = false;
        
        for (unsigned i = elitePopulationSize + 1; i < populationSize; ++i){
            
            Chromosome *chromosome = chromosomes[i];
            
            // Probability to Select Chromosome
            if (selectWithProbability(chromosome->probability)){
                
                // Add Surviving Chromosome
                survivingChromosomes.push_back(chromosome);
                
                // Check if reached desired Surviving Population Size
                if (survivingChromosomes.size() >= survivingPopulationSize){
                    didReachDesiredSize = true;
                    break;
                }
            }
        }
        
        if (didReachDesiredSize){
            break;
        }
    }
    
    return survivingChromosomes;
}

unsigned GeneticAlgorithm::selectWithProbability(double probability){
    
    unsigned uniformIntDistributionSize = 1000;
    
    std::uniform_int_distribution<int> uniformIntDistribution(1, uniformIntDistributionSize);
    
    unsigned value = uniformIntDistribution(mt19937);
    
    unsigned threshold = probability * uniformIntDistributionSize;
    
    return threshold >= value;
}

std::vector<Chromosome *> GeneticAlgorithm::crossover(std::vector<Chromosome *> survivingChromosomes){
    
    std::vector<Chromosome *> offspringChromosomes;
    
    while (1) {
        
        Chromosome *parent1 = *selectRandomly(survivingChromosomes.begin(), survivingChromosomes.end());
        std::vector<char> genes1 = parent1->genes;
        std::vector<char> newGenes1;
        
        Chromosome *parent2 = *selectRandomly(survivingChromosomes.begin(), survivingChromosomes.end());
        std::vector<char> genes2 = parent2->genes;
        std::vector<char> newGenes2;
        
        for (unsigned i = 0; i < input.size(); ++i){
            
            char gene1 = genes1[i];
            char gene2 = genes2[i];
            
            if (randomNumber() <= crossoverProbability){
                newGenes1.push_back(gene1);
                newGenes2.push_back(gene2);
            }
            else{
                newGenes1.push_back(gene2);
                newGenes2.push_back(gene1);
            }
        }
        
        Chromosome *offspring1 = new Chromosome();
        offspring1->genes = newGenes1;
        
        if (offspringChromosomes.size() != offspringPopulationSize){
            offspringChromosomes.push_back(offspring1);
        }
        else{
            break;
        }
        
        Chromosome *offspring2 = new Chromosome();
        offspring2->genes = newGenes2;
        
        if (offspringChromosomes.size() != offspringPopulationSize){
            offspringChromosomes.push_back(offspring2);
        }
        else{
            break;
        }
    }
    
    return offspringChromosomes;
}

template<typename Iter> Iter GeneticAlgorithm::selectRandomly(Iter start, Iter end) {
    
    // This method is used to select randomly from array
    
    std::uniform_int_distribution<int> uniformIntDistribution(0, unsigned(std::distance(start, end)) - 1);
    
    std::advance(start, uniformIntDistribution(mt19937));
    
    return start;
}

void GeneticAlgorithm::mutation(std::vector<Chromosome *> offspringChromosomes){
    
    for (unsigned i = 0; i < offspringChromosomes.size(); ++i){
        
        Chromosome *chromosome = offspringChromosomes[i];
        
        for (unsigned j = 0; j < chromosome->genes.size(); ++j){
            
            if (selectWithProbability(mutationProbability)){
                chromosome->genes[j] = GeneticAlgorithm::randomGene();
            }
        }
    }
}

void GeneticAlgorithm::logGenerationResults(unsigned generationNumber, Chromosome *chromosome){
    
    std::cout << "Generation: " + std::to_string(generationNumber + 1) << std::endl;
    
    std::cout << "Best Fitness: " + std::to_string(unsigned(chromosome->fitness)) << std::endl;
    
    std::cout << "Max Fitness: " + std::to_string(unsigned(input.size())) << std::endl;
    
    std::cout << "Genes: ";
    
    for (unsigned i = 0; i < chromosome->genes.size(); ++i){
        
        char gene = chromosome->genes[i];
        std::cout << std::string(1,gene);
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}
