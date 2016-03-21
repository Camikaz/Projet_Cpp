
//=========================== Includes =================================
#include "Cellule.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

//================= Definition of static attributes ====================

double Cellule::Pmut_ = 0;
double Cellule::Pdeath_ = 0.02;
double Cellule::Wmin_ = 0.001;

double Cellule::Raa_ = 0.1;
double Cellule::Rab_ = 0.1;
double Cellule::Rbb_ = 0.1;
double Cellule::Rbc_ = 0.1;

//================= Definition of static methods =======================

void Cellule::Parameters(double Pmut, double Pdeath, double Wmin, double Raa, double Rab, double Rbb, double Rbc){
  Pmut_ = Pmut;
  Pdeath_ = Pdeath;
  Wmin_ = Wmin;
  Raa_ = Raa;
  Rab_ = Rab;
  Rbb_ = Rbb;
  Rbc_ = Rbc;
}

//=========================== Constructors =============================
Cellule::Cellule() {
  state_ = DEAD;

  Aout_ = 0.1;
  Bout_ = 0;
  Cout_ = 0;  
  
  // create a standard cell randomly GA or GB
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea > 0.5){
    genome_ = GA;
  }
  else{
    genome_ = GB;
  }
  
  A_ = 0;
  B_ = 0;
  C_ = 0;
}


Cellule::Cellule(cell_genome G, double Ai){
  state_ = NEW;
  
  genome_ = G;
  A_ = 0;
  B_ = 0;
  C_ = 0;
  
  
  Aout_ = Ai;
  Bout_ = 0;
  Cout_ = 0;
}

//=========================== Destructor ===============================
Cellule::~Cellule() {

}

//=========================== Public Methods ===========================

//you can call it for adult, new or dead cell
void Cellule::Live(){
  if(state_ == ADULT){
    if(genome_ == GA){
      //resolution des equa diff sur 1, avec Euler explicite avec un pas de 0.1
      for(int k = 0; k < 10; k++){
        B_ = B_ + 0.1*A_*Rbc_;
        A_ = A_ + 0.1*(Aout_*Raa_ - A_*Rab_);
        Aout_ = Aout_ - 0.1*Aout_*Raa_;
      }//end Euler
    }//end GA
    
    if(genome_ == GB){
      for(int k = 0; k < 10; k++){
        C_ = C_ + 0.1*B_*Rbc_;
        B_ = B_ + 0.1*(Bout_*Rbb_ - B_*Rbc_);
        Bout_ = Bout_ -0.1*Bout_*Rbb_;
      }//end Euler
    }//end GB
  }//end ADULT
  else if(state_ == NEW){
    state_ = ADULT;
  }
}

//Create a cell from the mother cell C AND alterate the mother cell C
void Cellule::BirthFrom(Cellule& C){
  
  //keeping the mother cell information
  double A_m = C.A_;
  double B_m = C.B_;
  double C_m = C.C_;
  cell_genome genome_m = C.genome_;
  
  //alteration of the existing cell (it becomes à baby cell)
  C.state_ = NEW;

  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pmut_){
    if(genome_m == GA){
      C.genome_ = GB;
      }
    else{
      C.genome_ = GA;
      }
  }
  
  C.A_ = A_m*0.5;
  C.B_ = B_m*0.5;
  C.C_ = C_m*0.5;
  
  
  //creation of the new cell
  state_ = NEW;
  
  //new genome
  alea = rand()/(1.0*RAND_MAX);
  
  if(alea >= Pmut_){
    genome_ = C.genome();
  }
  else{
    if(genome_m == GA){
      genome_ = GB;
    }
    else{
      genome_ = GA;
      }
  }
  
  //new composition
  A_ = A_m*0.5;
  B_ = B_m*0.5;
  C_ = C_m*0.5;
}

void Cellule::Die(){
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pdeath_){
    state_ = DEAD;
    
    Aout_ += A_;
    Bout_ += B_;
    Cout_ += C_;
    
    A_ = 0;
    B_ = 0;
    C_ = 0;
  }
}

void Cellule::Clean(double Ai){
  Aout_ = Ai;
  Bout_ = 0;
  Cout_ = 0;
}
//=========================== Protected Methods ========================

//=========================== Functions ================================
