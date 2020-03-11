#include <vector>

using namespace std;

enum mask_type {SOBEL_I = 0, SOBEL_J = 1};


vector<vector<vector<int>>> MASK{

    {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    },

    {
        {1,2,1},
        {0,0,0},
        {-1,-2,-1}
    }

};

