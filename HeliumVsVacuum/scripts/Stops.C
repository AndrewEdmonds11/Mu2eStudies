void Stops() {

  const int n_stops = 2;
  std::string stops[n_stops] = {"Target", "OOT"};
  std::string histtitles[n_stops] = {"Target Stops per POT", "OOT Stops per POT"};
  TH1F* hists[n_stops] = {0};
  TCanvas* canvases[n_stops] = {0};

  const int n_settings = 3;
  std::string confs[n_settings] = {"baseline", "1atmArCO2", "1atmHe"};
  double n_POTs[n_settings] = {180e3, 180e3, 160e3};
  double resample_factors[n_settings] = {10, 10, 10};
  Int_t colours[n_settings] = {kBlack, kBlue, kRed};

  for (int i_stop = 0; i_stop < n_stops; ++i_stop) {
    std::string stop = stops[i_stop];
    std::string histtitle = histtitles[i_stop];

    canvases[i_stop] = new TCanvas();

    hists[i_stop] = new TH1F(stop.c_str(), histtitle.c_str(), n_settings,0,n_settings);
    hists[i_stop]->GetXaxis()->SetLabelSize(0.07);
    hists[i_stop]->SetStats(false);
    hists[i_stop]->SetYTitle("Stops / POT / bin");
    hists[i_stop]->SetMinimum(0);

    for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
      std::string conf = confs[i_setting];
      Int_t colour = colours[i_setting];

      std::string filename = "data/val." + stop + "Stops." + conf + ".root";
      TFile* file = new TFile(filename.c_str(), "READ");
      //      TTree* events = (TTree*) file->Get("Events");
      //      int n_events = events->GetEntries();
      std::string foldername = "Validation/SimParticlemv_" + stop + "StopFilter_noName";
      std::string histname = foldername + "/Zstop";
      TH1F* h = (TH1F*) file->Get(histname.c_str());

      h->SetDirectory(0);
      h->SetLineColor(colour);
      int n_events = h->GetEntries();
      std::cout << conf << " " << stop << " Events = " << n_events << std::endl;
      double n_POT = n_POTs[i_setting] * resample_factors[i_setting];
      h->Scale(1.0/n_POT);

      canvases[i_stop]->cd();
      h->Draw("HIST SAME");

      hists[i_stop]->GetXaxis()->SetBinLabel(i_setting+1, conf.c_str());
      hists[i_stop]->SetBinContent(i_setting+1, n_events/n_POT);
      hists[i_stop]->SetBinError(i_setting+1, std::sqrt(n_events)/n_POT);

      file->Close();
    }

    TCanvas* c = new TCanvas();
    hists[i_stop]->SetMarkerSize(2);
    hists[i_stop]->Draw("HIST E TEXT0");
  }

}
