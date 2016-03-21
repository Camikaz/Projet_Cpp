
#ifndef Envir_H__
#define Envir_H__


#include "Cellule.h"

class Envir {
 public :
  // =========================== Constructors ==========================
  Envir();
  //=========================== Destructor =============================
  ~Envir();
  // =========================== Getters ===============================

  //=========================== Setters ================================

  //=========================== Operators ==============================

  //=========================== Public Methods =========================
  inline int H() const;
  inline int W() const;

protected :
  //=========================== Protected Methods ======================
  
  //=========================== Attributes =============================
  Cellule* cells_;
  int W_;
  int H_;
  
};




//===========================Getters' definitions=======================
inline int Envir::W() const{
  return W_;
}

inline int Envir::H() const{
  return H_;
}
//===========================Setters' definitions=======================

//===========================Operators' definitions=====================

//========================Inline functions' definition==================



#endif // Envir_H__
