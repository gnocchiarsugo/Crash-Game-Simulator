#pragma once
#include <slot/header.h>
#include <vector>
#include <array>

const std::array<std::array<win_t, 4>, 12> paytable = {{
    {0, 0, 0, 0},
    {0, 0, 100, 2000},
    {0, 0, 50, 500},  
    {0, 0, 20, 200},  
    {0, 0, 15, 100},  
    {0, 0, 10, 50},   
    {0, 0, 7, 25},    
    {0, 0, 5, 15},    
    {0, 0, 3, 10},    
    {0, 0, 2, 5},     
    {0, 0, 0, 2000},  
    {0, 0, 1, 3},     

}};

const std::array<std::array<unsigned, 3>, 5> paylines = {{
    {0, 0, 0},
    {1, 1, 1},
    {2, 2, 2},
    {0, 1, 2},
    {2, 1, 0},
}};

// Dictionary used to map random indices to symbols
const std::array<sym_t, 11> index_dict = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const std::array<ChaCha::result_type, 11> spin_w = {18, 30, 48, 58, 73, 90, 109, 143, 176, 10, 245};