
#ifndef Envir_H__
#define Envir_H__
#include <cstdio>
#include <cstdlib>

#include "Cellule.h"

class Envir {
 public :
  // =========================== Static attributes =====================
  
  static double maxTime_;
  static int W_;
  static int H_;
  static double D_;
  // =========================== Constructors ==========================
  Envir();
  Envir(double T, double Ai);
  
  //=========================== Destructor =============================
  ~Envir();
  // =========================== Getters ===============================

  //=========================== Setters ================================

  //=========================== Operators ==============================

  //=========================== Public Methods =========================

protected :
  //=========================== Protected Methods ======================
  
  void Diffuse();
  int** MultiDie();
  void Competition(int** &dead_pos);
  void MultiLive();
  
  //=========================== Attributes =============================
  Cellule*** cells_; // 2D table containing pointers
  double time_;
  double T_;
  
};




//===========================Getters' definitions=======================

//===========================Setters' definitions=======================

//===========================Operators' definitions=====================

//========================Inline functions' definition==================



#endif // Envir_H__
