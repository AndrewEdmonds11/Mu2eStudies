void EleBeamFlash() {

  bool save_plots = true;
  std::string basename = "";

  const int n_hists = 1;
  std::string histnames[n_hists] = {"t"};//, "tgtmut"};
  std::string histtitles[n_hists] = {"EleBeamFlash (time)"};//, "Muon OOT Stops (time)"};
  std::string x_titles[n_hists] = {"t [ns]"};//, "t [ns]"};
  std::string draw_opts[n_hists] = {"HIST E SAME"};//, "HIST E SAME"};
  TH1F* hists[n_hists] = {0};
  TCanvas* canvases[n_hists] = {0};

  const int n_settings = 3;
  std::string confs[n_settings] = {"baseline", "1atmArCO2", "1atmHe"};
  double n_POTs[n_settings] = {500e3, 500e3, 500e3};
  double resample_factors[n_settings] = {10, 10, 10};
  Int_t colours[n_settings] = {kBlack, kBlue, kRed};

  TLegend* leg = new TLegend(0.2, 0.65, 0.50, 0.85);
  leg->SetTextSize(0.04);
  leg->SetLineColor(kWhite);
  std::stringstream leglabel;

  for (int i_hist = 0; i_hist < n_hists; ++i_hist) {
    std::string histname = histnames[i_hist];
    std::string histtitle = histtitles[i_hist];

    canvases[i_hist] = new TCanvas();
    canvases[i_hist]->SetLogy();

    //    hists[i_hist] = new TH1F(hist.c_str(), histtitle.c_str(), n_settings,0,n_settings);
    //    hists[i_hist]->GetXaxis()->SetLabelSize(0.07);
    //    hists[i_hist]->SetStats(false);
    //    hists[i_hist]->SetYTitle("Hists / POT / bin");
    //    hists[i_hist]->SetMinimum(0);

    for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
      std::string conf = confs[i_setting];
      Int_t colour = colours[i_setting];

      std::string filename = "data/val.EarlyEleBeamFlash." + conf + ".root";
      TFile* file = new TFile(filename.c_str(), "READ");
      //      TTree* events = (TTree*) file->Get("Events");
      //      int n_events = events->GetEntries();
      std::string foldername = "Validation/StrawGasSteps_compressDetStepMCs_noName";
      std::string hname = foldername + "/" + histname;
      TH1F* h = (TH1F*) file->Get(hname.c_str());

      h->SetDirectory(0);
      int n_events = h->GetEntries();
      std::cout << conf << " Events = " << n_events << std::endl;
      double n_POT = n_POTs[i_setting] * resample_factors[i_setting];
      h->Scale(1.0/n_POT);

      canvases[i_hist]->cd();
      h->SetMaximum(5e-3);
      h->SetTitle(histtitle.c_str());
      h->SetXTitle(x_titles[i_hist].c_str());
      h->SetYTitle("Straw Gas Steps / POT / bin");
      h->SetLineColor(colour);
      h->SetLineWidth(2);
      h->SetStats(false);
      h->Draw(draw_opts[i_hist].c_str());

      //      hists[i_stop]->GetXaxis()->SetBinLabel(i_setting+1, conf.c_str());
      //      hists[i_stop]->SetBinContent(i_setting+1, n_events/n_POT);
      //      hists[i_stop]->SetBinError(i_setting+1, std::sqrt(n_events)/n_POT);

      if (i_hist==0) {
        leglabel.str("");
        leglabel << conf << std::fixed << std::setprecision(4) << " (" << n_events/n_POT << " StrawGasSteps / POT)";
        leg->AddEntry(h, leglabel.str().c_str(), "pl");
      }

      file->Close();
    }
    leg->Draw();
    //    TCanvas* c = new TCanvas();
    //    hists[i_stop]->SetMarkerSize(2);
    //    hists[i_stop]->Draw("HIST E TEXT0");

    if (save_plots) {
      std::string pdfname = basename + "/Mu2eSim_HeliumVsVacuum_EarlyEleBeamFlash_" + histname + ".pdf";
      canvases[i_hist]->SaveAs(pdfname.c_str());
    }
  }

}
