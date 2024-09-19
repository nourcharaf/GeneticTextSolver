//
//  Chromosome.hpp
//  Genetic Algorithm
//
//  Created by Nour Charaf on 1/9/20.
//  Copyright © 2020 Apple. All rights reserved.
//

#ifndef Chromosome_hpp
#define Chromosome_hpp

#include "Parameters.hpp"

class Chromosome{
    
public:
    
    std::vector<char> genes;
    double fitness;
    double probability;
};

#endif /* Chromosome_hpp */
