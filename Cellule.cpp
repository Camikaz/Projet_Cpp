
//=========================== Includes =================================
#include "Cellule.h"


using namespace std;

//================= Definition of static attributes ====================

double Cellule::Pmut_ = 0;
double Cellule::Pdeath_ = 0.02;
double Cellule::Wmin_ = 0.001;

double Cellule::Raa_ = 0.1;
double Cellule::Rab_ = 0.1;
double Cellule::Rbb_ = 0.1;
double Cellule::Rbc_ = 0.1;

double Cellule::Ai_ = 0.1;

//================= Definition of static methods =======================

void Cellule::Parameters(double Pmut, double Pdeath, double Wmin, double Raa, double Rab, double Rbb, double Rbc, double Ai){
  Pmut_ = Pmut;
  Pdeath_ = Pdeath;
  Wmin_ = Wmin;
  Raa_ = Raa;
  Rab_ = Rab;
  Rbb_ = Rbb;
  Rbc_ = Rbc;
  Ai_ = Ai;
}


void Cellule::setAi(double Ai){
  Ai_ = Ai;
}

//=========================== Constructors =============================
Cellule::Cellule() {
  state_ = DEAD;

  Aout_ = Ai_;
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


Cellule::Cellule(cell_genome G){
  state_ = NEW;
  genome_ = G;
  
  A_ = 0;
  B_ = 0;
  C_ = 0;
  
  
  Aout_ = Ai_;
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
      //resolution des equa diff sur 1, avec Euler explicite et un pas de 0.1
      for(int k = 0; k < 10; k++){
        B_ = B_ + 0.1*A_*Rab_;
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

//Create a cell from the mother cell "C" AND alterate "C"
void Cellule::BirthFrom(Cellule* C){
  
  //keeping in memory the mother cell information
  double A_m = C->A_;
  double B_m = C->B_;
  double C_m = C->C_;
  cell_genome genome_m = C->genome_;
  
  //alteration of the mother cell (it becomes à baby cell)
  
  C->A_ = A_m*0.5;
  C->B_ = B_m*0.5;
  C->C_ = C_m*0.5;
  
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pmut_){
    if(genome_m == GA){
      C->genome_ = GB;
      }
    else{
      C->genome_ = GA;
      }
  }
  
  
  //creation of the new cell
  
  //new genome
  alea = rand()/(1.0*RAND_MAX);
  
  if(alea >= Pmut_){
    genome_ = genome_m;
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

short Cellule::Die(){
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pdeath_){
    state_ = DEAD;
    
    Aout_ += A_;
    Bout_ += B_;
    Cout_ += C_;
    
    A_ = 0;
    B_ = 0;
    C_ = 0;
    return 1;
  }
  return 0;
}

double Cellule::fitness() const{
  if(genome_ == GA){
    if(B_ >= Wmin_){
      return B_;
    }
    return 0;
  }
  
  else{
    if(C_ >= Wmin_){
      return C_;
    }
    return 0;
  }
}

void Cellule::Clean(){
  Aout_ = Ai_;
  Bout_ = 0;
  Cout_ = 0;
}
//=========================== Protected Methods ========================

//=========================== Functions ================================
