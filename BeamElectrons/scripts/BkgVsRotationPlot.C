void BkgVsRotationPlot() {

  TTree* tree = new TTree();
  tree->ReadFile("scripts/beamElectrons.csv");

  Int_t offsetSim = 0;
  tree->SetBranchAddress("offsetSim", &offsetSim);
  Int_t byHandSim = 0;
  tree->SetBranchAddress("byHandSim", &byHandSim);
  Int_t solCalcSim = 0;
  tree->SetBranchAddress("solCalcSim", &solCalcSim);
  Int_t MDC2020Sim = 0;
  tree->SetBranchAddress("MDC2020Sim", &MDC2020Sim);
  Float_t fieldRotation = 0;
  tree->SetBranchAddress("fieldRotation", &fieldRotation);
  Float_t nbkg = 0;
  tree->SetBranchAddress("nbkg", &nbkg);
  Float_t NPOT = 0;
  tree->SetBranchAddress("NPOT", &NPOT);
  Int_t useInFit = 0;
  tree->SetBranchAddress("useInFit", &useInFit);

  TGraphErrors* gr_offsetSim = new TGraphErrors();
  TGraphErrors* gr_byHandSim = new TGraphErrors();
  TGraphErrors* gr_solCalcSim = new TGraphErrors();
  TGraphErrors* gr_MDC2020Sim = new TGraphErrors();
  TGraphErrors* gr_forFit = new TGraphErrors();

  Int_t offsetColor = kBlack;
  Int_t byHandColor = kRed;
  Int_t solCalcColor = kBlue;
  Int_t MDC2020Color = kMagenta;

  double prob_scatter_into_tracker_fromTS1 = 1.04e-8;
  double prob_get_toTS5 = 129/23561.0;
  double prob_scatter_into_tracker = 2e-6;//1.04e-8/prob_get_toTS5;
  double extinction = 1e-10;
  double reco_eff = 1;//0.12;
  double N_POT_all = 3.6e20;
  std::vector<double> missing_points;
  std::vector<double> missing_point_POTs;
  std::vector<Int_t> missing_point_colors;
  for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
    tree->GetEntry(i_entry);

    double x = std::fabs(fieldRotation);
    if (nbkg==0) {
      missing_points.push_back(x);
      missing_point_POTs.push_back(NPOT);
      if(offsetSim==1) { missing_point_colors.push_back(offsetColor); }
      else if(byHandSim==1) { missing_point_colors.push_back(byHandColor); }
      else if(solCalcSim==1) { missing_point_colors.push_back(solCalcColor); }
      else if(MDC2020Sim==1) { missing_point_colors.push_back(MDC2020Color); }
    }
    double y = (nbkg*prob_scatter_into_tracker*extinction*reco_eff*N_POT_all)/NPOT;
    double y_err = (std::sqrt(nbkg)*(prob_scatter_into_tracker*extinction*reco_eff*N_POT_all))/NPOT;
    if (offsetSim==1) {
      gr_offsetSim->SetPoint(gr_offsetSim->GetN(), x, y);
      gr_offsetSim->SetPointError(gr_offsetSim->GetN()-1, 0, y_err);
    }
    if (byHandSim==1) {
      gr_byHandSim->SetPoint(gr_byHandSim->GetN(), x, y);
      gr_byHandSim->SetPointError(gr_byHandSim->GetN()-1, 0, y_err);
    }
    if (solCalcSim==1) {
      gr_solCalcSim->SetPoint(gr_solCalcSim->GetN(), x, y);
      gr_solCalcSim->SetPointError(gr_solCalcSim->GetN()-1, 0, y_err);
    }
    if (MDC2020Sim==1) {
      gr_MDC2020Sim->SetPoint(gr_MDC2020Sim->GetN(), x, y);
      gr_MDC2020Sim->SetPointError(gr_MDC2020Sim->GetN()-1, 0, y_err);
    }
    if (useInFit==1) {
      gr_forFit->SetPoint(gr_forFit->GetN(), x, y);
      gr_forFit->SetPointError(gr_forFit->GetN()-1, 0, y_err);
      // if (solCalcSim==1) {
      // 	x = 27.8; // also put a point at 27.8 mrad so it flattens out like offsetSim does
      // 	gr_forFit->SetPoint(gr_forFit->GetN(), x, y);
      // 	gr_forFit->SetPointError(gr_forFit->GetN()-1, 0, y_err);
      // }
    }
  }

  TCanvas* c = new TCanvas();
  c->SetLogy();

  gr_offsetSim->SetMarkerColor(offsetColor);
  gr_offsetSim->SetLineColor(offsetColor);
  gr_offsetSim->SetMarkerStyle(kFullCircle);
  gr_offsetSim->SetLineWidth(2);
  gr_offsetSim->Draw("APE");
  gr_offsetSim->GetXaxis()->SetTitle("Negative Field Rotation [mrad]");
  gr_offsetSim->GetXaxis()->SetLimits(-2,30);
  //  gr_offsetSim->GetYaxis()->SetTitle("Number of High-Energy Electrons Exiting TS / POT");
  //  gr_offsetSim->GetYaxis()->SetRangeUser(1e-9, 1e-4);
  gr_offsetSim->GetYaxis()->SetTitle("Number of Background Events");
  gr_offsetSim->GetYaxis()->SetRangeUser(1e-4, 2);

  gr_byHandSim->SetMarkerColor(byHandColor);
  gr_byHandSim->SetLineColor(byHandColor);
  gr_byHandSim->SetMarkerStyle(kFullCircle);
  gr_byHandSim->SetLineWidth(2);
  gr_byHandSim->Draw("PE SAME");

  gr_solCalcSim->SetMarkerColor(solCalcColor);
  gr_solCalcSim->SetLineColor(solCalcColor);
  gr_solCalcSim->SetMarkerStyle(kFullCircle);
  gr_solCalcSim->SetLineWidth(2);
  gr_solCalcSim->Draw("PE SAME");

  gr_MDC2020Sim->SetMarkerColor(MDC2020Color);
  gr_MDC2020Sim->SetLineColor(MDC2020Color);
  gr_MDC2020Sim->SetMarkerStyle(kFullCircle);
  gr_MDC2020Sim->SetLineWidth(2);
  gr_MDC2020Sim->Draw("PE SAME");

  TLegend* leg = new TLegend(0.15, 0.5, 0.45, 0.85);
  leg->SetLineColor(kWhite);
  leg->SetTextSize(0.04);
  leg->AddEntry(gr_offsetSim, "Vertical Offset Proxy", "pl");
  leg->AddEntry(gr_byHandSim, "By-Hand Rotation", "pl");
  leg->AddEntry(gr_solCalcSim, "SolCalc Field Maps", "pl");
  leg->AddEntry(gr_MDC2020Sim, "MDC2020", "pl");
  leg->Draw();
  c->Modified();
  c->Draw();
 
  for (int i_missing_point = 0; i_missing_point < missing_points.size(); ++i_missing_point) {
    double x = missing_points.at(i_missing_point);
    double n_POT = missing_point_POTs.at(i_missing_point);
    Int_t color = missing_point_colors.at(i_missing_point);
    double y_min = std::pow(10,c->GetUymin());
    double y_max = (1.*prob_scatter_into_tracker*extinction*reco_eff*N_POT_all)/n_POT;//std::pow(10,c->GetUymax());
    //    std::cout << y_min << ", " << y_max << std::endl;
    TArrow* arrow = new TArrow(x, y_min*1.1, x, y_min*2.5, 0.01, "<");
    arrow->SetLineWidth(2);
    arrow->SetLineColor(color);
    arrow->Draw();

    if(i_missing_point == 0) {
      TLatex* latex = new TLatex();
      latex->SetTextSize(0.03);
      latex->DrawLatex(x, y_min*3, "simulations with zero high-energy electrons exiting TS");
    }
  }

  TLatex* latex = new TLatex();
  latex->SetTextSize(0.03);
  std::stringstream text;
  text.str("");
  text << "Assuming: ";
  double start_x = 0.45;
  double start_y = 0.5;
  double y_step = -0.05;
  double latex_x = start_x;
  double latex_y = start_y;
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

  text.str("");
  text << "  Prob Scatter Into Tracker From TS Exit = " << prob_scatter_into_tracker;
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

  text.str("");
  text << "  Extinction Factor = " << extinction;
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

  text.str("");
  text << "  Reconstruction Efficiency = " << reco_eff;
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;


  text.str("");
  text << "  Total POT = " << N_POT_all;
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

  // gr_forFit->SetMarkerColor(kBlack);
  // gr_forFit->SetMarkerStyle(kOpenCircle);
  // gr_forFit->SetMarkerSize(2);
  // gr_forFit->SetLineWidth(2);
  //  gr_forFit->Draw("P E0 SAME");

  TCanvas* c_fit = new TCanvas();
  c_fit->SetLogy();
  gr_forFit->SetMarkerStyle(kFullCircle);
  gr_forFit->Draw("APE");
  gr_forFit->GetXaxis()->SetLimits(-2,30);
  //  gr_forFit->GetYaxis()->SetRangeUser(1e-9, 1e-4);
  gr_forFit->GetYaxis()->SetRangeUser(1e-4, 2);
  gr_forFit->GetXaxis()->SetTitle("Negative Field Rotation [mrad]");
  gr_forFit->GetYaxis()->SetTitle("Number of Background Events");

  TF1* fitFn = new TF1("fitFn", "TMath::Exp([0]*x + [1]);", -2, 30);
  gr_forFit->Fit(fitFn, "R");

  double y_limit = 0.1;//0.01;
  double x0 = -2; double x1 = fitFn->GetX(y_limit);
  TLine* line = new TLine(x0, y_limit, x1, y_limit);
  line->SetLineStyle(kDashed);
  line->Draw("LSAME");

  TLine* line2 = new TLine(x1, 0, x1, y_limit);
  line2->SetLineStyle(kDashed);
  line2->Draw("LSAME");

  latex->SetTextSize(0.035);
  latex_x = 0.35;
  latex_y = 0.3;
  text.str("");
  text << "Worried about a background of " << y_limit << " events?";
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

  text.str("");
  text << "#rightarrow need to know field misalignment to " << std::fixed << std::setprecision(1) << x1 << " mrad";
  latex->DrawLatexNDC(latex_x, latex_y, text.str().c_str());
  latex_y += y_step;

}
