#include "TrkAna/inc/EventInfo.hh"

void Norm() {

  std::string filename = "nts.owner.trkana-reco.version.sequencer.root";
  TFile* file = new TFile(filename.c_str(), "READ");

  double n_generated_events = ((TH1F*) file->Get("genCountLogger/numEvents"))->GetBinContent(1);
  double n_analyzed_events = ((TH1F*) file->Get("muStopLogger/numSeenEvents"))->GetBinContent(1);
  double n_mu_stops = ((TH1F*) file->Get("muStopLogger/numTrueMuStops"))->GetBinContent(1);
  n_mu_stops *= 1000; // because of prescaling
  // Add muon stops for events we didn't see
  double SDF = 0.6;
  double sigma = sqrt(-log(SDF));
  double extendedMean = 1.58e7;
  double cutMin = 0;
  double cutMax = 9.48e7;
  const double sigma2 = std::pow(sigma,2);
  // log-normal mu that gives distribution with the requested mean (before the cuts!)
  double mu0 = std::log(extendedMean) - 0.5*sigma2;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::lognormal_distribution<double> lognd_ = std::lognormal_distribution<double>(mu0,sigma);
  for (int i_extra_event = n_analyzed_events; i_extra_event <= n_generated_events; ++i_extra_event) {
    double res=cutMax; // force to enter the loop
    while( (res < cutMin) || (cutMax <= res) ) {
      res = lognd_(gen);
    }
    n_mu_stops += res*1.53217e-06*1000;
  }
  std::cout << "No. of generated events = " << n_generated_events << std::endl;
  std::cout << "No. of \"triggered\" events = " << n_analyzed_events << std::endl;
  std::cout << "No. of \"true\" muon stops = " << n_mu_stops << std::endl;

  double true_ce_rate = n_analyzed_events / n_mu_stops;
  std::cout << "\"True\" CE Rate = " << n_analyzed_events << " / " << n_mu_stops << " = " << true_ce_rate << " per stopped muon" << std::endl;

  std::string treename = "TrkAnaNeg/trkana";
  TTree* trkana = (TTree*) file->Get(treename.c_str());

  double min_mom = 80;
  double max_mom = 110;
  double mom_bin_width = 0.25;
  int n_mom_bins = (max_mom - min_mom) / mom_bin_width;
  TH1F* hMom = new TH1F("hMom", "", n_mom_bins,min_mom,max_mom);
  trkana->Draw("deent.mom>>hMom", "", "goff");
  hMom->SetLineColor(kBlack);
  hMom->SetLineWidth(2);
  hMom->SetMarkerColor(kBlack);
  hMom->SetMarkerStyle(kFullCircle);
  hMom->Draw("PE");
  double n_measured_ces = hMom->Integral();
  std::cout << "No. of \"measured\" Ces = " << n_measured_ces << std::endl;
  double measured_ce_rate = n_measured_ces / n_mu_stops;
  std::cout << "Measured Ce rate = " << n_measured_ces << " / " << n_mu_stops << " = " << measured_ce_rate << " per stopped muon" << std::endl;

  TH1F* hMomCut = new TH1F("hMomCut", "", n_mom_bins,min_mom,max_mom);
  trkana->Draw("deent.mom>>hMomCut", "(bestcrv<0||(de.t0-crvinfo._timeWindowStart[bestcrv]<-50||de.t0-crvinfo._timeWindowStart[bestcrv]>150.0))", "goff");
  hMomCut->SetLineColor(kRed);
  hMomCut->SetLineWidth(2);
  hMomCut->SetMarkerColor(kRed);
  hMomCut->SetMarkerStyle(kFullCircle);
  hMomCut->Draw("PE SAME");
  double n_cut_ces = hMomCut->Integral();
  std::cout << "No. of \"cut\" Ces = " << n_cut_ces << std::endl;
  double cut_ce_rate = n_cut_ces / n_mu_stops;
  std::cout << "Cut Ce rate = " << n_cut_ces << " / " << n_mu_stops << " = " << cut_ce_rate << " per stopped muon" << std::endl;

  // double n_tracks = trkana->GetEntries();
  // double n_gen_tracks = n_tracks;
  // double n_total_POT = 0;

  // mu2e::EventInfo evtinfo;
  // trkana->SetBranchAddress("evtinfo.", &evtinfo);
  // for (int i_track = 0; i_track < n_tracks; ++i_track) {
  //   trkana->GetEntry(i_track);
  //   n_total_POT += evtinfo._nprotons;
  // }
  // std::cout << "N Gen Tracks = " << n_gen_tracks << std::endl;
  // std::cout << "N Measured Tracks = " << n_tracks << std::endl;
  // std::cout << "True POT = " << n_total_POT << std::endl;
  // double n_total_stop_mu = 0.0015*n_total_POT;
  // std::cout << "True N Stop Mu = " << n_total_stop_mu << std::endl;
  // double n_total_cap_mu = 0.609*n_total_stop_mu;
  // std::cout << "True N Cap Mu = " << n_total_cap_mu << std::endl;
  // double true_ce_rate = n_gen_tracks / n_total_cap_mu;
  // std::cout << "\"True\" Ce rate = " << true_ce_rate << " per captured muon" << std::endl;

  // int n_measured_tracks = trkana->GetEntries("(bestcrv<0||(de.t0-crvinfo._timeWindowStart[bestcrv]<-50||de.t0-crvinfo._timeWindowStart[bestcrv]>150.0))");
  // double measured_ce_rate = n_measured_tracks / n_total_cap_mu;
  // std::cout << "\"Measured\" Ce rate = " << measured_ce_rate << " per captured muon" << std::endl;
}
