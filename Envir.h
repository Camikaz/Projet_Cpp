
#ifndef Envir_H__
#define Envir_H__

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

#include <sstream>



using std::stringstream;
using std::string;
using std::to_string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::ostringstream;


#include "Cellule.h"

class Envir {
 public :
  // =========================== Static attributes =====================
  
  static int maxTime_;
  static int W_;
  static int H_;
  static double D_;
  // =========================== Constructors ==========================
  Envir();
  Envir(int T, double Ai);
  
  //=========================== Destructor =============================
  ~Envir();
  // =========================== Getters ===============================

  //=========================== Setters ================================

  //=========================== Operators ==============================

  //=========================== Public Methods =========================
  void Run(int n);
  void Save(string name) const;
  
protected :
  //=========================== Protected Methods ======================
  
  void Diffuse();
  int** MultiDie();
  void Competition(int** dead_pos);
  void MultiLive();
  void MultiClean();
  
  //=========================== Attributes =============================
  Cellule*** cells_; // 2D table containing pointers
  int time_;
  int T_;
  
};




//===========================Getters' definitions=======================

//===========================Setters' definitions=======================

//===========================Operators' definitions=====================

//========================Inline functions' definition==================



#endif // Envir_H__
