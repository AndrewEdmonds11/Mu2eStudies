setup dhtools
setup mu2etools

njobs=10

source $MUSE_WORK_DIR/backing/Production/Scripts/gen_MuStopPileup.sh MDC2020 p u ${njobs}
sed -i 's/root:\/\/fndca1.fnal.gov:1094\/pnfs\/fnal.gov\/usr\//\/pnfs\//' TargetStopResampler_000/cnf.mu2e.TargetStopResampler.MDC2020u.001202_00000000.fcl

echo "physics.analyzers.GeneratorPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : g4run
}

physics.analyzers.AcceptedPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : compressDetStepMCs
  SelectEvents : [ \"PileupPath\" ]
}

physics.EndPath : [ @sequence::physics.EndPath, GeneratorPlots, AcceptedPlots ]

physics.filters.DetStepFilter.MinimumPartMom : 0
physics.filters.DetStepFilter.MinimumTrkStepEnergy : 0
physics.filters.DetStepFilter.MinimumCrvStepEnergy : 0
physics.filters.DetStepFilter.MinimumTrkSteps : 0
physics.filters.DetStepFilter.MinimumSumCaloStepE : 0

# turn on VD hits for this
physics.producers.compressDetStepMCs.stepPointMCTags : [ \"g4run:virtualdetector\" ]
outputs.Output.outputCommands : [ @sequence::outputs.Output.outputCommands, \"keep mu2e::StepPointMCs_compressDetStepMCs_virtualdetector_*\"]

# to change proton spectra
services.GlobalConstantsService.inputFile : \"Offline/GlobalConstantsService/data/globalConstants_02.txt\"
physics.producers.generate.captureProducts[0].spectrum.spectrumShape : \"Murat\"
physics.producers.generate.captureProducts[0].spectrum.pdgId : 2212
physics.producers.generate.captureProducts[1].spectrum.spectrumShape : \"Murat\"
physics.producers.generate.captureProducts[1].spectrum.pdgId : 1000010020
" >> TargetStopResampler_000/cnf.mu2e.TargetStopResampler.MDC2020u.001202_00000000.fcl

mu2e -c TargetStopResampler_000/cnf.mu2e.TargetStopResampler.MDC2020u.001202_00000000.fcl --TFileName gen-plots-mu-stop-pileup.root -n 100000
mu2e -c vd-steps.fcl -s dts.MU2EGRIDDSOWNER.MuStopPileup.MU2EGRIDDSCONF.001202_00000000.art --TFileName vd-steps-mu-stop-pileup.root
