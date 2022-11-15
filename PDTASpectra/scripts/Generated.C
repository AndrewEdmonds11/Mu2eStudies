void Generated(bool save_plots = false) {

  std::string basedir = "/mu2e/app/users/edmonds/plots/2022-11-13";

  double u = 931.49410242;
  const int n_particles = 4;
  std::string particles[n_particles] = {"proton", "deuteron", "triton", "alpha"};
  double masses[n_particles] = {1.007276466621*u, 2.0141017781*u, 3.01604928*u, 4.001506179127*u};
  int pdgIds[n_particles] = {2212, 1000010020, 1000010030, 1000020040};
  double max_moms[n_particles] = {400, 400, 400, 400};

  for (int i_particle = 0; i_particle < n_particles; ++i_particle) {
    std::string particle = particles[i_particle];
    std::string filename = "gen-plots-" + particle + ".root";
    TFile* file = new TFile(filename.c_str(), "READ");
    TTree* generated = (TTree*) file->Get("GeneratorPlots/GenAna");
    TTree* accepted = (TTree*) file->Get("AcceptedPlots/GenAna");

    std::stringstream drawcmd,cutcmd;

    TLegend* leg = new TLegend(0.4, 0.4, 0.6, 0.6);
    leg->SetTextSize(0.04);
    leg->SetLineColor(kWhite);

    double min_mom = 0;
    double max_mom = max_moms[i_particle];
    double mom_width = 10;
    int n_mom_bins = (max_mom - min_mom) / mom_width;
    std::string histname ="hGenMom";
    TH1F* hGenMom = new TH1F(histname.c_str(), particles[i_particle].c_str(), n_mom_bins,min_mom,max_mom);
    drawcmd.str("");
    drawcmd << "sqrt(genmom.mag2())>>" << histname;
    cutcmd.str("");
    cutcmd << "genCrCode==174";
    generated->Draw(drawcmd.str().c_str(), cutcmd.str().c_str(), "goff");
    hGenMom->SetStats(false);


    histname = "hAccMom";
    TH1F* hAccMom = new TH1F(histname.c_str(), "Initial Momentum", n_mom_bins,min_mom,max_mom);
    drawcmd.str("");
    drawcmd << "sqrt(genmom.mag2())>>" << histname;
    accepted->Draw(drawcmd.str().c_str(), cutcmd.str().c_str(), "goff");

    std::string canvasname = "c_mom_" + particle;
    TCanvas* c_mom = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hGenMom->SetMinimum(0);
    hGenMom->SetXTitle("Generated Momentum [MeV/c]");
    std::stringstream axistitle;
    axistitle.str("");
    axistitle << "Count / " << hGenMom->GetBinWidth(1) << " MeV/c";
    hGenMom->SetYTitle(axistitle.str().c_str());
    hAccMom->SetLineColor(kRed);
    hGenMom->SetLineWidth(2);
    hAccMom->SetLineWidth(2);
    hGenMom->Draw("HIST E");
    hAccMom->Draw("HIST E SAME");
    leg->AddEntry(hGenMom, ("generated (" + std::to_string(int(hGenMom->GetEntries())) + " entries)").c_str(), "l");
    leg->AddEntry(hAccMom, ("enters a detector volume (" + std::to_string(int(hAccMom->GetEntries())) + " entries)").c_str(), "l");

    leg->Draw();

    TEfficiency* hEffMom = new TEfficiency(*hAccMom, *hGenMom);
    canvasname = "c_effmom_" + particle;
    TCanvas* c_effmom = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hEffMom->Draw("A*E");
    axistitle.str("");
    axistitle << "Efficiency / " << hGenMom->GetBinWidth(1) << " MeV";
    gPad->Update();
    hEffMom->GetPaintedGraph()->GetYaxis()->SetTitle(axistitle.str().c_str());


    double mass = masses[i_particle];
    double min_energy = 0;
    double max_energy = std::ceil(sqrt(max_mom*max_mom + mass*mass) - mass);
    double energy_width = 1;
    int n_energy_bins = (max_energy - min_energy) / energy_width;
    histname ="hGenE";
    TH1F* hGenE = new TH1F(histname.c_str(), particles[i_particle].c_str(), n_energy_bins,min_energy,max_energy);
    drawcmd.str("");
    drawcmd << "sqrt(genmom.mag2() + " << mass << "^2) - " << mass << ">>" << histname;
    cutcmd.str("");
    cutcmd << "genCrCode==174";
    generated->Draw(drawcmd.str().c_str(), cutcmd.str().c_str(), "goff");
    hGenE->SetStats(false);

    histname = "hAccE";
    TH1F* hAccE = new TH1F(histname.c_str(), "Initial Energy", n_energy_bins,min_energy,max_energy);
    drawcmd.str("");
    drawcmd << "sqrt(genmom.mag2() + " << mass << "^2) - " << mass << ">>" << histname;
    accepted->Draw(drawcmd.str().c_str(), cutcmd.str().c_str(), "goff");

    canvasname = "c_energy_" + particle;
    TCanvas* c_energy = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hGenE->SetMinimum(0);
    hGenE->SetXTitle("Generated Energy [MeV]");
    axistitle.str("");
    axistitle << "Count / " << hGenE->GetBinWidth(1) << " MeV";
    hGenE->SetYTitle(axistitle.str().c_str());
    hAccE->SetLineColor(kRed);
    hGenE->SetLineWidth(2);
    hAccE->SetLineWidth(2);
    hGenE->Draw("HIST E");
    hAccE->Draw("HIST E SAME");
    leg->Draw();

    TEfficiency* hEffE = new TEfficiency(*hAccE, *hGenE);
    canvasname = "c_effenergy_" + particle;
    TCanvas* c_effenergy = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hEffE->Draw("A*E");
    axistitle.str("");
    axistitle << "Efficiency / " << hGenE->GetBinWidth(1) << " MeV";
    gPad->Update();
    hEffE->GetPaintedGraph()->GetYaxis()->SetTitle(axistitle.str().c_str());

    filename = "vd-steps-" + particle + ".root";
    TFile* vd_file = new TFile(filename.c_str(), "READ");
    TTree* vdsteps = (TTree*) vd_file->Get("VDSteps/nt");
    c_energy->cd();
    cutcmd.str("");
    cutcmd << "pdgId==" << pdgIds[i_particle] << " && volumeCopy==13";

    c_mom->cd();
    //    vdsteps->Draw("pmag", cutcmd.str().c_str(), "HIST E SAME");

    TLegend* leg2 = new TLegend(0.4, 0.65, 0.6, 0.85);
    leg2->SetTextSize(0.04);
    leg2->SetLineColor(kWhite);

    canvasname = "c_beforeafter_energy_" + particle;
    TCanvas* c_beforeafter_energy = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hAccE->SetXTitle("Energy [MeV]");
    hAccE->SetTitle(particle.c_str());
    axistitle.str("");
    axistitle << "Count / " << hAccE->GetBinWidth(1) << " MeV";
    hAccE->SetYTitle(axistitle.str().c_str());
    hAccE->SetStats(false);

    TH1F* hTrkEntE = (TH1F*) hAccE->Clone("hTrkEntE");
    hTrkEntE->SetLineWidth(2);
    hTrkEntE->SetLineColor(kMagenta);
    hTrkEntE->SetXTitle("Energy [MeV]");
    hTrkEntE->SetTitle(particle.c_str());
    axistitle.str("");
    axistitle << "Count / " << hTrkEntE->GetBinWidth(1) << " MeV";
    hTrkEntE->SetYTitle(axistitle.str().c_str());
    hTrkEntE->SetStats(false);

    vdsteps->Draw("ek>>hTrkEntE", cutcmd.str().c_str(), "goff");
    if (hTrkEntE->GetMaximum() < hAccE->GetMaximum()) {
      hAccE->Draw("HIST E");
      hTrkEntE->Draw("HIST E SAME");
    }
    else {
      hTrkEntE->Draw("HIST E");
      hAccE->Draw("HIST E SAME");
    }

    leg2->AddEntry(hAccE, ("generated energy (" + std::to_string(int(hAccE->GetEntries())) + " entries)").c_str(), "l");
    leg2->AddEntry(hTrkEntE, ("energy at tracker entrance (" + std::to_string(int(hTrkEntE->GetEntries())) + " entries)").c_str(), "l");
    leg2->Draw();

    canvasname = "c_beforeafter_mom_" + particle;
    TCanvas* c_beforeafter_mom = new TCanvas(canvasname.c_str(), canvasname.c_str());
    hAccMom->SetXTitle("Momentum [MeV/c]");
    hAccMom->SetTitle(particle.c_str());
    axistitle.str("");
    axistitle << "Count / " << hAccMom->GetBinWidth(1) << " MeV/c";
    hAccMom->SetYTitle(axistitle.str().c_str());
    hAccMom->SetStats(false);

    TH1F* hTrkEntMom = (TH1F*) hAccMom->Clone("hTrkEntMom");
    hTrkEntMom->SetLineWidth(2);
    hTrkEntMom->SetLineColor(kMagenta);
    hTrkEntMom->SetXTitle("Momentum [MeV/c]");
    hTrkEntMom->SetTitle(particle.c_str());
    axistitle.str("");
    axistitle << "Count / " << hTrkEntMom->GetBinWidth(1) << " MeV/c";
    hTrkEntMom->SetYTitle(axistitle.str().c_str());
    hTrkEntMom->SetStats(false);

    vdsteps->Draw("pmag>>hTrkEntMom", cutcmd.str().c_str(), "goff");
    if (hTrkEntMom->GetMaximum() < hAccMom->GetMaximum()) {
      hAccMom->Draw("HIST E");
      hTrkEntMom->Draw("HIST E SAME");
    }
    else {
      hTrkEntMom->Draw("HIST E");
      hAccMom->Draw("HIST E SAME");
    }
    leg2->Draw();

    if (save_plots) {
      std::string pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_Mom.pdf";
      c_mom->SaveAs(pdfname.c_str());
      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_EffMom.pdf";
      c_effmom->SaveAs(pdfname.c_str());

      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_Energy.pdf";
      c_energy->SaveAs(pdfname.c_str());
      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_EffEnergy.pdf";
      c_effenergy->SaveAs(pdfname.c_str());

      c_effenergy->SetLogy();
      hGenE->GetYaxis()->SetRangeUser(1e-4, 2e-1);
      hGenE->GetXaxis()->SetRangeUser(0,10);
      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_EffEnergy_ZoomIn.pdf";
      c_effenergy->SaveAs(pdfname.c_str());

      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_BeforeAfterEnergy.pdf";
      c_beforeafter_energy->SaveAs(pdfname.c_str());

      pdfname = basedir + "/Mu2eSim_ChargedParticles_" + particle + "_BeforeAfterMom.pdf";
      c_beforeafter_mom->SaveAs(pdfname.c_str());

    }
  }
}
