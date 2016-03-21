
//=========================== Includes =================================
#include "Envir.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

//================= Definition of static attributes ====================
int Envir::maxTime_ = 10000;
int Envir::W_ = 32;
int Envir::H_ = 32;
double Envir::D_ = 0.1;

//=========================== Constructors =============================
Envir::Envir() {
  T_ = 500;
  time_ = 0;
  
  cells_ = new Cellule**[H_];
  
  for(int k = 0; k < H_; k++){
    cells_[k] = new Cellule*[W_];
    for(int j = 0; j< W_; j++){
      cells_[k][j] = new Cellule();
    }
  }
  
}

// Rq: possibilité de modifier la repartition aleatoire des genomes pour avoir une egalité parfaite
Envir::Envir(int T, double Ai) {
  Cellule::setAi(Ai);
  
  T_ = T;
  time_ = 0;
    
  cells_ = new Cellule**[H_];
  
  for(int k = 0; k < H_; k++){
    cells_[k] = new Cellule*[W_];
    for(int j = 0; j< W_; j++){
      cells_[k][j] = new Cellule();
    }
  }
  
}

//=========================== Destructor ===============================
Envir::~Envir() {
  for(int k = H_ -1; k == 0; k--){
      for(int j = W_ -1; j == 0; j--){
        delete cells_[k][j];
      }
      delete[] cells_[k];
    }
  delete[] cells_;
}

//=========================== Public Methods ===========================

void Envir::Run(int n){
  int Tour = 0;
  
  while(Tour < n){
    while(time_%T_ != 0){
      Diffuse();
      Competition(MultiDie());
      MultiLive();
      time_++;
    }
    MultiClean();
    Tour++;
  }
  
}
//=========================== Protected Methods ========================
void Envir::Diffuse(){
  
  // keep in memory the previous state (A,B,C) for each cells
  double** A_cp = new double*[H_]; 
  double** B_cp = new double*[H_]; 
  double** C_cp = new double*[H_]; 
  
  for(int k = 0; k < H_; k++){
    A_cp[k] = new double[W_];
    B_cp[k] = new double[W_];
    C_cp[k] = new double[W_];
    
    for(int j = 0; j< W_; j++){
      A_cp[k][j] = cells_[k][j]->A();
      B_cp[k][j] = cells_[k][j]->B();
      C_cp[k][j] = cells_[k][j]->C();
    }
  }
  
  double A_diff = 0;
  double B_diff = 0;
  double C_diff = 0;
  
  for(int k = 0 ; k < H_ ; k++){
    for(int j = 0 ; j< W_ ; j++){
      
      A_diff = cells_[k][j]->A();
      B_diff = cells_[k][j]->B();
      C_diff = cells_[k][j]->C();
      
      for(int kl = -1; kl <= 1 ; kl++){
        for(int jl = -1; jl <= 1; jl++){
          A_diff += cells_[(k+kl)%H_][(j+jl)%W_]->A();
          B_diff += cells_[(k+kl)%H_][(j+jl)%W_]->B();
          C_diff += cells_[(k+kl)%H_][(j+jl)%W_]->C();
        }
      }
      cells_[k][j]->SetCase(A_diff, B_diff, C_diff);
    }
  }
  
  // delete the memory tabs
  for(int k = H_ -1; k == 0; k--){
      delete A_cp[k];
      delete B_cp[k];
      delete C_cp[k];
    }
  delete[] A_cp;
  delete[] B_cp;
  delete[] C_cp;
  
}

//return a table of pointers to the dead cells (randomly organised)
int** Envir::MultiDie(){
  
  //initialisation of the table of the position of the dead cells
  int** dead_pos = new int*[W_*H_]; 
  int** dead_pos_r = new int*[W_*H_]; 
  
  for( int k = 0; k < W_*H_ ; k++){
    
    dead_pos[k] = new int[2];
    dead_pos[k][0] = -1;
    dead_pos[k][1] = -1;
    
    dead_pos_r[k] = new int[2];
    dead_pos_r[k][0] = -1;
    dead_pos_r[k][1] = -1;

  }
  // end init
  
  int iter = 0;
  
  // kill the unlucky cells and write their pos in the table
  for(int k =0; k < H_; k++){
    for(int j = 0; j< W_; j++){
      if( cells_[k][j]->Die() ){
        dead_pos[iter][0] = k;
        dead_pos[iter][1] = j;
        iter ++;
      }
    }
  }
  
  iter --;
  int alea = 0;
  int iter2 = 0;
  
  // make the randomized table dead_pos_r
  while(iter != 0){
    
    alea = (int) iter * rand()/(1.0*RAND_MAX);
    
    dead_pos_r[iter2][0] = dead_pos[alea][0];
    dead_pos_r[iter2][1] = dead_pos[alea][1];
    
    iter2 ++;
    
    dead_pos[alea] = dead_pos[iter];      
    iter --;
    
  }
  
  // delete the unnecessary table dead_pos
  for(int k = 0; k < W_*H_ ; k++){
    delete[] dead_pos[k];
  }
  delete[] dead_pos;
  
  return dead_pos_r;
}

void Envir::Competition(int** dead_pos){
  int iter = 0;
  
  // the coordinates of a current dead_cell
  int k = 0;
  int j = 0;
  
  // the coordinate of the best living cell living next to the current dead cell
  int k_selected = -1;
  int j_selected = -1;
  double BestW = -1;
  
  // for each dead cell
  while (dead_pos[iter][0] != -1){
    k = dead_pos[iter][0];
    j = dead_pos[iter][1];
    
    // finding the cell with the best fitness
    for(int kl = -1; kl <= 1 ; kl++){
      for(int jl = -1; jl <= 1; jl++){
        if( cells_[(k+kl)%H_][(j+jl)%W_]-> fitness() > BestW){
          k_selected = (k+kl)%H_;
          j_selected = (j+jl)%H_;
        }
      }
    }
    
    // division of the selected cell
    cells_[k][j]-> BirthFrom( cells_[k_selected][j_selected] );
    
    iter++;
  }
  
  //deletion of the table dead_pos
  for(int k = 0; k < W_*H_ ; k++){
    delete[] dead_pos[k];
  }
  delete[] dead_pos;
}


void Envir::MultiLive(){
  for(int k = 0; k< H_; k++){
    for(int j =0; j< W_; j++){
      cells_[k][j]->Live();
    }
  }
}

void Envir::MultiClean(){
  for(int k = 0; k< H_; k++){
    for(int j =0; j< W_; j++){
      cells_[k][j]->Clean();
    }
  }
}


//=========================== Functions ================================
