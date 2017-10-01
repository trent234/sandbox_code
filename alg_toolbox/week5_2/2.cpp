#include <iostream>
#include <vector>
/*
 * Its a beast of an if statement...
 * if the bar is less than the current weight limit AND
 * the current bar + the value of 1 less bar with the current bar weight removed is greater than just the current bar AND
 * option a is less than the knapsack weight, use both bars.
 * if not, skip the current bar and select the val for same weight w/o this bar (1 above)
 */

using std::vector;

int optimal_weight(int totalWeight, const vector<int> &barList) {

    //grid[bar amount = Y][weight amount = X]
    vector< vector<int> > grid(barList.size(), vector<int>(totalWeight + 1));
    int sumWeight = 0;
    for(int y = 1; y < barList.size(); y++){

        //std::cout<<"y: " << y << "\n";
        for (int x = 1; x <= totalWeight; x++){
            if(barList[y] <= x && barList[y] + grid[y - 1][x - barList[y]] > grid[y - 1][x] && barList[y] + grid[y - 1][x - barList[y]] <= totalWeight){
                    grid[y][x] = barList[y] + grid[y - 1][x - barList[y]];
                    //std::cout<< "x: " << x << " " << grid[y][x] << "\n";
                } else {
                    grid[y][x] = grid[y - 1][x];
                    //std::cout<< "x: " << x << " " << grid[y][x] << "\n";
                }
            //size - 1 bc it includes the 0th element. we want the count of all gold bars
            if(y == barList.size() - 1 && x == totalWeight ) sumWeight = grid[y][x]; //snag that end corner number
          }
    }
  return sumWeight;
}

int main() {
  int totalWeight, barQuant;
  std::cin >> totalWeight >> barQuant;
  vector<int> barList(barQuant + 1);
  for (int i = 1; i <= barQuant; i++) {
    std::cin >> barList[i];
  }
  std::cout << optimal_weight(totalWeight, barList) << '\n';
}
