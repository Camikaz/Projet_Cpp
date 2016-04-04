
//=========================== Includes =================================
#include "Envir.h"

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
  Save("Bact");
  while(Tour < n){

    
    while(time_%T_ != T_-1){
      
      Diffuse();
            
      MultiLive();
      
      MultiDie();
    
      Competition();
      
      time_++;
      
      Save("Bact");
    } // end of a turn
    
    MultiClean();
    time_++; 
    Tour++;      
    Save("Bact");
  } //end while Tour < n
}


void Envir::Save(string name) const{
  
  ostringstream  oss;
  string Fname = "init";
  
  oss.str("");
  oss.clear();
  oss << name << time_ << ".ppm";
  Fname = oss.str();
  
  ofstream f;
	f.open(Fname, std::ios:: out | std::ios::trunc | std::ios::binary);
  f << "P6\n" << 4*W_ << " "<< H_ << "\n" << 255 << "\n";
  
  char a = 100;
  char b = 100;
  char c = 0;
  
  double ratio = 255/(Cellule::Ai_*2);
  
  for(int k = 0; k < W_ ; k++){
    c = 0;
    for(int j = 0; j < H_ ; j++){
      if(cells_[k][j]->state() == DEAD){
        a = 0;
        b = 0;
        c = 0;
      }
      
      else if(cells_[k][j]->state() == YOUNG_DEAD){
        a = 100;
        b = 100;
        c = 100;
      }
      
      else if(cells_[k][j]-> genome() == GA){
        a = 255;
        b = 70;
        c = 0;
      }
      
      else{
        a = 70;
        b = 255;
        c = 0;
      }
      
      if(cells_[k][j]->state() == NEW){
        a = a*2;
        b = b*2;
        c = 150;
      }
      
      f.write(&a , sizeof(char));
      f.write(&a , sizeof(char));
      f.write(&b , sizeof(char));
    }
    
    c = 0;
    // Show the map of Aout;
    for(int j = 0; j < H_ ; j++){
      a = ((char) (cells_[k][j]-> Aout() * ratio) )%255;
      f.write(&a , sizeof(char));
      f.write(&c , sizeof(char));
      f.write(&c , sizeof(char));
    }
    
    // Show the map of Bout;
    for(int j = 0; j < H_ ; j++){
      a = ((char) (cells_[k][j]-> Bout() * ratio) )%255;
      f.write(&c , sizeof(char));
      f.write(&a , sizeof(char));
      f.write(&c , sizeof(char));
    }
    
    // Show the map of Cout;
    for(int j = 0; j < H_ ; j++){
      a = ((char) (cells_[k][j]-> Cout() * ratio*1.5) )%255;
      f.write(&c , sizeof(char));
      f.write(&c , sizeof(char));
      f.write(&a , sizeof(char));
    }
    
  }
  
}

//~ void Envir::Save(string name) const{
  //~ 
  //~ ostringstream  oss;
  //~ string Fname = "init";
  //~ 
  //~ oss.str("");
  //~ oss.clear();
  //~ oss << name << time_ << ".ppm";
  //~ Fname = oss.str();
  //~ 
  //~ ofstream f;
	//~ f.open(Fname, std::ios:: out | std::ios::trunc | std::ios::binary);
  //~ f << "P6\n" << 2*W_ << " "<< H_ << "\n" << 255 << "\n";
  //~ 
  //~ char a = 100;
  //~ char b = 100;
  //~ char c = 100;
  //~ 
  //~ for(int k = 0; k < W_ ; k++){
    //~ for(int j = 0; j < H_ ; j++){
      //~ if(cells_[k][j]->state() == DEAD){
        //~ a = 0;
        //~ b = 0;
      //~ }
      //~ else if(cells_[k][j]-> genome() == GA){
        //~ a = 200;
        //~ b = 70;
      //~ }
      //~ else{
        //~ a = 70;
        //~ b = 200;
      //~ }
      //~ f.write(&a , sizeof(char));
      //~ f.write(&a , sizeof(char));
      //~ f.write(&b , sizeof(char));
    //~ }
    //~ 
    //~ // Show the map of Aout Bout and Cout;
    //~ for(int j = 0; j < H_ ; j++){
      //~ a = ((char) cells_[k][j]-> Aout()*10)%255;
      //~ b = ((char) cells_[k][j]-> Bout()*10)%255;
      //~ c = ((char) cells_[k][j]-> Cout()*10)%255;
      //~ f.write(&a , sizeof(char));
      //~ f.write(&b , sizeof(char));
      //~ f.write(&c , sizeof(char));
    //~ }
    //~ 
    //~ 
  //~ }
  //~ 
