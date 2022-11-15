setup dhtools
setup mu2etools

njobs=10

source $MUSE_WORK_DIR/backing/Production/Scripts/gen_MuStopPileup.sh MDC2020 p u ${njobs}

for file in `ls TargetStopResampler_000/*.fcl`; do

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

services.TFileService.fileName : \"nts.owner.gen-plots-murat.sequencer.root\"
" >> ${file}

done

mv TargetStopResampler_000/ TargetStopResampler_murat_000/
