#include "TTree.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TAxis.h"
#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TMatrixD.h"
#include "TFitResult.h"

extern TSystem* gSystem;
extern TStyle* gStyle;

struct GlobalChi2 {
  GlobalChi2(  ROOT::Math::IMultiGenFunction* f1,
               ROOT::Math::IMultiGenFunction* f2) :
    fChi2_1(f1), fChi2_2(f2) {}

  // parameter vector is first background (in common 1 and 2)
  // and then is signal (only in 2)
  double operator() (const double *par) const {
    if (fChi2_1 && fChi2_1) {
      return (*fChi2_1)(par) + (*fChi2_2)(par);
    }
    else if (fChi2_1 && !fChi2_2) {
      return (*fChi2_1)(par);
    }
    else if (!fChi2_1 && fChi2_2) {
      return (*fChi2_2)(par);
    }
    else {
      std::cout << "Neither ROOT::Math::Chi2 functions are defined.." << std::endl;
      return -1;
    }
  }

  const  ROOT::Math::IMultiGenFunction * fChi2_1;
  const  ROOT::Math::IMultiGenFunction * fChi2_2;
};

void TWISTAndAlCapFits(bool save_plots = false) {
  std::string basedir = "/mu2e/app/users/edmonds/plots/2022-11-08/";

  gStyle->SetOptFit(1111);

  const int n_particles = 4;

  std::string particles[n_particles] = {"protons", "deuterons", "tritons", "alphas"};

  const int n_materials = 3;
  std::string materials[n_materials] = {"Al", "Si", "Ti"};
  //  std::string materials[n_materials] = {"Si"};

  const int n_datasets = 2;
  std::string datasets[n_datasets] = {"TWIST", "AlCap"};
  Int_t markers[n_datasets] = {kOpenSquare, kFullCircle};
  Int_t colours[n_datasets] = {kBlue, kRed};
  TGraph* gr_data[n_materials][n_particles][n_datasets];
  ROOT::Fit::BinData* bin_data[n_materials][n_particles][n_datasets];

  // Define the fit function
  const int n_fits = 2;
  double fit_min = 3.5;//[n_materials][n_particles] = { {3.5, 4.5, 5.0, 15.0}, // Al
  //                                                {4.0, 5.0, 6.0, 15.0}, // Si
  //                                                {3.5, 4.5, 5.0, 15.0} }; // Ti
  double fit_max = 45;
  std::string fit_names[n_fits] = {"Hungerford", "Murat"}; // "No-Thresh", "Murat"};
  std::string fit_exprs[n_fits] = {"[0]*(1 - [1]/x)^[2] * TMath::Exp(-x/[3])",
                                   //                                   "[0]*(x/(1+x))^[1] * TMath::Exp(-x/[2])",
                                   "[0]* ((x<[3])*( (x/(1 + [1]*x))^[2] * TMath::Exp(-x/[4])) + (x>[3])*(TMath::Exp(-x/[5])))"};
  Int_t fit_colours[n_fits] = {kMagenta, kBlack};
  const int max_n_pars = 6;
  int n_pars[n_fits] = {4, 6}; // 3, 6};
  double init_pars[n_fits][max_n_pars];
  std::string par_names[n_fits][max_n_pars];
  bool is_fixed_par[n_fits][max_n_pars];
  par_names[0][0] = "N"; par_names[0][1] = "T_{th}"; par_names[0][2] = "#alpha"; par_names[0][3] = "T_{0}";
  init_pars[0][0] = 0.105; init_pars[0][1] = 0.2; init_pars[0][2] = 2.0; init_pars[0][3] = 5.0;
  is_fixed_par[0][0] = false; is_fixed_par[0][1] = true; is_fixed_par[0][2] = true; is_fixed_par[0][3] = false;

  // par_names[1][0] = "N"; par_names[1][1] = "#alpha"; par_names[1][2] = "T_{0}";
  // init_pars[1][0] = 0.105; init_pars[1][1] = 2.0; init_pars[1][2] = 5.0;
  // is_fixed_par[1][0] = false; is_fixed_par[1][1] = true; is_fixed_par[1][2] = false;

  par_names[1][0] = "N"; par_names[1][1] = "p1"; par_names[1][2] = "p2"; par_names[1][3] = "p3"; par_names[1][4] = "p4"; par_names[1][5] = "p5";
  init_pars[1][0] = 0.105; init_pars[1][1] = 0.5; init_pars[1][2] = 2.0; init_pars[1][3] = 7.755; init_pars[1][4] = 2.5; init_pars[1][5] = 6;
  is_fixed_par[1][0] = false; is_fixed_par[1][1] = false; is_fixed_par[1][2] = false; is_fixed_par[1][3] = false; is_fixed_par[1][4] = false; is_fixed_par[1][5] = false;

  std::string latex_exprs[n_fits] = {par_names[0][0]+"#left(1 - #frac{" + par_names[0][1] + "}{E}#right)^{" + par_names[0][2] + "} e^{-E/" + par_names[0][3] + "}",
                                     "#splitline{" + par_names[1][0]+"#left( #frac{E}{(1 + "+par_names[1][1]+"E)}#right)^{"+par_names[1][2]+"} e^{-E/"+par_names[1][4]+"}   (E<"+par_names[1][3]+")}{                           "+par_names[1][0]+"e^{-E/"+par_names[1][5]+"} (E>"+par_names[1][3]+")}"};

  //  TF1* fit_fn = new TF1("fit_fn", "[0]*x^[1]*(1/(TMath::Exp([2]*x) - 1))", 3.5,45);
  //  const int Npar = 3;
  //  double init_par[Npar] = { 0.105, 1, 0.5 };//, 2.0, 5.0};
  //  TF1* fit_fn = new TF1("fit_fn", "[0]*(1/([1]*x^[2] - 1))", 3.5,45);
  //  const int Npar = 3;
  //  double init_par[Npar] = { 0.105, 1.25, 0.5 };//, 2.0, 5.0};
  //  TF1* fit_fn = new TF1("fit_fn", "(x/(1+[0]*x))^[1] * TMath::Exp(-x/[2])", 3.5,45);
  //  const int Npar = 3;
  //  double init_par[Npar] = { 300, 0.8, 5.0};
  //  TF1* fit_fn = new TF1("fit_fn", "[0]*TMath::Exp(-x/[1])", 3.5,45);
  //  const int Npar = 2;
  //  double init_par[Npar] = { 0.105, 5.0};

  //  ROOT::Fit::Chi2Function* chi2_fns[n_materials][n_particles][n_datasets];
  TF1* fit_fns[n_materials][n_particles][n_datasets][n_fits];
  ROOT::Fit::Chi2Function* chi2_fns[n_materials][n_particles][n_datasets][n_fits];
  ROOT::Fit::FitResult results[n_materials][n_particles][n_datasets][n_fits];

  for (int i_material = 0; i_material < n_materials; ++i_material) {
    std::string material = materials[i_material];
    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      std::string particle = particles[i_particle];
      std::cout << "START: " << particle << std::endl;
      for (int i_dataset = 0; i_dataset < n_datasets; ++i_dataset) {
        std::string dataset = datasets[i_dataset];
        std::string csvname = dataset + "_" + material + "_" + particle;
        // if (dataset == "TWIST") {
        //   csvname += "_sparse";
        // }
        csvname += ".csv";
        std::cout << "START: " << csvname << std::endl;
        if (gSystem->AccessPathName(csvname.c_str())) {
          std::cout << csvname << " does not exist.. Skipping this dataset" << std::endl;
          gr_data[i_material][i_particle][i_dataset] = 0;
          bin_data[i_material][i_particle][i_dataset] = 0;

          for (int i_fit = 0; i_fit < n_fits; ++i_fit) {
            chi2_fns[i_material][i_particle][i_dataset][i_fit] = 0;
          }
          continue;
        }

        TTree* tree = new TTree();
        tree->ReadFile(csvname.c_str());
        int n_points = tree->GetEntries();
        float spec_E[n_points];
        float spec_y[n_points];
        float spec_erry[n_points];
        float i_E = 0;
        tree->SetBranchAddress("E", &i_E);
        float i_Y = 0;
        tree->SetBranchAddress("Y", &i_Y);
        float i_YerrStat = 0;
        if (dataset == "AlCap") { // AlCap has separated out statistical and systematic errors
          tree->SetBranchAddress("YerrStat", &i_YerrStat);
        }
        float i_YerrSyst = 0;
        tree->SetBranchAddress("YerrSyst", &i_YerrSyst);
        for (int i_point = 0; i_point < tree->GetEntries(); ++i_point) {
          tree->GetEntry(i_point);
          spec_E[i_point] = i_E;
          if (dataset == "TWIST") {
            spec_y[i_point] = i_Y;
            spec_erry[i_point] = i_YerrSyst;
          }
          else if (dataset == "AlCap") {
            spec_y[i_point] = i_Y*1e-3;
            spec_erry[i_point] = std::sqrt(i_YerrStat*1e-3*i_YerrStat*1e-3 + i_YerrSyst*1e-3*i_YerrSyst*1e-3);
          }
          else {
            std::cout << "Unknown dataset: " << dataset << std::endl;
            return;
          }
        }

        gr_data[i_material][i_particle][i_dataset] = new TGraphErrors(n_points, spec_E, spec_y, 0, spec_erry);
        gr_data[i_material][i_particle][i_dataset]->SetMarkerStyle(markers[i_dataset]);
        gr_data[i_material][i_particle][i_dataset]->SetLineColor(colours[i_dataset]);
        gr_data[i_material][i_particle][i_dataset]->SetLineWidth(2);
        gr_data[i_material][i_particle][i_dataset]->SetMarkerColor(colours[i_dataset]);
        gr_data[i_material][i_particle][i_dataset]->SetStats(false);

        ROOT::Fit::DataOptions opt;
        ROOT::Fit::DataRange range;
        range.SetRange(spec_E[0],spec_E[n_points-1]);
        std::cout << "Range = " << spec_E[0] << ", " << spec_E[n_points-1] << std::endl;
        bin_data[i_material][i_particle][i_dataset] = new ROOT::Fit::BinData(opt,range);
        ROOT::Fit::FillData(*bin_data[i_material][i_particle][i_dataset], gr_data[i_material][i_particle][i_dataset]);

        for (int i_fit = 0; i_fit < n_fits; ++i_fit) {
          std::string fit_name = fit_names[i_fit];
          std::string fit_expr = fit_exprs[i_fit];
          TF1* fit_fn = new TF1(fit_name.c_str(), fit_expr.c_str(), fit_min,fit_max);

          fit_fn->SetNpx(100000);
          fit_fn->SetLineColor(kBlack);
          ROOT::Math::WrappedMultiTF1 wrap_fit_fn(*fit_fn,1);

          fit_fns[i_material][i_particle][0][i_fit] = fit_fn;
          chi2_fns[i_material][i_particle][i_dataset][i_fit] = new ROOT::Fit::Chi2Function(*bin_data[i_material][i_particle][i_dataset], wrap_fit_fn);
        }
      }

      for (int i_fit = 0; i_fit < n_fits; ++i_fit) {
        GlobalChi2 globalChi2(chi2_fns[i_material][i_particle][0][i_fit], chi2_fns[i_material][i_particle][1][i_fit]);
        ROOT::Fit::Fitter fitter;

        // // create before the parameter settings in order to fix or set range on them
        int n_par = n_pars[i_fit];
        fitter.Config().SetParamsSettings(n_par,init_pars[i_fit]);
        fitter.Config().MinimizerOptions().SetPrintLevel(0);
        fitter.Config().SetMinimizer("Minuit2","Migrad");
        for (int i_par = 0; i_par < n_par; ++i_par) {
          fitter.Config().ParSettings(i_par).SetName(par_names[i_fit][i_par]);
          //            fit_fn->SetParameter(i_par, init_pars[i_fit][i_par]);
          if (is_fixed_par[i_fit][i_par]) {
            fitter.Config().ParSettings(i_par).Fix();
          }
        }


          //        //      fitter.Config().ParSettings(1).SetLimits(1,5);
          //        //      fitter.Config().ParSettings(2).SetLimits(0.001,10);
          //        //      fitter.Config().ParSettings(1).SetLimits(2,10);
          //        //      fitter.Config().ParSettings(2).SetLimits(0,1);
          //        fitter.Config().ParSettings(2).SetLimits(1, 3);


        // fit FCN function directly
        // (specify optionally data size and flag to indicate that is a chi2 fit)
        unsigned int totalSize = 0;
        if ( bin_data[i_material][i_particle][0]) {
          totalSize += bin_data[i_material][i_particle][0]->Size();
        }
        if ( bin_data[i_material][i_particle][1]) {
          totalSize += bin_data[i_material][i_particle][1]->Size();
        }

        fitter.FitFCN(n_par,globalChi2,0,totalSize,true);
        ROOT::Fit::FitResult result = fitter.Result();
        result.Print(std::cout);
        results[i_material][i_particle][0][i_fit] = result;

        //        ROOT::Fit::Fitter fitter2;
        //        fitter2.Config() = fitter.Config();
        // //      fitter2.Config().ParSettings(0).Fix();
        // fitter2.Config().ParSettings(0).Release();
        // fitter2.Config().ParSettings(1).Release();
        // fitter2.Config().ParSettings(2).Fix();
        // //      fitter2.Config().ParSettings(3).Fix();
        // // //      fitter2.Config().ParSettings(3).Release();
        // fitter2.FitFCN(Npar,globalChi2,0,totalSize,true);
        // result = fitter2.Result();
        // result.Print(std::cout);
      }

      // And draw everthing
      TLegend* leg = new TLegend(0.4, 0.78, 0.6, 0.88);
      leg->SetTextSize(0.04);
      leg->SetLineColor(kWhite);
      std::string canvasname = "c_" + material + "_" + particle;
      TCanvas* c = new TCanvas(canvasname.c_str(), canvasname.c_str());
      if (gr_data[i_material][i_particle][0]) {
        gr_data[i_material][i_particle][0]->Draw("APE");
        gr_data[i_material][i_particle][0]->GetXaxis()->SetLimits(0, 40);
        gr_data[i_material][i_particle][0]->SetTitle(std::string(particles[i_particle] + ", " + materials[i_material]).c_str());
        gr_data[i_material][i_particle][0]->GetXaxis()->SetTitle("Energy [MeV]");
        gr_data[i_material][i_particle][0]->GetYaxis()->SetTitle("(Yield / Capture) / MeV");
        leg->AddEntry(gr_data[i_material][i_particle][0], std::string(datasets[0] + " Data").c_str(), "pl");
      }

      if (gr_data[i_material][i_particle][1]) {
        if (gr_data[i_material][i_particle][0]) {
          gr_data[i_material][i_particle][1]->Draw("PESAME");
        }
        else {
          gr_data[i_material][i_particle][1]->Draw("APE");
          gr_data[i_material][i_particle][1]->GetXaxis()->SetLimits(0, 40);
          gr_data[i_material][i_particle][1]->SetTitle(std::string(particles[i_particle] + ", " + materials[i_material]).c_str());
          gr_data[i_material][i_particle][1]->GetXaxis()->SetTitle("Energy [MeV]");
          gr_data[i_material][i_particle][1]->GetYaxis()->SetTitle("(Yield / Capture) / MeV");
        }
        leg->AddEntry(gr_data[i_material][i_particle][1], std::string(datasets[1] + " Data").c_str(), "pl");
      }

      for (int i_fit = 0; i_fit < n_fits; ++i_fit) {
        TF1* fit_fn = fit_fns[i_material][i_particle][0][i_fit];
        auto result = results[i_material][i_particle][0][i_fit];
        int n_par = n_pars[i_fit];

        fit_fn->SetFitResult( result );//, iparTWIST);
        TF1* fit_fn_draw = (TF1*) fit_fn->Clone("fit_fn_draw");
        TF1* fit_fn_extrap = (TF1*) fit_fn->Clone("fit_fn_extrap");
        fit_fn_extrap->SetFitResult( result );//, iparTWIST);
        if (gr_data[i_material][i_particle][0]) {
          gr_data[i_material][i_particle][0]->GetListOfFunctions()->Add(fit_fn_draw);
        }
        if (gr_data[i_material][i_particle][1]) {
          gr_data[i_material][i_particle][1]->GetListOfFunctions()->Add(fit_fn_draw);
        }
        fit_fn_extrap->SetLineStyle(kDashed);
        fit_fn_extrap->SetRange(0,45);
        if (i_fit == 0) { // has a T_th parameter
          fit_fn_extrap->SetRange(fit_fn_extrap->GetParameter(1),45);
        }
        fit_fn_draw->SetLineColor(fit_colours[i_fit]);
        fit_fn_extrap->SetLineColor(fit_colours[i_fit]);
        fit_fn_draw->Draw("LSAME");
        fit_fn_extrap->Draw("LSAME");
      }
      leg->Draw();
      c->SetLogy(1);
      if (save_plots) {
        std::string plotname = basedir + "/AlCapTWISTData_Fits_" + materials[i_material] + "_" + particles[i_particle] + "_LogY.pdf";
        c->SaveAs(plotname.c_str());
      }

      for (int i_fit = 0; i_fit < n_fits; ++i_fit) {
        TF1* fit_fn = fit_fns[i_material][i_particle][0][i_fit];
        auto result = results[i_material][i_particle][0][i_fit];
        int n_par = n_pars[i_fit];

        double E_min = 0;
        if (i_fit == 0) { // has a T_th parameter
          E_min = fit_fn->GetParameter(1);
        }
        double integral = fit_fn->Integral(E_min, 100);
        TMatrixD covMatrix(n_par,n_par);
        result.GetCovarianceMatrix(covMatrix);
        result.PrintCovMatrix(std::cout);
        double error = fit_fn->IntegralError(0.2, 100, result.GetParams(), covMatrix.GetMatrixArray());
        std::cout << fit_fn->GetName() << ": Integral = " << integral << " +/- " << error << std::endl;

        std::stringstream leglabel;
        leglabel << fit_names[i_fit] << " Fit" << std::fixed << std::setprecision(1) << " (#chi^{2} / ndf = " << result.Chi2() << " / " << result.Ndf() << ")";
        //        leg->AddEntry(fit_fn_draw, leglabel.str().c_str(), "l");

        leglabel.str("");
        leglabel << fit_names[i_fit] << " Fit (extrapolated)";
        //        leg->AddEntry(fit_fn_extrap, leglabel.str().c_str(), "l");

        TLatex* latex = new TLatex();
        latex->SetTextAlign(22);
        latex->SetTextSize(0.04);
        latex->SetTextColor(fit_colours[i_fit]);
        double latex_x = 0.4 + (i_fit*0.3);
        double latex_y = 0.7;
        double latex_y_step = 0.05;
        latex->DrawLatexNDC(latex_x, latex_y, (fit_names[i_fit] + " Fit").c_str());
        latex_y -= latex_y_step;
        latex_y -= 0.05;
        latex->DrawLatexNDC(latex_x, latex_y, latex_exprs[i_fit].c_str());
        latex_y -= 0.05; // bit of a buffer around expression
        std::stringstream text;
        for (int i_par = 0; i_par < n_pars[i_fit]; ++i_par) {
          latex_y -= latex_y_step;
          text.str("");
          text << std::fixed << std::setprecision(3) << par_names[i_fit][i_par] << " = " << result.Parameter(i_par);
          if (is_fixed_par[i_fit][i_par]) {
            text << "  fixed";
          }
          else {
            text << " #pm " << result.ParError(i_par);
          }
          latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
        }
        latex_y -= latex_y_step;
        text.str("");
        text << std::fixed << std::setprecision(1) << " (#chi^{2} / ndf = " << result.Chi2() << " / " << result.Ndf() << ")";
        latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
      }
      std::cout << "END " << particle << std::endl;

      c->SetLogy(0);
      if (save_plots) {
        std::string plotname = basedir + "/AlCapTWISTData_Fits_" + materials[i_material] + "_" + particles[i_particle] + ".pdf";
        c->SaveAs(plotname.c_str());
      }
    }
  }



  // TCanvas * c_fit = new TCanvas("Simfit","Simultaneous fit of two histograms",
  //      10,10,700,700);
  // c_fit->Divide(1,2);
  // c_fit->cd(1);
  // gStyle->SetOptFit(1111);

  // gTWISTProtons->GetListOfFunctions()->Add(fit_fn);
  // gTWISTProtons->Draw("APE");

  // c_fit->cd(2);
  // gAlCapProtons->GetListOfFunctions()->Add(fit_fn);
  // gAlCapProtons->Draw("APE");

  // // TF1* fit_fn = new TF1("fit_fn", "[0]*(x/(1+x))^[1] * TMath::Exp(-x/[2])", 0, 20);
  // // const int Npar = 3;
  // // double init_par[Npar] = { 0.105, 1.3279, 5.0};

  // // TF1* fit_fn = new TF1("fit_fn", "[0]*(x/(1+[1]*x))^[2] * TMath::Exp(-x/[3])", 0, 20);
  // // const int Npar = 4;
  // // double init_par[Npar] = { 0.105, 1, 1.3279, 5.0};

}
