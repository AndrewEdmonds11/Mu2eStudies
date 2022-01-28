void CeDio()  {

  const int n_components = 2;
  std::string names[n_components] = {"Ce", "Dio"};
  std::string filenames[n_components] = {"/mu2e/data/users/edmonds/GitHub/trkana-CeEndpoint-mix.trkqual-dev.root", "/mu2e/data/users/edmonds/GitHub/trkana-flateminus-mix-cat.MDC2018h.root"};
  std::string draw_options[n_components] = {"HIST E", "HIST E SAME"};
  std::string weights[n_components] = {"1e-16*0.609*0.0015*3.6e20", "evtwt.DIOWeight*0.391*0.0015*3.6e20*35"};
  Int_t colours[n_components] = {kRed, kBlue};

  TCanvas* c = new TCanvas();
  c->SetLogy();
  for (int i_component = 0; i_component < n_components; ++i_component) {
    std::string i_name = names[i_component];
    std::string i_filename = filenames[i_component];
    std::string i_draw_option = draw_options[i_component];
    std::string i_evtwt = weights[i_component];
    Int_t i_colour = colours[i_component];

    std::cout << i_name << ": " << i_filename << std::endl;

    TFile* file = new TFile(i_filename.c_str(), "READ");
    int n_gen_events = ((TH1F*) file->Get("genCountLogger/numEvents"))->GetBinContent(1);

    TTree* trkana = (TTree*) file->Get("TrkAnaNeg/trkana");
    std::string i_histname = "hMom_" + i_name;
    double min_mom = 95;
    double max_mom = 110;
    double mom_width = 0.05;
    int n_mom_bins = (max_mom - min_mom) / mom_width;
    TH1F* hMom = new TH1F(i_histname.c_str(), "", n_mom_bins,min_mom,max_mom);

    std::string drawcmd = "deent.mom>>" + i_histname;
    std::string cutcmd = i_evtwt + "*(de.status>0 && de.t0>700 && de.t0<1695 && deent.td>0.577350 && deent.td<1.000 && deent.d0>-80 && deent.d0<105 && deent.d0+2./deent.om>450 && deent.d0+2./deent.om<680 && (bestcrv<0 || (de.t0-crvinfo._timeWindowStart[bestcrv]<-50 || de.t0-crvinfo._timeWindowStart[bestcrv]>150.0)) && dequal.TrkQual>0.8 && dequal.TrkPID>0.95 && ue.status<=0)";
    //    std::cout << drawcmd << ", " << cutcmd << std::endl;
    trkana->Draw(drawcmd.c_str(), cutcmd.c_str(), "goff");
    hMom->Scale(1.0 / n_gen_events);
    hMom->SetMaximum(1e3);
    hMom->SetLineColor(i_colour);
    hMom->Draw(i_draw_option.c_str());
    int min_integral_bin = hMom->GetXaxis()->FindBin(103.85);
    int max_integral_bin = hMom->GetXaxis()->FindBin(105.10);
    double integral = hMom->Integral(min_integral_bin, max_integral_bin);
    std::cout << "Integral 103.85 - 105.1 = " << integral << std::endl;
  }

}
