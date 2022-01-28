void ToyMC() {

  int n_generated_events = 1;

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
}
