#include "VDHit.h"

void WindowEDeps() {

  const int n_settings = 4;
  std::string settings[n_settings] = {"new", "newThick", "newThickAl", "newThick-noPT"};
  double n_POTs[n_settings] = {0};
  double primary_beam_window_edeps[n_settings] = {0};
  double target_access_window_edeps[n_settings] = {0};
  TH1D* h_PrimaryBeamWindowEDepVsTime[n_settings] = {0};
  std::string bin_labels[n_settings] = {"2.54 mm G4_Ti", "25.4 mm G4_Ti", "25.4 mm G4_Al", "#splitline{   25.4 mm G4_Ti}{(no production target)}"};

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string setting = settings[i_setting];

    std::string filename = "data/window-steps-" + setting + "-wWindowSDs.root";
    TFile* file = new TFile(filename.c_str(), "READ");

    int n_gen_events = ((TH1D*) file->Get("genCountLogger/numEvents"))->GetBinContent(1);

    TTree* primary_beam_window_tree = (TTree*) file->Get("PrimaryBeamWindowSteps/nt");
    VDHit primary_beam_window_hit;
    primary_beam_window_tree->SetBranchAddress("hits", &primary_beam_window_hit);
    TH1D* hist = new TH1D("h_PrimaryBeamWindowEDepVsTime", "Primary Beam Window", 1700,0,1700);

    double total_primary_beam_window_edep = 0;
    for (int i_entry = 0; i_entry < primary_beam_window_tree->GetEntries(); ++i_entry) {
      primary_beam_window_tree->GetEntry(i_entry);
      total_primary_beam_window_edep += primary_beam_window_hit.totalEDep;
      hist->Fill(std::fmod(primary_beam_window_hit.time,1695), primary_beam_window_hit.totalEDep);
    }
    std::cout << "Sum = " << total_primary_beam_window_edep << " MeV" << std::endl;


    TTree* target_access_window_tree = (TTree*) file->Get("TargetAccessWindowSteps/nt");
    VDHit target_access_window_hit;
    target_access_window_tree->SetBranchAddress("hits", &target_access_window_hit);
    
    double total_target_access_window_edep = 0;
    for (int i_entry = 0; i_entry < target_access_window_tree->GetEntries(); ++i_entry) {
      target_access_window_tree->GetEntry(i_entry);
      total_target_access_window_edep += target_access_window_hit.totalEDep;
    }
    std::cout << "Sum = " << total_target_access_window_edep << " MeV" << std::endl;

    n_POTs[i_setting] = n_gen_events;
    primary_beam_window_edeps[i_setting] = total_primary_beam_window_edep;
    target_access_window_edeps[i_setting] = total_target_access_window_edep;
    h_PrimaryBeamWindowEDepVsTime[i_setting] = hist;
  }

  TH1D* h_PrimaryBeamWindowEDeps = new TH1D("h_PrimaryBeamWindowEDeps", "Primary Beam Window", n_settings,0,n_settings);
  TH1D* h_TargetAccessWindowEDeps = new TH1D("h_TargetAccessWindowEDeps", "Target Access Window", n_settings,0,n_settings);
  TCanvas* c_primary_time = new TCanvas();
  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string bin_label = bin_labels[i_setting];
    double scale_factor = (1.0/n_POTs[i_setting]); // MeV per POT
    scale_factor /= 1e3; // GeV / POT
    //    scale_factor *= 3.9e7; // MeV per pulse
    //    scale_factor *= 1e6; // eV per pulse
    //    scale_factor *= 1.6e-19; // J / pulse
    //    scale_factor /= 1695e-9; // J/s / pulse
    h_PrimaryBeamWindowEDeps->Fill(bin_label.c_str(), primary_beam_window_edeps[i_setting] * scale_factor);
    h_TargetAccessWindowEDeps->Fill(bin_label.c_str(), target_access_window_edeps[i_setting] * scale_factor);

    scale_factor /= 1e-9;
    h_PrimaryBeamWindowEDepVsTime[i_setting]->Scale(scale_factor);
    c_primary_time->cd();
    h_PrimaryBeamWindowEDepVsTime[i_setting]->Draw("HIST SAME");
  }

  TCanvas* c_primary = new TCanvas();
  h_PrimaryBeamWindowEDeps->SetStats(false);
  //  h_PrimaryBeamWindowEDeps->SetYTitle("Energy Deposited / Microbunch [W]");
  h_PrimaryBeamWindowEDeps->SetYTitle("Energy Deposited [GeV / POT]");
  h_PrimaryBeamWindowEDeps->GetXaxis()->SetLabelSize(0.05);
  h_PrimaryBeamWindowEDeps->SetMarkerSize(1.5);
  h_PrimaryBeamWindowEDeps->GetYaxis()->SetRangeUser(0, h_PrimaryBeamWindowEDeps->GetMaximum()*1.10);
  h_PrimaryBeamWindowEDeps->Draw("HIST TEXT0");

  TCanvas* c_target = new TCanvas();
  h_TargetAccessWindowEDeps->SetStats(false);
  //  h_TargetAccessWindowEDeps->SetYTitle("Energy Deposited / Microbunch [W]");
  h_TargetAccessWindowEDeps->SetYTitle("Energy Deposited [GeV / POT]");
  h_TargetAccessWindowEDeps->GetXaxis()->SetLabelSize(0.05);
  h_TargetAccessWindowEDeps->SetMarkerSize(1.5);
  h_TargetAccessWindowEDeps->GetYaxis()->SetRangeUser(0, h_TargetAccessWindowEDeps->GetMaximum()*1.10);
  h_TargetAccessWindowEDeps->Draw("HIST TEXT0");

}
