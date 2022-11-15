#include "TH1.h"
#include "TCanvas.h"
#include "TChain.h"
#include "Math/Vector3D.h"
#include "TLegend.h"

#include <iostream>
#include <sstream>

#include "VDHit.h"

void GeneratedMuStopPileUp_Comparison(bool save_plots = false) {

  std::string basedir = "/mu2e/app/users/edmonds/plots/2022-11-15";

  double u = 931.49410242;
  const int n_particles = 2;
  std::string particles[n_particles] = {"proton", "deuteron"};//, "triton", "alpha"};
  double masses[n_particles] = {1.007276466621*u, 2.0141017781*u};//, 3.01604928*u, 4.001506179127*u};
  int pdgIds[n_particles] = {2212, 1000010020};//, 1000010030, 1000020040};
  TCanvas* c_gen_energies[n_particles];
  TCanvas* c_acc_energies[n_particles];
  TCanvas* c_trkent_energies[n_particles];

  TLegend* legs[n_particles];


  const int n_settings = 3;
  std::string jobids[n_settings] = {"40527330", "40533059", "40533108"};
  std::string tags[n_settings] = {"baseline", "hungerford", "murat"};
  int n_jobs[n_settings] = {10, 10, 10};
  Int_t colours[n_settings] = {kBlack, kRed, kBlue};

  TH1D* hGenEs[n_settings][n_particles];
  TH1D* hAccEs[n_settings][n_particles];
  TH1D* hTrkEntEs[n_settings][n_particles];

  for (int i_setting = 0; i_setting < n_settings; ++i_setting) {
    std::string tag = tags[i_setting];
    std::string jobid = jobids[i_setting];
    int n_job = n_jobs[i_setting];

    TChain* generated = new TChain("GeneratorPlots/GenAna");
    TChain* accepted = new TChain("AcceptedPlots/GenAna");
    TChain* vdsteps = new TChain("VDSteps/nt");

    std::stringstream filename;

    filename.str("");
    filename << "vd-steps-mu-stop-pileup." << tag << ".root";
    vdsteps->Add(filename.str().c_str());
    for (int i_job = 0; i_job < n_job; ++i_job) {
      filename.str("");
      filename << "/pnfs/mu2e/scratch/users/edmonds/workflow/PDTASpectra/failed/logcheck/" << jobid << "/00/" << std::setw(5) << std::setfill('0') << i_job << "/nts.owner.gen-plots-" << tag << ".sequencer.root";
      std::cout << filename.str() << std::endl;
      generated->Add(filename.str().c_str());
      accepted->Add(filename.str().c_str());
      //      TTree* generated = (TTree*) file->Get("GeneratorPlots/GenAna");
      //TTree* accepted = (TTree*) file->Get("AcceptedPlots/GenAna");

      //      filename = "vd-steps-mu-stop-pileup" + tag + ".root";
      //      TFile* vd_file = new TFile(filename.c_str(), "READ");
      //      TTree* vdsteps = (TTree*) vd_file->Get("VDSteps/nt");
    }
    std::cout << "generated n entries = " << generated->GetEntries() << std::endl;
    std::cout << "accepted n entries = " << accepted->GetEntries() << std::endl;
    std::cout << "vdsteps n entries = " << vdsteps->GetEntries() << std::endl;

    // Create histograms for this setting
    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      std::string particle = particles[i_particle];
      if (i_setting == 0) {
        std::string canvasname = "c_gen_energy_" + particle;
        c_gen_energies[i_particle] = new TCanvas(canvasname.c_str(), canvasname.c_str());

        canvasname = "c_acc_energy_" + particle;
        c_acc_energies[i_particle] = new TCanvas(canvasname.c_str(), canvasname.c_str());

        canvasname = "c_trkent_energy_" + particle;
        c_trkent_energies[i_particle] = new TCanvas(canvasname.c_str(), canvasname.c_str());

        legs[i_particle] = new TLegend(0.6, 0.6, 0.8, 0.8);
        legs[i_particle]->SetTextSize(0.04);
        legs[i_particle]->SetLineColor(kWhite);
      }

      double min_energy = 0;
      double max_energy = 100;
      double energy_width = 0.5;
      int n_energy_bins = (max_energy - min_energy) / energy_width;
      std::string histname ="hGenE_" + particle + "_" + tag;
      TH1D* hGenE = new TH1D(histname.c_str(), particles[i_particle].c_str(), n_energy_bins,min_energy,max_energy);
      hGenEs[i_setting][i_particle] = hGenE;

      energy_width = 2.0;
      n_energy_bins = (max_energy - min_energy) / energy_width;
      histname ="hAccE_" + particle + "_" + tag;
      TH1D* hAccE = new TH1D(histname.c_str(), particles[i_particle].c_str(), n_energy_bins,min_energy,max_energy);
      hAccEs[i_setting][i_particle] = hAccE;

      histname ="hTrkEntE_" + particle + "_" + tag;
      TH1D* hTrkEntE = new TH1D(histname.c_str(), particles[i_particle].c_str(), n_energy_bins,min_energy,max_energy);
      hTrkEntEs[i_setting][i_particle] = hTrkEntE;
    }

    // Now fill histograms while looping through each chain once
    ROOT::Math::XYZVectorF* genmom = new ROOT::Math::XYZVectorF;
    generated->SetBranchAddress("genmom", &genmom);
    int genCrCode;
    generated->SetBranchAddress("genCrCode", &genCrCode);
    int genId;
    generated->SetBranchAddress("genId", &genId);
    int n_entries = generated->GetEntries();
    for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
      generated->GetEntry(i_entry);
      if (i_entry % 1000000 == 0) {
        std::cout << i_entry << " / " << n_entries << std::endl;
      }

      if (genCrCode == 165) {
        for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
          if (genId == pdgIds[i_particle]) {
            std::string particle = particles[i_particle];
            double mass = masses[i_particle];
            double ek = sqrt(genmom->mag2() + mass*mass) - mass;
            hGenEs[i_setting][i_particle]->Fill(ek);
          }
        }
      }
    }

    accepted->SetBranchAddress("genmom", &genmom);
    accepted->SetBranchAddress("genCrCode", &genCrCode);
    accepted->SetBranchAddress("genId", &genId);
    n_entries = accepted->GetEntries();
    for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
      accepted->GetEntry(i_entry);
      if (i_entry % 1000000 == 0) {
        std::cout << i_entry << " / " << n_entries << std::endl;
      }

      if (genCrCode == 165) {
        for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
          if (genId == pdgIds[i_particle]) {
            std::string particle = particles[i_particle];
            double mass = masses[i_particle];
            double ek = sqrt(genmom->mag2() + mass*mass) - mass;
            hAccEs[i_setting][i_particle]->Fill(ek);
          }
        }
      }
    }

    VDHit vdhit;
    vdsteps->SetBranchAddress("hits", &vdhit);

    n_entries = vdsteps->GetEntries();
    for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
      vdsteps->GetEntry(i_entry);
      if (i_entry % 1000000 == 0) {
        std::cout << i_entry << " / " << n_entries << std::endl;
      }
      if (vdhit.volumeCopy == 13) {
        for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
          if (vdhit.pdgId == pdgIds[i_particle]) {
            hTrkEntEs[i_setting][i_particle]->Fill(vdhit.ek);
          }
        }
      }
    }

    // Now plot
    for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
      std::string particle = particles[i_particle];
      auto hGenE = hGenEs[i_setting][i_particle];
      auto hAccE = hAccEs[i_setting][i_particle];
      auto hTrkEntE = hTrkEntEs[i_setting][i_particle];
      //      hGenE->SetStats(false);

      c_gen_energies[i_particle]->cd();
      hGenE->SetXTitle("Generated Energy [MeV]");
      std::stringstream axistitle;
      axistitle.str("");
      axistitle << "Count / " << hGenE->GetBinWidth(1) << " MeV";
      hGenE->SetYTitle(axistitle.str().c_str());
      hGenE->SetStats(false);
      hGenE->SetLineWidth(2);
      hGenE->SetLineColor(colours[i_setting]);
      hGenE->Draw("HIST E SAME");
      legs[i_particle]->AddEntry(hGenE, tag.c_str(), "l");

      c_acc_energies[i_particle]->cd();
      axistitle.str("");
      axistitle << "Count / " << hAccE->GetBinWidth(1) << " MeV";
      hAccE->SetYTitle(axistitle.str().c_str());
      hAccE->SetStats(false);
      hAccE->SetLineWidth(2);
      hAccE->SetLineColor(colours[i_setting]);
      hAccE->SetXTitle("Generated Energy [MeV]");
      hAccE->Draw("HIST E SAME");

      c_trkent_energies[i_particle]->cd();
      axistitle.str("");
      axistitle << "Count / " << hTrkEntE->GetBinWidth(1) << " MeV";
      hTrkEntE->SetYTitle(axistitle.str().c_str());
      hTrkEntE->SetStats(false);
      hTrkEntE->SetLineWidth(2);
      hTrkEntE->SetLineColor(colours[i_setting]);
      hTrkEntE->SetXTitle("Energy at Tracker Entrance [MeV]");
      hTrkEntE->Draw("HIST E SAME");

    }
  }

    //      leg->AddEntry(hGenE, ("generated (" + std::to_string(int(hGenE->GetEntries())) + " entries)").c_str(), "l");

    //      leg->Draw();

  for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
    std::string particle = particles[i_particle];

    c_gen_energies[i_particle]->cd();
    legs[i_particle]->Draw();

    c_acc_energies[i_particle]->cd();
    legs[i_particle]->Draw();

    c_trkent_energies[i_particle]->cd();
    legs[i_particle]->Draw();

    if (save_plots) {
      std::string pdfname = basedir + "/Mu2eSim_MuStopPileup_" + particle + "_GenE_ModelComparison.pdf";
      c_gen_energies[i_particle]->SaveAs(pdfname.c_str());

      pdfname = basedir + "/Mu2eSim_MuStopPileup_" + particle + "_AccE_ModelComparison.pdf";
      c_acc_energies[i_particle]->SaveAs(pdfname.c_str());

      pdfname = basedir + "/Mu2eSim_MuStopPileup_" + particle + "_TrkEntE_ModelComparison.pdf";
      c_trkent_energies[i_particle]->SaveAs(pdfname.c_str());
    }
  }
}
