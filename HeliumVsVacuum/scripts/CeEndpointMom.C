void CeEndpointMom() {

  bool save_plots = true;
  std::string basename = "/mu2e/app/users/edmonds/plots/2021-07-29/";

  const int n_settings = 3;
  std::string confs[n_settings] = {"baseline", "1atmArCO2", "1atmHe"};
  double n_CEs[n_settings] = {10e3, 10e3, 10e3};
  Int_t colours[n_settings] = {kBlack, kBlue, kRed};

  TLegend* leg = new TLegend(0.15, 0.65, 0.40, 0.85);
  leg->SetTextSize(0.04);
  leg->SetLineColor(kWhite);
  std::stringstream leglabel;

  TCanvas* c = new TCanvas();

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string conf = confs[i_setting];
    Int_t colour = colours[i_setting];

    std::string filename = "data/nts.edmonds.CeEndpoint_vd-steps." + conf + ".root";
    TFile* file = new TFile(filename.c_str(), "READ");
    std::string treename = "VDSteps/nt";
    TTree* vdsteps = (TTree*) file->Get(treename.c_str());

    std::string histname = "hMom_" + conf;
    TH1F* hMom = new TH1F(histname.c_str(), "", 100,95,110);
    std::string drawcmd = "pmag>>" + histname;
    vdsteps->Draw(drawcmd.c_str(), "volumeCopy==13", "goff");

    hMom->SetDirectory(0);

    int n_events = hMom->GetEntries();
    int n_plot_events = hMom->Integral();
    double min_integral_bin = hMom->GetXaxis()->FindBin(103.85);
    double max_integral_bin = hMom->GetXaxis()->FindBin(105.10);
    int n_window_events = hMom->Integral(min_integral_bin,max_integral_bin);//hMom->GetEntries();
    std::cout << conf << " Events = " << n_events << std::endl;
    double n_CE = n_CEs[i_setting];
    hMom->Scale(1.0/n_CE);

    hMom->SetTitle("True Mom at Tracker Entrance");
    hMom->SetXTitle("Momentum [MeV/c]");
    hMom->SetYTitle("N Ce / Generated Event / bin");
    hMom->SetLineColor(colour);
    hMom->SetLineWidth(2);
    hMom->SetStats(false);
    hMom->SetMaximum(0.11);
    hMom->Draw("HIST E SAME");

    leglabel.str("");
    leglabel << conf << std::fixed << std::setprecision(4) << " (" << n_events << ", " << n_plot_events << ", " << n_window_events << ")";
    leg->AddEntry(hMom, leglabel.str().c_str(), "pl");

    file->Close();
  }
  leg->SetHeader("config (total events, p>95 MeV/c, 103.85 < p < 105.1 MeV/c)");
  leg->Draw();

  if (save_plots) {
    std::string pdfname = basename + "/Mu2eSim_HeliumVsVacuum_CeEndpoint_mom.pdf";
    c->SaveAs(pdfname.c_str());
  }
}