//~ }



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
      A_cp[k][j] = cells_[k][j]->Aout();
      B_cp[k][j] = cells_[k][j]->Bout();
      C_cp[k][j] = cells_[k][j]->Cout();
    }
  }
  
  double A_diff = 0;
  double B_diff = 0;
  double C_diff = 0;
  
  for(int k = 0 ; k < H_ ; k++){
    for(int j = 0 ; j< W_ ; j++){
      
      A_diff = A_cp[k][j];
      B_diff = B_cp[k][j];
      C_diff = C_cp[k][j];
      
      for(int kl = -1; kl <= 1 ; kl++){
        for(int jl = -1; jl <= 1; jl++){
          A_diff += D_* A_cp[(k+kl+H_)%H_][(j+jl+W_)%W_];
          B_diff += D_* B_cp[(k+kl+H_)%H_][(j+jl+W_)%W_];
          C_diff += D_* C_cp[(k+kl+H_)%H_][(j+jl+W_)%W_];
        }
      }
      
      A_diff = A_diff - 9*D_ * A_cp[k][j];
      B_diff = B_diff - 9*D_ * B_cp[k][j];
      C_diff = C_diff - 9*D_ * C_cp[k][j];

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


void Envir::MultiDie(){
  for(int k = 0; k < H_; k++){
    for(int j = 0; j < W_; j++){
      cells_[k][j] -> Die();
    }
  }
}

void Envir::Competition(){
  
  // the coordinate of the best living cell living next to the current dead cell
  double BestW = -1;
  
  //these vectors aren't randomized
  vector<int> dead_cells_X0;
  vector<int> dead_cells_Y0;
  
  //finding the dead cells positions
  for(int ki = 0 ; ki < H_ ; ki++){
    for(int ji = 0 ; ji < W_ ; ji++){
      if(cells_[ki][ji] -> state() == DEAD){
        dead_cells_X0.push_back(ki);
        dead_cells_Y0.push_back(ji);
      }
    }
  }
  
  //these vectors are randomized
  vector<int> dead_cells_X;
  vector<int> dead_cells_Y;
  
  int ret_size = dead_cells_X0.size() -1;
  
  if(ret_size < 0){
    return;
  }
  else if(ret_size == 0){
    dead_cells_X.push_back( dead_cells_X0[ret_size] );
    dead_cells_Y.push_back( dead_cells_Y0[ret_size] );
  }
  
  int alea;
  
  
  //randomization
  while(ret_size > 0){
    alea = rand()%ret_size;
    
    dead_cells_X.push_back( dead_cells_X0[alea] );
    dead_cells_Y.push_back( dead_cells_Y0[alea] );
    
    dead_cells_X0[alea] = dead_cells_X0[ret_size];
    dead_cells_Y0[alea] = dead_cells_Y0[ret_size];
    
    ret_size --;
  }// end randomization
  
  // These vectors will contain the positions of the cells with the best fitness
  vector<int> posX;
  vector<int> posY;
  
  // for each dead cell of position (k,j), we choose which cell will divide (if it's occurs)
  int k;
  int j;
  
  for(unsigned int i = 0; i< dead_cells_X.size(); i++){
    
    k = dead_cells_X[i];
    j = dead_cells_Y[i];

    
    BestW = -1;
    posX.clear();
    posY.clear();
    
    // finding the cell with the best fitness
    for(int kl = -1; kl <= 1 ; kl++){
      for(int jl = -1; jl <= 1; jl++){
        
        
        if( cells_[(k+kl + H_)%H_][(j+jl + W_)%W_]-> fitness() > BestW){
          posX.clear();
          posY.clear();
          posX.push_back((k+kl + H_)%H_) ;
          posY.push_back((j+jl + W_)%W_) ;
          
          BestW = cells_[(k+kl + H_)%H_][(j+jl + W_)%W_]-> fitness();
        }
        
        if( cells_[(k+kl + H_)%H_][(j+jl + W_)%W_]-> fitness() == BestW){
          posX.push_back((k+kl + H_)%H_) ;
          posY.push_back((j+jl + W_)%W_) ;
        }
        
        
          
      } //end k
    } // end j
    
    
    // division of the selected cell
    if(BestW > 0){
      int choice = rand()%posX.size();
      // the chosen bactery;
      int k_selected = posX[choice];
      int j_selected = posY[choice];
      
      cells_[k][j]-> BirthFrom( cells_[k_selected][j_selected] );
    }
  }
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
