#include "VDHit.h"

void PSEndCapComparisons() {

  gStyle->SetOptStat(111111);

  bool save_plots = true;
  //  std::string basedir = "plots/2021-12-15/";
  std::string basedir = "plots/2022-01-19/";

  int vdid = 20;
  std::string vdname = "PS End Cap";
  std::string vdstr = "20";

  // int vdid = 1;
  // std::string vdname = "TS Entrance";
  // std::string vdstr = "1";

  // int vdid = 8;
  // std::string vdname = "TS Exit";
  // std::string vdstr = "8";

  //  const int n_settings = 2;
  //  std::string settings[n_settings] = {"old", "new"};
  //  Int_t colours[n_settings] = {kBlack, kRed};

  const int n_settings = 4;
  std::string settings[n_settings] = {"new", "new_wWindowSDs", "newThick_wWindowSDs", "newThickAl_wWindowSDs"};
  Int_t colours[n_settings] = {kBlack, kRed, kBlue, kMagenta};
  std::string leglabels[n_settings] = {"2.54 mm, G4_Ti Windows (DocDB-40463)", "2.54 mm, G4_Ti Windows (re-run)", "25.4 mm (1\"), G4_Ti Windows", "25.4 mm (1\"), G4_Al Windows"};
  double normalizations[n_settings] = {0};

  const int n_directions = 3;
  std::string directions[n_directions] = {"All", "Dwn", "Up"};
  std::string dirtitles[n_directions] = {"All", "Downstream", "Upstream"};
  const int i_all_direction = 0;
  const int i_dwn_direction = 1;
  const int i_up_direction = 2;

  const int n_particles = 9;
  int pdgIds[n_particles] = {0, 11, -11, 22, 2212, -211, 211, 13, -13}; // 0 = all particles
  std::string particles[n_particles] = {"all", "elec", "posi", "phot", "prot", "pi-", "pi+", "mu-", "mu+"};
  std::string particle_titles[n_particles] = {"Particles", "Electrons", "Positrons", "Photons", "Protons", "Negative Pions", "Positive Pions", "Negative Muons", "positive Muons"};

  TH2D* h_XYs[n_particles][n_directions][n_settings] = {0};
  TH1D* h_KinEs[n_particles][n_directions][n_settings] = {0};
  TH1D* h_Zs[n_particles][n_directions][n_settings] = {0};
  TH1D* h_times[n_particles][n_directions][n_settings] = {0};

  TCanvas* c_XYs[n_particles][n_directions] = {0};
  TCanvas* c_KinEs[n_particles][n_directions] = {0};
  TCanvas* c_Zs[n_particles][n_directions] = {0};
  TCanvas* c_times[n_particles][n_directions] = {0};


  for (int i_direction = 0; i_direction < n_directions; ++i_direction) {
    std::string direction = directions[i_direction];

    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      std::string particle = particles[i_particle];

      std::string canvasname = "c_XY" + direction + "_" + particle + "_VD" + vdstr;
      c_XYs[i_particle][i_direction] = new TCanvas(canvasname.c_str(), canvasname.c_str(), 800,800);
      c_XYs[i_particle][i_direction]->Divide(2,2);

      canvasname = "c_KinE" + direction + "_" + particle + "_VD" + vdstr;
      c_KinEs[i_particle][i_direction] = new TCanvas(canvasname.c_str(), canvasname.c_str());
      c_KinEs[i_particle][i_direction]->SetLogy();

      canvasname = "c_Z" + direction + "_" + particle + "_VD" + vdstr;
      c_Zs[i_particle][i_direction] = new TCanvas(canvasname.c_str(), canvasname.c_str());

      canvasname = "c_time" + direction + "_" + particle + "_VD" + vdstr;
      c_times[i_particle][i_direction] = new TCanvas(canvasname.c_str(), canvasname.c_str());
      c_times[i_particle][i_direction]->SetLogy();
    }
  }

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string setting = settings[i_setting];
    Int_t colour = colours[i_setting];

    std::string filename = "data/vd-steps-" + setting + ".root";
    //    std::string filename = "data/vd-steps-resampled-" + setting + ".root";
    TFile* file = new TFile(filename.c_str(), "READ");

    int n_gen_events = ((TH1D*) file->Get("genCountLogger/numEvents"))->GetBinContent(1);
    //    n_gen_events = 50e6;
    std::cout << setting << ": N_POT = " << n_gen_events << std::endl;
    normalizations[i_setting] = n_gen_events;

    TTree* vd_steps = (TTree*) file->Get("VDSteps/nt");
    int n_steps = vd_steps->GetEntries();

    for (int i_direction = 0; i_direction < n_directions; ++i_direction) {
      std::string direction = directions[i_direction];
      std::string dirtitle = dirtitles[i_direction];
      
      for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
	std::string particle = particles[i_particle];
	std::string particle_title = particle_titles[i_particle];

	std::string histname = "h_XY_" + direction + "_" + particle + "_" + setting;
	std::string histtitle = "XY of " + dirtitle + " " + particle_title + " at " + vdname + " VD (#" + vdstr + ")";
	double min_x = 3100;
	double max_x = 4800;
	double x_width = 10;
	int n_x_bins = (max_x - min_x) / x_width;
	double min_y = -800;
	double max_y = 800;
	double y_width = 10;
	int n_y_bins = (max_y - min_y) / y_width;
	h_XYs[i_particle][i_direction][i_setting] = new TH2D(histname.c_str(), histtitle.c_str(), n_x_bins,min_x,max_x, n_y_bins,min_y,max_y);
	h_XYs[i_particle][i_direction][i_setting]->SetDirectory(0);

	histname = "h_KinE_" + direction + "_" + particle + "_" + setting;
	histtitle = "Kinetic Energy of " + dirtitle + " " + particle_title + " at " + vdname + " VD (#" + vdstr + ")";
	double min_e = 0;
	double max_e = 1000;
	double e_width = 1.0;
	if (direction == "Dwn") {
	  max_e = 200;
	}
	if (particle == "prot") {
	  max_e = 8500;
	  e_width = 25;
	}
	int n_e_bins = (max_e - min_e) / e_width;
	h_KinEs[i_particle][i_direction][i_setting] = new TH1D(histname.c_str(), histtitle.c_str(), n_e_bins,min_e,max_e);
	h_KinEs[i_particle][i_direction][i_setting]->SetLineColor(colour);
	h_KinEs[i_particle][i_direction][i_setting]->SetDirectory(0);

	histname = "h_Z_" + direction + "_" + particle + "_" + setting;
	histtitle = "Z of " + dirtitle + " " + particle_title + " at " + vdname + " VD (#" + vdstr + ")";
	double min_z = -9500;
	double max_z = -8000;
	double z_width = 10;
	int n_z_bins = (max_z - min_z) / z_width;
	h_Zs[i_particle][i_direction][i_setting] = new TH1D(histname.c_str(), histtitle.c_str(), n_z_bins,min_z,max_z);
	h_Zs[i_particle][i_direction][i_setting]->SetLineColor(colour);
	h_Zs[i_particle][i_direction][i_setting]->SetDirectory(0);

	histname = "h_time_" + direction + "_" + particle + "_" + setting;
	histtitle = "Time of " + dirtitle + " " + particle_title + " at " + vdname + " VD (#" + vdstr + ")";
	double min_time = 0;
	double max_time = 1800;
	double time_width = 1;
	int n_time_bins = (max_time - min_time) / time_width;
	h_times[i_particle][i_direction][i_setting] = new TH1D(histname.c_str(), histtitle.c_str(), n_time_bins,min_time,max_time);
	h_times[i_particle][i_direction][i_setting]->SetLineColor(colour);
	h_times[i_particle][i_direction][i_setting]->SetDirectory(0);
      }
    }

    VDHit hit;
    vd_steps->SetBranchAddress("hits", &hit);
    //    n_steps = 10;
    for (int i_step = 0; i_step < n_steps; ++i_step ) {
      vd_steps->GetEntry(i_step);
      if (hit.volumeCopy == vdid) {
	for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
	  int pdgId = pdgIds[i_particle];
	  
	  if (pdgId == 0 || pdgId == hit.pdgId) {
	    h_KinEs[i_particle][i_all_direction][i_setting]->Fill(hit.ek);
	    h_XYs[i_particle][i_all_direction][i_setting]->Fill(hit.x, hit.y);
	    h_Zs[i_particle][i_all_direction][i_setting]->Fill(hit.z);
	    h_times[i_particle][i_all_direction][i_setting]->Fill(std::fmod(hit.time,1695));
	    if (hit.pz>0) {
	      h_KinEs[i_particle][i_dwn_direction][i_setting]->Fill(hit.ek);
	      h_XYs[i_particle][i_dwn_direction][i_setting]->Fill(hit.x, hit.y);
	      h_Zs[i_particle][i_dwn_direction][i_setting]->Fill(hit.z);
	      h_times[i_particle][i_dwn_direction][i_setting]->Fill(std::fmod(hit.time,1695));
	    }
	    else if (hit.pz<0) {
	      h_KinEs[i_particle][i_up_direction][i_setting]->Fill(hit.ek);
	      h_XYs[i_particle][i_up_direction][i_setting]->Fill(hit.x, hit.y);
	      h_Zs[i_particle][i_up_direction][i_setting]->Fill(hit.z);
	      h_times[i_particle][i_up_direction][i_setting]->Fill(std::fmod(hit.time,1695));
	    }
	  }
	}
      }
    }

    std::stringstream axislabel;
    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      for (int i_direction = 0; i_direction < n_directions; ++i_direction) {
	c_KinEs[i_particle][i_direction]->cd();
	h_KinEs[i_particle][i_direction][i_setting]->Scale(1.0/n_gen_events);
	axislabel.str("");
	axislabel << "Steps / " << h_KinEs[i_particle][i_direction][i_setting]->GetBinWidth(1) << " MeV / POT";
	h_KinEs[i_particle][i_direction][i_setting]->SetXTitle("Energy [MeV]");
	h_KinEs[i_particle][i_direction][i_setting]->SetYTitle(axislabel.str().c_str());
	std::cout << h_KinEs[i_particle][i_direction][i_setting]->GetName() << ": " << h_KinEs[i_particle][i_direction][i_setting] << std::endl;
	h_KinEs[i_particle][i_direction][i_setting]->Draw("HIST E SAMES");
      
	c_XYs[i_particle][i_direction]->cd(i_setting+1);
	h_XYs[i_particle][i_direction][i_setting]->Scale(1.0/n_gen_events);
	h_XYs[i_particle][i_direction][i_setting]->SetYTitle("Steps / bin / POT");
	h_XYs[i_particle][i_direction][i_setting]->Draw("COLZ");

	c_Zs[i_particle][i_direction]->cd();
	h_Zs[i_particle][i_direction][i_setting]->Scale(1.0/n_gen_events);
	h_Zs[i_particle][i_direction][i_setting]->SetYTitle("Steps / bin / POT");
	h_Zs[i_particle][i_direction][i_setting]->Draw("HIST E SAMES");

	c_times[i_particle][i_direction]->cd();
	h_times[i_particle][i_direction][i_setting]->Scale(1.0/n_gen_events);
	h_times[i_particle][i_direction][i_setting]->SetXTitle("time % 1695 [ns]");
	h_times[i_particle][i_direction][i_setting]->SetYTitle("Steps / bin / POT");
	h_times[i_particle][i_direction][i_setting]->Draw("HIST E SAMES");
      }
    }
  }

  TLegend* leg = new TLegend(0.25, 0.65, 0.60, 0.85);
  leg->SetTextSize(0.04);
  leg->SetLineColor(kWhite);

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string setting = settings[i_setting];
    //    std::string leglabel = setting + " PS End Cap Geometry";
    std::string leglabel = leglabels[i_setting];
    leg->AddEntry(h_KinEs[0][0][i_setting], leglabel.c_str(), "l");
  }

  if (save_plots) {
    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      for (int i_direction = 0; i_direction < n_directions; ++i_direction) {
	std::string basename = basedir + std::string(c_KinEs[i_particle][i_direction]->GetName());
	std::string pngname = basename + ".png";
	c_KinEs[i_particle][i_direction]->cd();
	leg->Draw();
	c_KinEs[i_particle][i_direction]->SaveAs(pngname.c_str());
	
	if (i_direction != i_dwn_direction) {
	  h_KinEs[i_particle][i_direction][0]->GetXaxis()->SetRangeUser(0,200);
	  pngname = basename + "_ZoomIn.png";
	  c_KinEs[i_particle][i_direction]->cd();
	  c_KinEs[i_particle][i_direction]->SaveAs(pngname.c_str());
	}
	else {
	  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
	    double min_integral_bin = h_KinEs[i_particle][i_direction][i_setting]->GetXaxis()->FindBin(100);
	    double max_integral_bin = h_KinEs[i_particle][i_direction][i_setting]->GetNbinsX();
	    double integral = h_KinEs[i_particle][i_direction][i_setting]->Integral();//min_integral_bin, max_integral_bin);
	    double n_gen_events = normalizations[i_setting];
	    double error = std::sqrt(integral*n_gen_events) / n_gen_events;
	    TLatex* latex = new TLatex();
	    std::stringstream text;
	    if (i_setting == 0) {
	      text << "Integral:";// (>100 MeV/c):";
	      latex->DrawLatexNDC(0.55, 0.35, text.str().c_str());
	    }
	    text.str("");
	    text << "(" << std::fixed << std::setprecision(3) << integral/1e-3 << " #pm " << error/1e-3 << ") #times 10^{-3} / POT";
	    latex->SetTextColor(colours[i_setting]);
	    latex->DrawLatexNDC(0.55, 0.3 - i_setting*0.05, text.str().c_str());
	  }
	    
	  pngname = basename + "_Annotated.png";
	  c_KinEs[i_particle][i_direction]->cd();
	  c_KinEs[i_particle][i_direction]->SaveAs(pngname.c_str());
	}

	basename = basedir + std::string(c_XYs[i_particle][i_direction]->GetName());
	pngname = basename + ".png";
	c_XYs[i_particle][i_direction]->SaveAs(pngname.c_str());

	basename = basedir + std::string(c_Zs[i_particle][i_direction]->GetName());
	pngname = basename + ".png";
	c_Zs[i_particle][i_direction]->cd();
	leg->Draw();
	c_Zs[i_particle][i_direction]->SaveAs(pngname.c_str());

	basename = basedir + std::string(c_times[i_particle][i_direction]->GetName());
	pngname = basename + ".png";
	c_times[i_particle][i_direction]->cd();
	leg->Draw();
	c_times[i_particle][i_direction]->SaveAs(pngname.c_str());
      }
    }
  }
}
