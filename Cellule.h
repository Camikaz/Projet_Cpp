
#ifndef Cellule_H__
#define Cellule_H__
#include <cstdio>
#include <cstdlib>
#include <iostream>

using std::cout;
//this class represent a cell, AND the case that is under it

enum cell_state{DEAD, ADULT, NEW, YOUNG_DEAD};
enum cell_genome{GA,GB};

class Cellule {
 public :
 
  // =========================== Static attributes =====================
  static double Pmut_;
  static double Pdeath_;
  static double Wmin_;
  
  static double Raa_;
  static double Rab_;
  static double Rbb_;
  static double Rbc_;
  
  static double Ai_;
  
  // =========================== Static methods =====================
  static void Parameters(double Pmut, double Pdeath, double Wmin, double Raa, double Rab, double Rbb, double Rbc, double Ai);
  static void setAi(double Ai);
  
  // =========================== Constructors ==========================
  Cellule();
  
  //=========================== Destructor =============================
  ~Cellule();
  
  // =========================== Getters ===============================
  inline cell_state state() const;
  inline cell_genome genome() const;
  
  inline double A() const;
  inline double B() const;
  inline double C() const;
  
  inline double Aout() const;
  inline double Bout() const;
  inline double Cout() const;
  
  double fitness() const;
  
  //=========================== Setters ================================
  inline void SetCase(double Aout, double Bout, double Cout);
  
  //=========================== Operators ==============================

  //=========================== Public Methods =========================

  void Live();
  void BirthFrom(Cellule* C);
  short Die();
  void Clean();

protected :
  //=========================== Protected Methods ======================
  
  //=========================== Attributes =============================
  cell_state state_;
  cell_genome genome_;
  
  double A_;
  double B_;
  double C_;
  
  double Aout_;
  double Bout_;
  double Cout_;
  
};




//===========================Getters' definitions=======================

inline cell_state Cellule::state() const{
  return state_;
}

inline cell_genome Cellule::genome() const{
  return genome_;
}

inline double Cellule::A() const{
  return A_;
}

inline double Cellule::B() const{
  return B_;
}

inline double Cellule::C() const{
  return C_;
}

inline double Cellule::Aout() const{
  return Aout_;
}

inline double Cellule::Bout() const{
  return Bout_;
}

inline double Cellule::Cout() const{
  return Cout_;
}


//===========================Setters' definitions=======================

inline void Cellule::SetCase(double Aout, double Bout, double Cout){
  Aout_ = Aout;
  Bout_ = Bout;
  Cout_ = Cout;
}

//===========================Operators' definitions=====================

//========================Inline functions' definition==================



#endif // Cellule_H__
