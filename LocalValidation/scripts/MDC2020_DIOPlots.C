void MDC2020_DIOPlots() {

  std::string filename = "nts.owner.trkana-reco.version.sequencer.root";
  TFile* file = new TFile(filename.c_str(), "READ");

  std::string treename = "TrkAnaNeg/trkana";
  TTree* trkana = (TTree*) file->Get(treename.c_str());

  int min_gen = 0;
  int max_gen = 180;
  double gen_bin_width = 2;
  int n_gen_bins = (max_gen - min_gen) / gen_bin_width;

  double min_mom = 0;
  double max_mom = 150;
  double mom_bin_width = 1;
  int n_mom_bins = (max_mom - min_mom) / mom_bin_width;

  TH2F* hMomVsGen = new TH2F("hMomVsGen", "", n_gen_bins,min_gen,max_gen, n_mom_bins,min_mom,max_mom);
  trkana->Draw("deent.mom:demcsim.gen>>hMomVsGen", "", "COLZ");
  hMomVsGen->SetStats(false);
  hMomVsGen->SetXTitle("Process Code (demcsim.gen)");
  hMomVsGen->SetYTitle("Reco Momentum (deent,mom) [MeV/c]");

  const int n_gen_labels = 3;
  double x_gen_labels[n_gen_labels] = {14, 114, 167};
  std::string gen_labels[n_gen_labels] = {"Decay", "DIO", "mu2eCeMinusEndpoint"};
  TLatex* latex = new TLatex();
  latex->SetTextAngle(90);
  latex->SetTextAlign(11);
  //  latex->SetTextColor(kRed);
  for (int i_gen_label = 0; i_gen_label < n_gen_labels; ++i_gen_label) {
    double latex_x = x_gen_labels[i_gen_label]-gen_bin_width;
    double latex_y = 40;
    std::string text = gen_labels[i_gen_label];
    latex->DrawLatex(latex_x, latex_y, text.c_str());
  }

  TCanvas* c = new TCanvas();
  double min_dio_mom = 35;
  double max_dio_mom = 55;
  double dio_mom_width = 0.1;
  int n_dio_mom_bins = (max_dio_mom - min_dio_mom) / dio_mom_width;
  TH1F* hTrueMom = new TH1F("hTrueMom", "", n_dio_mom_bins,min_dio_mom,max_dio_mom);
  trkana->Draw("demcsim.mom>>hTrueMom", "demcsim.gen==114", "COLZ");
  hTrueMom->SetTitle("Initial Momentum of Reconstructed DIOs (demcsim.gen==114)");
  hTrueMom->SetXTitle("Initial Momentum (demcsim.mom) [MeV/c]");
  std::stringstream axistitle;
  axistitle.str("");
  axistitle << "Count / " << dio_mom_width << " MeV/c";
  hTrueMom->SetYTitle(axistitle.str().c_str());

  TCanvas* c2 = new TCanvas();
  gStyle->SetOptStat(111111);
  double min_r = 0;
  double max_r = 800;
  double r_width = 10;
  int n_r_bins = (max_r - min_r) / r_width;
  double min_z = -3500;
  double max_z = 2000;
  double z_width = 100;
  int n_z_bins = (max_z - min_z) / z_width;
  TH2F* hStartPosRZ = new TH2F("hStartPosRZ", "", n_z_bins,min_z,max_z, n_r_bins,min_r,max_r);
  trkana->Draw("sqrt(demcsim.posx^2+demcsim.posy^2):demcsim.posz>>hStartPosRZ", "demcsim.gen==114", "COLZ");
  hStartPosRZ->SetTitle("Initial Position of Reconstructed DIOs (demcsim.gen==114)");
  hStartPosRZ->SetXTitle("Initial Z Position (demcsim.posz, det. coords) [mm]");
  //  axistitle.str("");
  //  axistitle << "Count / " << dio_mom_width << " MeV/c";
  hStartPosRZ->SetYTitle("Initial R Position (det. coords) [mm]");

  latex->SetTextAngle(0);
  latex->SetTextAlign(22);
  latex->DrawLatex(-2700, 350, "IPA");
  latex->DrawLatex(1750, 300, "???");

}
