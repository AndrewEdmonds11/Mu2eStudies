void TimePlot() {

  //  TFile* file = new TFile("nts.owner.trkana.CeEndpointMixTriggered.MDC2020km.root", "READ");
  //  TTree* trkana = (TTree*) file->Get("TrkAnaNeg/trkana");

  TFile* file_flash = new TFile("sim-parts-flash.root", "READ");
  TTree* simparts_flash = (TTree*) file_flash->Get("SimParts/ntpssp");

  TH1F* hGenCount_flash = (TH1F*) file_flash->Get("genCountLogger/numEvents");
  auto n_POT_flash = hGenCount_flash->GetBinContent(1);
  std::cout << "AE: n_POT = " << n_POT_flash << std::endl;
  double early_flash_prescale = 1000;
  n_POT_flash /= early_flash_prescale; 

  double min_t0 = -250;
  double max_t0 = 2000;//1850;
  double t0_width = 25;
  int n_t0_bins = (max_t0 - min_t0) / t0_width;


  TFile* file_ces = new TFile("sim-parts-ces.root", "READ");
  TTree* simparts_ces = (TTree*) file_ces->Get("SimParts/ntpssp");

  TH1F* hGenCount_ces = (TH1F*) file_ces->Get("genCountLogger/numEvents");
  auto n_POT_ces = hGenCount_ces->GetBinContent(1);
  n_POT_ces *= 3.9e7;


  TH1F* hTime_POT = new TH1F("hTime_POT", "", n_t0_bins,min_t0,max_t0);
  simparts_ces->Draw("egt>>hTime_POT", "pdg==2212 && scc==56", "HIST");
  hTime_POT->Scale(1.0/hTime_POT->Integral());
  hTime_POT->SetStats(false);
  hTime_POT->SetFillColor(kGray);
  hTime_POT->SetLineWidth(2);
  hTime_POT->SetLineColor(kGray+2);
  hTime_POT->SetYTitle("Arbitrary Units");
  hTime_POT->SetXTitle("Time [ns]");


  TH1F* hTime_POT2 = new TH1F("hTime_POT2", "", n_t0_bins,min_t0,max_t0);
  simparts_ces->Draw("egt+1695>>hTime_POT2", "pdg==2212 && scc==56", "HIST SAME");
  hTime_POT2->Scale(1.0/hTime_POT2->Integral());
  hTime_POT2->SetFillColor(kGray);
  hTime_POT2->SetLineWidth(2);
  hTime_POT2->SetLineColor(kGray+2);


  TH1F* hTime_flash = new TH1F("hTime_flash", "", n_t0_bins,min_t0,max_t0);
  simparts_flash->Draw("egt>>hTime_flash", "pdg==11", "HIST SAME");
  hTime_flash->Scale(1.0/hTime_flash->Integral());
  hTime_flash->SetFillColor(kAzure-4);
  hTime_flash->SetLineWidth(2);
  hTime_flash->SetLineColor(kBlue);

  TH1F* hTime_flash2 = new TH1F("hTime_flash2", "", n_t0_bins,min_t0,max_t0);
  simparts_flash->Draw("egt+1695>>hTime_flash2", "pdg==11", "HIST SAME");
  hTime_flash2->Scale(1.0/hTime_flash2->Integral());
  hTime_flash2->SetFillColor(kAzure-4);
  hTime_flash2->SetLineWidth(2);
  hTime_flash2->SetLineColor(kBlue);

  TH1F* hTime_muon = new TH1F("hTime_muon", "", n_t0_bins,min_t0,max_t0);
  simparts_ces->Draw("egt>>hTime_muon", "pdg==13", "HIST SAME");
  hTime_muon->Scale(1.0/hTime_muon->Integral());
  hTime_muon->SetFillColor(kPink-4);
  hTime_muon->SetLineWidth(2);
  hTime_muon->SetLineColor(kRed);


  TH1F* hTime_bkg = new TH1F("hTime_bkg", "", n_t0_bins,min_t0,max_t0);
  simparts_ces->Draw("egt>>hTime_bkg", "scc==167", "HIST SAME");
  hTime_bkg->Scale(1.0/hTime_bkg->Integral());
  hTime_bkg->SetFillColor(kSpring-5);
  hTime_bkg->SetLineWidth(2);
  hTime_bkg->SetLineColor(kSpring-7);

  TH1F* hTime_signal = new TH1F("hTime_signal", "", n_t0_bins,min_t0,max_t0);
  hTime_signal->SetBinContent(hTime_signal->FindBin(1000), 0.005);
  hTime_signal->Draw("HIST SAME");
  hTime_signal->SetLineColor(kBlack);
  hTime_signal->SetFillColor(kBlack);

  TLegend* leg = new TLegend(0.26, 0.5, 0.75, 0.89);
  leg->SetTextSize(0.035);
  leg->SetLineColor(kWhite);
  leg->AddEntry(hTime_POT, "proton on target", "f");
  leg->AddEntry(hTime_flash, "beam flash", "f");
  leg->AddEntry(hTime_muon, "muon arrival", "f");
  leg->AddEntry(hTime_bkg, "#splitline{muonic atom decay/capture}{products (in digitization window)}", "f");
  leg->AddEntry(hTime_signal, "conversion electron signal", "f");
  leg->Draw();

  double digi_line_y = hTime_bkg->GetMaximum()*1.10;
  double digi_start = 450;
  double digi_end = 1600;
  TBox* digi_box = new TBox(digi_start, 0, digi_end, digi_line_y);
  digi_box->SetLineColor(kRed);
  digi_box->SetLineWidth(2);
  digi_box->SetLineStyle(kDashed);
  digi_box->SetFillStyle(0);
  digi_box->Draw("LSAME");
  TLatex* latex = new TLatex();
  latex->SetTextSize(0.03);
  latex->SetTextColor(kRed);
  latex->SetTextAlign(21);
  latex->DrawLatex(digi_start+((digi_end-digi_start)/2.0), digi_line_y*1.10, "digitization window");
}
