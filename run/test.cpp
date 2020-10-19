#include<iostream>
using namespace std;




int get_2nd(vector<int> &nums){
    int max1=0,max2=0;
    for(int i=0;i<nums.size();i++){
        if(nums[i]>max1){
            max2=max1;
            max1=nums[i];
        }else{
            if(nums[i]>max2){
                max2=nums[i];
            }
        }
    }
    return max2;


}
