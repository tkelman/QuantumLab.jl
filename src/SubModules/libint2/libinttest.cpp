#include <libint2.hpp>
//struct substruct{
//  double subval;
//};
//struct teststruct{
//  private:
//  double sin(double);
//  struct hiddentype {
//    double unnecessary;
//  };
//  double privat;
//  substruct sub2;
//  public:
//  substruct sub;
//  double val;
//};
//
//void printteststruct(teststruct stru){
//  printf("%f\n",stru.val);
//  printf("Sub: %f\n",stru.sub.subval);
//}

libint2::Shell* createShell(double* origin, int lqn, int nprim, double* exponents, double* coefficients){
  std::vector<double> alpha = std::vector<double>(exponents,exponents+nprim);
  std::vector<double> coeff = std::vector<double>(coefficients,coefficients+nprim);
  libint2::Shell* sh = new libint2::Shell(
      alpha,
      {  // contraction 0: s shell (l=0), spherical=false, contraction coefficients
      //{0, false, {0.27693435969118385, 0.26783885053249434, 0.08347366923669435}}
      {lqn, false, coeff}
      },
      {{origin[0], origin[1], origin[2]}}   // origin coordinates
      );
  return sh;
}

char lqnToLetter(int number){
  switch (number) {
    case 0: return 'S';
    case 1: return 'P'; 
    case 2: return 'D';
    case 3: return 'F';
    case 4: return 'G';
    case 5: return 'H';
    default: throw("QuantumNumber not yet implemented");
  }
}

void printShell(libint2::Shell* sh){
  printf("%c-Shell (LibInt2)   @   (%f, %f, %f)\n",lqnToLetter(sh->contr[0].l),sh->O[0],sh->O[1],sh->O[2]);

  printf("  Exponents:    ");
  for (double exp : sh->alpha) {
    printf("  %f",exp);
  }
  printf("\n");

  printf("  Coefficients: ");
  for (double exp : sh->contr[0].coeff) {
    printf("  %f",exp);
  }
  printf("\n");
}

libint2::Engine* createEngineCoulomb(int nprims, int maxlqn){
  libint2::Engine* engine= new libint2::Engine(libint2::Operator::coulomb, // will compute coulomb ints
                                               nprims,                     // max # of primitives in shells this engine will accept
                                               maxlqn,                     // max angular momentum of shells this engine will accept
                                               0
                                              );
  return engine;
}

libint2::OneBodyEngine* createEngineOverlap(int nprims, int maxlqn){
  libint2::OneBodyEngine* onebody_engine = new libint2::OneBodyEngine(libint2::OneBodyEngine::overlap,	 // will compute overlap ints
                                                                      nprims, 		 		 // max # of primitives in shells this engine will accept
                                                                      maxlqn			         // max angular momentum of shells this engine will accept
                                                                     );
  return onebody_engine;
}

void destroyEngine(libint2::Engine* engine){
  delete engine;
}
void destroyOneBodyEngine(libint2::OneBodyEngine* obengine){
  delete obengine;
}
void destroyShell(libint2::Shell* sh){
  delete sh;
}

const double* computeERI(libint2::Engine* engine, libint2::Shell* mu, libint2::Shell* nu, /* 1/r */ libint2::Shell* lambda, libint2::Shell* sigma){
  const double* eris = engine->compute(*mu,*nu,*lambda,*sigma);
  return eris;
}

const double* computeOverlap(libint2::OneBodyEngine* engine, libint2::Shell* shell1, libint2::Shell* shell2){
  const double* ints = engine->compute(*shell1,*shell2);
  return ints;
}

int main(){
  libint2::initialize();

  ////////
  printf("Create Shells.\n");
  std::vector<libint2::Shell> shells;
  shells.push_back({
	      {3.425250910, 0.623913730, 0.168855400}, // exponents of primitive Gaussians
              {  // contraction 0: s shell (l=0), spherical=false, contraction coefficients
                {0, false, {0.27693435969118385, 0.26783885053249434, 0.08347366923669435}}
              },
              {{0., 0., 0.}}   // origin coordinates
      });
  shells.push_back({
	      {3.425250910, 0.623913730, 0.168855400}, // exponents of primitive Gaussians
              {  // contraction 0: s shell (l=0), spherical=false, contraction coefficients
                {0, false, {0.27693435969118385, 0.26783885053249434, 0.08347366923669435}}
              },
              {{0., 0., 1.}}   // origin coordinates
      });
  ////////

  ////////
  printf("Create Engine.\n");
  libint2::OneBodyEngine onebody_engine(libint2::OneBodyEngine::overlap, // will compute overlap ints
                                        3, // max # of primitives in shells this engine will accept
                                        1      // max angular momentum of shells this engine will accept
                                       );
  ////////
  
  ////////
  printf("Compute Overlap.\n");
  for (libint2::Shell sh1 : shells) {
    for (libint2::Shell sh2 : shells) {
      const double* ints = onebody_engine.compute(sh1,sh2);
      printf("  %f\n",ints[0]);
    }
  }
  ////////

  libint2::finalize();
  printf("All done.\n");
  return 0;
}
