
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
  state_ = ADULT;

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


//=========================== Destructor ===============================
Cellule::~Cellule() {

}

//=========================== Public Methods ===========================

//you can call it for adult, new or dead cell
void Cellule::Live(){
  if(state_ == NEW){
    state_ = ADULT;
    // then the cell will also metabolize
  }
  
  if(state_ == YOUNG_DEAD){
    state_ = DEAD;
  }
  // the cell metabolize
  if(state_ == ADULT){
    if(genome_ == GA){
      //resolution des equa diff sur 1, avec Euler explicite et un pas de 0.1
      for(int k = 0; k < 10; k++){
        B_ = B_ + 0.1*A_*Rab_;
        A_ = A_ + 0.1*(Aout_*Raa_ - A_*Rab_);
        Aout_ = Aout_ - 0.1*Aout_*Raa_;
      }//end Euler
    }//end GA
    
    //if genome_ == GB
    else{
      for(int k = 0; k < 10; k++){
        C_ = C_ + 0.1*B_*Rbc_;
        B_ = B_ + 0.1*(Bout_*Rbb_ - B_*Rbc_);
        Bout_ = Bout_ -0.1*Bout_*Rbb_;
        Bout_ = Bout_ -0.1*Bout_*Rbb_;
      }//end Euler
    }//end GB
  }//end ADULT
}

//Create a cell from the mother cell "C" AND alterate "C"
void Cellule::BirthFrom(Cellule* Cm){
  state_ = NEW;
  Cm->state_ = NEW;
  
  A_ = Cm-> A_ * 0.5;
  B_ = Cm-> B_ * 0.5;
  C_ = Cm-> C_ * 0.5;
  
  Cm-> A_ = A_;
  Cm-> B_ = B_;
  Cm-> C_ = C_;

  // mutation for the mother cell
  cell_genome genome_m = Cm->genome_;
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pmut_){
    if(genome_m == GA){
      Cm->genome_ = GB;
      }
    else{
      Cm->genome_ = GA;
      }
  }
  
  //
  alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pmut_){
    if(genome_m == GA){
      genome_ = GB;
    }
    else{
      genome_ = GA;
    }
  }
  else{
    genome_ = genome_m;
  }
}

short Cellule::Die(){
  if(state_ == DEAD || state_ == YOUNG_DEAD){
    return 1;
  }
  
  double alea = rand()/(1.0*RAND_MAX);
  
  if(alea < Pdeath_){
    state_ = YOUNG_DEAD;
    
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
  if(state_ == DEAD || state_ == NEW || state_ == YOUNG_DEAD){
    return 0;
  }
  
  if(genome() == GA){
    if(B_ >= Wmin_){
      return B_;
    }
  }
  
  else if(C_ >= Wmin_){
    return C_;
  }
  
  return 0;
}

void Cellule::Clean(){
  Aout_ = Ai_;
  Bout_ = 0;
  Cout_ = 0;
}
//=========================== Protected Methods ========================

//=========================== Functions ================================
