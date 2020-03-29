// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppEigen.h>
#include <Rcpp.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigen>

using namespace Rcpp;
using namespace Eigen;

#include "randompca.h"

// [[Rcpp::export]]
List flashpca_internal(
   const Eigen::Map<Eigen::MatrixXd> X,
   const int stand,
   const unsigned int ndim,
   const unsigned int divisor,
   const unsigned int maxiter,
   const double tol,
   const long seed,
   const bool verbose,
   const bool do_loadings,
   const bool return_scale)
{
   try{
      Eigen::MatrixXd Xm(X);

      RandomPCA rpca;
      rpca.stand_method_x = stand;
      rpca.divisor = divisor;
      rpca.verbose = verbose;

      rpca.pca_fast(Xm, 0, ndim, maxiter,
         tol, seed, do_loadings);
      
      NumericMatrix U(wrap(rpca.U));
      NumericMatrix P(wrap(rpca.Px));
      NumericVector d(wrap(rpca.d));

      NumericVector X_mean(0);
      NumericVector X_sd(0);
      NumericVector pve(wrap(rpca.pve));

      // STANDARDISE_NONE: 0
      if(return_scale && stand != 0)
      {
         NumericMatrix X_meansd(wrap(rpca.X_meansd));
         X_mean = X_meansd(_, 0);
         X_sd = X_meansd(_, 1);
      }
      
      Rcpp::List res;

      if(do_loadings)
      {
         NumericMatrix V(wrap(rpca.V));
         res = Rcpp::List::create(
            Rcpp::Named("values")=d,
            Rcpp::Named("vectors")=U,
            Rcpp::Named("projection")=P,
            Rcpp::Named("loadings")=V,
            Rcpp::Named("center")=X_mean,
            Rcpp::Named("scale")=X_sd,
	    Rcpp::Named("pve")=pve
         );
      }
      else
      {
         res = Rcpp::List::create(
            Rcpp::Named("values")=d,
            Rcpp::Named("vectors")=U,
            Rcpp::Named("projection")=P,
            Rcpp::Named("center")=X_mean,
            Rcpp::Named("scale")=X_sd,
	    Rcpp::Named("pve")=pve
         );
      }

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("flashpca_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List flashpca_plink_internal(
   const std::string fn,
   const int stand,
   const unsigned int ndim,
   const unsigned int divisor,
   const unsigned int maxiter,
   const unsigned int block_size,
   const double tol,
   const long seed,
   const bool verbose,
   const bool do_loadings,
   const bool return_scale)
{
   try{
      RandomPCA rpca;
      rpca.stand_method_x = stand;
      rpca.divisor = divisor;
      rpca.verbose = verbose;

      NumericVector X_mean(0);
      NumericVector X_sd(0);

      std::string fam_file, geno_file, bim_file, pheno_file;
      geno_file = fn + std::string(".bed");
      bim_file = fn + std::string(".bim");
      fam_file = fn + std::string(".fam");

      Data data;
      data.verbose = verbose;
      data.stand_method_x = stand; 
      data.read_pheno(fam_file.c_str(), 6);
      data.read_plink_bim(bim_file.c_str());
      data.geno_filename = geno_file.c_str();
      data.get_size();
      data.prepare();

      rpca.pca_fast(data, block_size, ndim,
         maxiter, tol, seed, do_loadings);
      
      NumericMatrix U(wrap(rpca.U));
      NumericMatrix P(wrap(rpca.Px));
      NumericVector d(wrap(rpca.d));
      NumericVector pve(wrap(rpca.pve));

      Rcpp::List res;

      // STANDARDISE_NONE: 0
      if(return_scale && stand != 0)
      {
         NumericMatrix X_meansd(wrap(rpca.X_meansd));
         X_mean = X_meansd(_, 0);
         X_sd = X_meansd(_, 1);
      }

      if(do_loadings)
      {
         NumericMatrix V(wrap(rpca.V));
         res = Rcpp::List::create(
            Rcpp::Named("values")=d,
            Rcpp::Named("vectors")=U,
            Rcpp::Named("projection")=P,
            Rcpp::Named("loadings")=V,
            Rcpp::Named("center")=X_mean,
            Rcpp::Named("scale")=X_sd,
	    Rcpp::Named("pve")=pve
         );
      }
      else
      {
         res = Rcpp::List::create(
            Rcpp::Named("values")=d,
            Rcpp::Named("vectors")=U,
            Rcpp::Named("projection")=P,
            Rcpp::Named("center")=X_mean,
            Rcpp::Named("scale")=X_sd,
	    Rcpp::Named("pve")=pve
         );
      }

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("flashpca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List scca_internal(
   const Eigen::Map<Eigen::MatrixXd> X,
   const Eigen::Map<Eigen::MatrixXd> Y,
   const double lambda1,
   const double lambda2,
   const unsigned int ndim,
   const int stand_x,
   const int stand_y,
   const int divisor,
   const int mem,
   const long seed,
   const int maxiter,
   const double tol,
   const bool verbose,
   const unsigned int num_threads,
   const bool useV,
   const Eigen::Map<Eigen::MatrixXd> Vinit)
{
   try{

#ifdef _OPENMP
      omp_set_num_threads(num_threads);
#endif

      Eigen::MatrixXd Xm = X;
      Eigen::MatrixXd Ym = Y;

      RandomPCA rpca;
      rpca.stand_method_x = stand_x;
      rpca.stand_method_y = stand_y;
      rpca.divisor = divisor;
      rpca.verbose = verbose;

      if(useV)
      {
         Eigen::MatrixXd Vm = Vinit;
         rpca.scca(Xm, Ym, lambda1, lambda2, seed,
            ndim, mem, maxiter, tol, Vm);
      }
      else
      {
         rpca.scca(Xm, Ym, lambda1, lambda2, seed,
            ndim, mem, maxiter, tol);
      }

      NumericMatrix U(wrap(rpca.U));
      NumericMatrix V(wrap(rpca.V));
      NumericMatrix Px(wrap(rpca.Px));
      NumericMatrix Py(wrap(rpca.Py));
      NumericVector d(wrap(rpca.d));

      Rcpp::List res;

      res = Rcpp::List::create(
            Rcpp::Named("U")=U,
            Rcpp::Named("V")=V,
            Rcpp::Named("d")=d,
            Rcpp::Named("Px")=Px,
            Rcpp::Named("Py")=Py
      );

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("scca_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List ucca_plink_internal(
   const std::string fn,
   const Eigen::Map<Eigen::MatrixXd> Y,
   const int stand_x,
   const int stand_y,
   const bool verbose)
{
   try{
      RandomPCA rpca;
      rpca.stand_method_x = stand_x;
      rpca.stand_method_y = stand_y;
      rpca.verbose = verbose;

      NumericVector X_mean(0);
      NumericVector X_sd(0);

      std::string fam_file, geno_file, bim_file, pheno_file;
      geno_file = fn + std::string(".bed");
      bim_file = fn + std::string(".bim");
      fam_file = fn + std::string(".fam");

      Data data;
      data.verbose = verbose;
      data.stand_method_x = stand_x; 
      data.Y = Y;
      data.N = Y.rows();
      data.read_plink_bim(bim_file.c_str());
      data.geno_filename = geno_file.c_str();
      data.get_size();
      data.prepare();

      rpca.ucca(data);
      
      NumericMatrix U(wrap(rpca.U));
      NumericMatrix P(wrap(rpca.Px));
      NumericVector d(wrap(rpca.d));

      NumericMatrix resM(wrap(rpca.res));
      colnames(resM) = StringVector::create("R", "Fstat", "P");
      StringVector rownamesV(data.snp_ids.size());
      for(int i = 0 ; i < data.snp_ids.size() ; i++)
         rownamesV(i) = data.snp_ids[i];
      rownames(resM) = rownamesV;

      Rcpp::List res = Rcpp::List::create(
         Rcpp::Named("result")=resM
      );

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("ucca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List scca_plink_internal(
   const std::string fn,
   const Eigen::Map<Eigen::MatrixXd> Y,
   const double lambda1,
   const double lambda2,
   const unsigned int ndim,
   const int stand_x,
   const int stand_y,
   const int divisor,
   const int mem,
   const long seed,
   const int maxiter,
   const double tol,
   const bool verbose,
   const unsigned int num_threads,
   const unsigned int block_size,
   const bool useV,
   const Eigen::Map<Eigen::MatrixXd> Vinit)
{
   try{

#ifdef _OPENMP
      omp_set_num_threads(num_threads);
#endif

      //Eigen::MatrixXd Ym = Y;

      RandomPCA rpca;
      rpca.stand_method_x = stand_x;
      rpca.stand_method_y = stand_y;
      rpca.divisor = divisor;
      rpca.verbose = verbose;

      std::string fam_file, geno_file, bim_file, pheno_file;
      geno_file = fn + std::string(".bed");
      bim_file = fn + std::string(".bim");
      fam_file = fn + std::string(".fam");

      Data data;
      data.verbose = verbose;
      data.stand_method_x = stand_x; 
      data.Y = Y;
      data.N = Y.rows();
      data.read_plink_bim(bim_file.c_str());
      data.geno_filename = geno_file.c_str();
      data.get_size();
      data.prepare();

      if(useV)
      {
         Eigen::MatrixXd Vm = Vinit;
         rpca.scca(data, lambda1, lambda2, seed,
            ndim, mem, maxiter, tol, block_size, Vm);
      }
      else
      {
         rpca.scca(data, lambda1, lambda2, seed,
            ndim, mem, maxiter, tol, block_size);
      }

      NumericMatrix U(wrap(rpca.U));
      NumericMatrix V(wrap(rpca.V));
      NumericMatrix Px(wrap(rpca.Px));
      NumericMatrix Py(wrap(rpca.Py));
      NumericVector d(wrap(rpca.d));

      Rcpp::List res;

      res = Rcpp::List::create(
            Rcpp::Named("U")=U,
            Rcpp::Named("V")=V,
            Rcpp::Named("d")=d,
            Rcpp::Named("Px")=Px,
            Rcpp::Named("Py")=Py
      );

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("scca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List ucca_internal(
   const Eigen::Map<Eigen::MatrixXd> X,
   const Eigen::Map<Eigen::MatrixXd> Y,
   const int stand_x,
   const int stand_y,
   const bool verbose)
{
   try{
      Eigen::MatrixXd Xm = X;
      Eigen::MatrixXd Ym = Y;

      RandomPCA rpca;
      rpca.stand_method_x = stand_x;
      rpca.stand_method_y = stand_y;
      rpca.verbose = verbose;

      NumericVector X_mean(0);
      NumericVector X_sd(0);

      rpca.ucca(Xm, Ym);
      
      NumericMatrix U(wrap(rpca.U));
      NumericMatrix P(wrap(rpca.Px));
      NumericVector d(wrap(rpca.d));

      NumericMatrix resM(wrap(rpca.res));
      colnames(resM) = StringVector::create("R", "Fstat", "P");

      // Rownames of 'result' will be assigned in the calling R code
      Rcpp::List res = Rcpp::List::create(
         Rcpp::Named("result")=resM
      );

      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("ucca_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
NumericMatrix standardise_impute(
   const Eigen::Map<Eigen::MatrixXd> XX,
   const int method)
{

   MatrixXd X = XX;
   standardise(X, method, false);
   NumericMatrix Xm(wrap(X));
   return Xm;
}

// [[Rcpp::export]]
List check_internal(
   const Eigen::MatrixXd& X,
   const int stand, 
   const Eigen::MatrixXd& evec,
   const Eigen::VectorXd& eval,
   const unsigned int divisor,
   const bool verbose)
{
   try{
      RandomPCA rpca;
      rpca.divisor = divisor;
      rpca.verbose = verbose;
      rpca.stand_method_x = stand;
      Eigen::MatrixXd XX(X);
      Eigen::MatrixXd evec_(evec);
      Eigen::VectorXd eval_(eval);
      rpca.check(XX, evec_, eval_); 
      Rcpp::List res = Rcpp::List::create(
         Rcpp::Named("err")=rpca.err,
	 Rcpp::Named("mse")=rpca.mse,
	 Rcpp::Named("rmse")=rpca.rmse);
      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("flashpca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

// [[Rcpp::export]]
List check_plink_internal(
   const std::string fn,
   const int stand,
   const Eigen::MatrixXd evec,
   const Eigen::VectorXd eval,
   const unsigned int block_size,
   const unsigned int divisor,
   const bool verbose)
{
   try{
      RandomPCA rpca;
      rpca.stand_method_x = stand;
      rpca.divisor = divisor;
      rpca.verbose = verbose;

      NumericVector X_mean(0);
      NumericVector X_sd(0);

      std::string fam_file, geno_file, bim_file, pheno_file;
      geno_file = fn + std::string(".bed");
      bim_file = fn + std::string(".bim");
      fam_file = fn + std::string(".fam");

      Data data;
      data.verbose = verbose;
      data.stand_method_x = stand; 
      data.read_pheno(fam_file.c_str(), 6);
      data.read_plink_bim(bim_file.c_str());
      data.geno_filename = geno_file.c_str();
      data.get_size();
      data.prepare();

      MatrixXd evec_(evec);
      VectorXd eval_(eval);
      rpca.check(data, block_size, evec_, eval_);
      
      Rcpp::List res = Rcpp::List::create(
         Rcpp::Named("err")=rpca.err,
	 Rcpp::Named("mse")=rpca.mse,
	 Rcpp::Named("rmse")=rpca.rmse);
      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("flashpca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

//List project_internal(
//   const Eigen::MatrixXd& X,
//   const Eigen::MatrixXd& loadings,
//   const std::string ref_allele,
//   const VectorXd& orig_mean,
//   const VectorXd& orig_sd,
//   const unsigned int divisor,
//   const bool verbose)
//{
//   try{
//      MatrixXd X2(X);
//      unsigned int n = X.rows(), p = X.cols();
//      VectorXd xj(n);
//      for(unsigned int j = 0 ; j < p ; j++)
//      {
//	 xj = X2.col(j);
//	 X2.col(j) = (xj.array() - orig_mean(j)) / orig_sd(j);
//	 for(unsigned int i = 0 ; i < n ; i++)
//	 {
//	    if(isnan(X2(i, j)))
//	       X2(i, j) = 0;
//	 }
//      }
//      MatrixXd Px = X2 * loadings / sqrt(divisor);
//      Rcpp::List res = Rcpp::List::create(
//         Rcpp::Named("projection")=Px);
//      return res;
//   }
//   catch(std::exception &ex)
//   {
//      forward_exception_to_r(ex);
//   }
//   catch(...)
//   {
//      ::Rf_error("flashpca_plink_internal: unknown c++ exception");
//   }
//   return NA_REAL;
//}

// [[Rcpp::export]]
List project_plink_internal(
   const std::string fn,
   const Eigen::MatrixXd& loadings,
   const std::vector<std::string> ref_alleles,
   const Eigen::VectorXd& orig_mean,
   const Eigen::VectorXd& orig_sd,
   const unsigned int block_size,
   const unsigned int divisor,
   const bool verbose)
{
   try{
      RandomPCA rpca;
      rpca.divisor = divisor;
      rpca.verbose = verbose;
      rpca.V = loadings;

      std::string fam_file, geno_file, bim_file, pheno_file;
      geno_file = fn + std::string(".bed");
      bim_file = fn + std::string(".bim");
      fam_file = fn + std::string(".fam");

      Data data;
      data.verbose = verbose;
      data.read_pheno(fam_file.c_str(), 6);
      data.read_plink_bim(bim_file.c_str());
      data.geno_filename = geno_file.c_str();
      data.get_size();
      data.prepare();

      data.use_preloaded_maf = true;
      data.X_meansd = MatrixXd::Zero(ref_alleles.size(), 2);
      data.X_meansd.col(0) = orig_mean;
      data.X_meansd.col(1) = orig_sd;

      rpca.project(data, block_size);
      
      Rcpp::List res = Rcpp::List::create(
         Rcpp::Named("projection")=rpca.Px);
      return res;
   }
   catch(std::exception &ex)
   {
      forward_exception_to_r(ex);
   }
   catch(...)
   {
      ::Rf_error("flashpca_plink_internal: unknown c++ exception");
   }
   return NA_REAL;
}

