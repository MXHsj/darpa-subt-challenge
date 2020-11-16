#include <iostream>
#include <vector>
#include <math.h>

int select_sector(std::vector<int> hist_bin)
{
    int len_threshold = 5;
    int curr_len = 0;
    bool isValley = false;
    std::vector<int> candidates;
    int mid = round(hist_bin.size() / 2);
    int valley_idx = mid;
    // store all admissible valleys
    for (int i = 0; i < hist_bin.size(); i++)
    {
        if (hist_bin[i] == 0 && !isValley)
        {
            isValley = true;
        }
        if ((hist_bin[i] == 1 && isValley) || (i == hist_bin.size() - 1))
        {
            isValley = false;
            if (curr_len > len_threshold)
            {
                candidates.push_back(i - round(curr_len / 2));
            }
            curr_len = 0;
        }
        if (isValley)
        {
            curr_len++;
        }
    }
    // find the valley needs minimum turning
    int dist = 100;
    std::cout << "candidates: ";
    for (int i = 0; i < candidates.size(); i++)
    {
        if (abs(candidates[i] - mid) < dist)
        {
            valley_idx = candidates[i];
            dist = abs(candidates[i] - mid);
        }
        std::cout << candidates[i] << " ";
    }

    std::cout << "\nvalley: " << valley_idx << std::endl;

    return valley_idx;
}

int main()
{
    std::vector<int> hist_bin = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int valley_idx = select_sector(hist_bin);
}