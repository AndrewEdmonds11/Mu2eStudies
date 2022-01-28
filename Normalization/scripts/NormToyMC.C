void Norm() {

  int n_ubunches = 10;
  double nominalPBI = 3.9e7;
  TF1* POT = new TF1("POT", "TMath::LogNormal(x, [0])", 0,10*nominalPBI);
  double SDF = 0.5;
  POT->SetParameter(0, std::sqrt(-std::log(SDF)));
  POT->Draw();
  double n_POT_per_ubunch = 3.9e7;
  double n_mustop_per_POT = 0.0015;

  for (int i_ubunch = 0; i_ubunch < n_ubunches; ++i_ubunch) {
    double n_mustop = n_mustop_per_POT * n_POT_per_ubunch;
    std::cout << "n_muStop = " << n_mustop << std::endl;
  }

}
