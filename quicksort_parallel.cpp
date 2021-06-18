#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;
int SIZE = 100000;

void init(vector<int> &arr);
void printVector(vector<int> &arr, int pid);

int main(int argc, char *argv[]){
    
    int pid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    vector<int> arr(SIZE);
    init(arr);
    
    int dimension = (int) log2(nproc);

    for(int d = dimension-1; d >= 0; d--){
        int pivot;
        
        if (pid == 0){
        	pivot = arr[arc4random()%arr.size()];
        }

        MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vector<int> low, high;
        
        for(auto i:arr){
            if (i <= pivot){
            	low.push_back(i);
            }else{
            	high.push_back(i);
            }
        }

        int high_size = high.size(), low_size = low.size();

        if ((pid&(1<<d)) == 0){
            MPI_Send(&high_size, 1, MPI_INT, pid+(1<<d), 0, MPI_COMM_WORLD);
            MPI_Send(&high[0], high.size(), MPI_INT, pid+(1<<d), 1, MPI_COMM_WORLD);
            MPI_Recv(&high_size, 1, MPI_INT, pid+(1<<d), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            high.resize(high_size);
            MPI_Recv(&high[0], high_size, MPI_INT, pid+(1<<d), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else{
            MPI_Send(&low_size, 1, MPI_INT, pid-(1<<d), 0, MPI_COMM_WORLD);
            MPI_Send(&low[0], low.size(), MPI_INT, pid-(1<<d), 1, MPI_COMM_WORLD);
            MPI_Recv(&low_size, 1, MPI_INT, pid-(1<<d), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            low.resize(low_size);
            MPI_Recv(&low[0], low_size, MPI_INT, pid-(1<<d), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        arr.clear();
        arr.insert(arr.end(), low.begin(), low.end());
        arr.insert(arr.end(), high.begin(), high.end());
        // printVector(arr, pid);
    }
    sort(arr.begin(), arr.end());
    cout << pid << " " << arr.size() << endl;
    MPI_Finalize();
}

void init(vector<int> &arr){
    for(int i = 0; i < arr.size(); i++){
    	arr[i] = arc4random()%((int)(1e8));
    }
}

void printVector(vector<int> &arr, int pid){
    cout << pid << ": ";
    for(int i = 0; i < arr.size(); i++){
    	cout << arr[i] << " ";
    }
    cout << endl; 
}