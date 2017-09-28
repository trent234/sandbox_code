#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using std::vector;

int main() {
  //std::cout<<"input the number: " << std::endl;
  int n;
  std::cin >> n;
  //the first col [0] is the number of operations at that point
  //the second col [1] is the value at that point
  vector< vector<int> > vec(n + 1, vector<int>(2));

  vec[0][0] = 1;
  vec[0][1] = 1; //we start with 1 at the "0th" operation
  vec[1][1] = 1; //to get the b
  for(int i = 2; i <= n; i++)
  {
    //std::cout<< "LOOP " << i << std::endl;
    int best = INT_MAX;
    int choice = 0;
    //if its 1 then set operations to 0 and val to 1

    //if its greater than 1 start wit h the default choice to add 1 (worst case)
    if ( i >= 1 && vec[i - 1][0] < best){
        best = vec[i - 1][0];
        choice = 1;
        //std::cout<< "1 is an option..." << std::endl;
    }
    //if the number we are at can be *2 to get here (even) and the number we would come from has less moves
    //than the option above, and that prev move * 2 will be at or below the target.. update best choice
    if (i > 0 && i % 2 == 0 && vec[i / 2][0] < best){
        best = vec[i / 2][0];
        choice = 2;
        //std::cout<< "2 is an option..." << std::endl;
    }
    //if the number we are at can be *3 to get here and the number we would come from has less moves
    //than the best option so far, and that prev move *3 will be at or below the target.. update best choice
    if (i > 0 && i % 3 == 0 && vec[i / 3][0] < best){
        best = vec[i / 3][0];
        choice = 3;
        //std::cout<< "3 is an option..." << std::endl;
    }

    if (choice == 1) vec[i][0] = vec[i - 1][0] + 1; //increment operation counter by 1
    if (choice == 2) vec[i][0] = vec[i / 2][0] + 1; //increment operation counter by 1
    if (choice == 3) vec[i][0] = vec[i / 3][0] + 1; //increment operation counter by 1

    vec[i][1] = choice;

    //std::cout<< "vec[" << i << "][0]: " << vec[i][0] << std::endl;
    //std::cout<< "vec[" << i << "][1]: " << vec[i][1] << std::endl << std::endl;

    //if (vec[i][1] == n) break;
  }
int counter = 0;
vector< int > vec2(1);
  while (n != 1){
    vec2.push_back(n);

    if(vec[n][1] == 1) {
        n--;
    } else if(vec[n][1] == 2) {
        n /= 2;
    } else if(vec[n][1] == 3) {
        n /= 3;
    }
    counter++;
  }

  std::cout<< counter << std::endl;
  std::cout<< 1 << " ";
  for(n = counter; n >= 1; n--){
    std::cout << vec2[n] << " ";
  }
    std::cout << std::endl;
    return 0;
}


