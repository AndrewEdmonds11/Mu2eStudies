setup dhtools
setup mu2etools

njobs=2

# rm -rf FlatProton_000/
# source $MUSE_WORK_DIR/backing/Production/Scripts/gen_FlatMuDaughter.sh FlatePlus MDC2020 p u Muminus ${njobs} 100000 2212 0 400 "Offline/Mu2eG4/geom/bfgeom_no_tsu_ps_v01.txt"
# mv FlatePlus_000/ FlatProton_000/
# sed -i 's/root:\/\/fndca1.fnal.gov:1094\/pnfs\/fnal.gov\/usr\//\/pnfs\//' FlatProton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl

# echo "physics.analyzers.GeneratorPlots : {
#   module_type : GeneratorPlots
#   SimParticleCollection : g4run
# }

# physics.analyzers.AcceptedPlots : {
#   module_type : GeneratorPlots
#   SimParticleCollection : compressDetStepMCs
#   SelectEvents : [ \"PrimaryPath\" ]
# }

# physics.EndPath : [ @sequence::physics.EndPath, GeneratorPlots, AcceptedPlots ]

# physics.filters.PrimaryFilter.MinimumPartMom : 0
# physics.filters.PrimaryFilter.MinimumTrkStepEnergy : 0
# physics.filters.PrimaryFilter.MinimumCrvStepEnergy : 0
# physics.filters.PrimaryFilter.MinimumTrkSteps : 0
# physics.filters.PrimaryFilter.MinimumSumCaloStepE : 0
# " >> FlatProton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl

# mu2e -c FlatProton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl --TFileName gen-plots-proton.root
# mv dts.MU2EGRIDDSOWNER.FlatePlus.MU2EGRIDDSCONF.001202_00000000.art dts.MU2EGRIDDSOWNER.FlatProton.MU2EGRIDDSCONF.001202_00000000.art
# mu2e -c vd-steps.fcl -s dts.MU2EGRIDDSOWNER.FlatProton.MU2EGRIDDSCONF.001202_00000000.art --TFileName vd-steps-proton.root

## DEUTERONS
rm -rf FlatDeuteron_000/
source $MUSE_WORK_DIR/backing/Production/Scripts/gen_FlatMuDaughter.sh FlatePlus MDC2020 p u Muminus ${njobs} 100000 1000010020 0 400 "Offline/Mu2eG4/geom/bfgeom_no_tsu_ps_v01.txt"
mv FlatePlus_000/ FlatDeuteron_000/
sed -i 's/root:\/\/fndca1.fnal.gov:1094\/pnfs\/fnal.gov\/usr\//\/pnfs\//' FlatDeuteron_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl

echo "physics.analyzers.GeneratorPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : g4run
}

physics.analyzers.AcceptedPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : compressDetStepMCs
  SelectEvents : [ \"PrimaryPath\" ]
}

physics.EndPath : [ @sequence::physics.EndPath, GeneratorPlots, AcceptedPlots ]

physics.filters.PrimaryFilter.MinimumPartMom : 0
physics.filters.PrimaryFilter.MinimumTrkStepEnergy : 0
physics.filters.PrimaryFilter.MinimumCrvStepEnergy : 0
physics.filters.PrimaryFilter.MinimumTrkSteps : 0
physics.filters.PrimaryFilter.MinimumSumCaloStepE : 0
" >> FlatDeuteron_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl


mu2e -c FlatDeuteron_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl --TFileName gen-plots-deuteron.root
mv dts.MU2EGRIDDSOWNER.FlatePlus.MU2EGRIDDSCONF.001202_00000000.art dts.MU2EGRIDDSOWNER.FlatDeuteron.MU2EGRIDDSCONF.001202_00000000.art
mu2e -c vd-steps.fcl -s dts.MU2EGRIDDSOWNER.FlatDeuteron.MU2EGRIDDSCONF.001202_00000000.art --TFileName vd-steps-deuteron.root


## TRITONS
rm -rf FlatTriton_000/
source $MUSE_WORK_DIR/backing/Production/Scripts/gen_FlatMuDaughter.sh FlatePlus MDC2020 p u Muminus ${njobs} 100000 1000010030 0 400 "Offline/Mu2eG4/geom/bfgeom_no_tsu_ps_v01.txt"
mv FlatePlus_000/ FlatTriton_000/
sed -i 's/root:\/\/fndca1.fnal.gov:1094\/pnfs\/fnal.gov\/usr\//\/pnfs\//' FlatTriton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl

echo "physics.analyzers.GeneratorPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : g4run
}

physics.analyzers.AcceptedPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : compressDetStepMCs
  SelectEvents : [ \"PrimaryPath\" ]
}

physics.EndPath : [ @sequence::physics.EndPath, GeneratorPlots, AcceptedPlots ]

physics.filters.PrimaryFilter.MinimumPartMom : 0
physics.filters.PrimaryFilter.MinimumTrkStepEnergy : 0
physics.filters.PrimaryFilter.MinimumCrvStepEnergy : 0
physics.filters.PrimaryFilter.MinimumTrkSteps : 0
physics.filters.PrimaryFilter.MinimumSumCaloStepE : 0
" >> FlatTriton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl


mu2e -c FlatTriton_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl --TFileName gen-plots-triton.root
mv dts.MU2EGRIDDSOWNER.FlatePlus.MU2EGRIDDSCONF.001202_00000000.art dts.MU2EGRIDDSOWNER.FlatTriton.MU2EGRIDDSCONF.001202_00000000.art
mu2e -c vd-steps.fcl -s dts.MU2EGRIDDSOWNER.FlatTriton.MU2EGRIDDSCONF.001202_00000000.art --TFileName vd-steps-triton.root


## ALPHAS
rm -rf FlatAlpha_000/
source $MUSE_WORK_DIR/backing/Production/Scripts/gen_FlatMuDaughter.sh FlatePlus MDC2020 p u Muminus ${njobs} 100000 1000020040 0 400 "Offline/Mu2eG4/geom/bfgeom_no_tsu_ps_v01.txt"
mv FlatePlus_000/ FlatAlpha_000/
sed -i 's/root:\/\/fndca1.fnal.gov:1094\/pnfs\/fnal.gov\/usr\//\/pnfs\//' FlatAlpha_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl

echo "physics.analyzers.GeneratorPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : g4run
}

physics.analyzers.AcceptedPlots : {
  module_type : GeneratorPlots
  SimParticleCollection : compressDetStepMCs
  SelectEvents : [ \"PrimaryPath\" ]
}

physics.EndPath : [ @sequence::physics.EndPath, GeneratorPlots, AcceptedPlots ]

physics.filters.PrimaryFilter.MinimumPartMom : 0
physics.filters.PrimaryFilter.MinimumTrkStepEnergy : 0
physics.filters.PrimaryFilter.MinimumCrvStepEnergy : 0
physics.filters.PrimaryFilter.MinimumTrkSteps : 0
physics.filters.PrimaryFilter.MinimumSumCaloStepE : 0
" >> FlatAlpha_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl


mu2e -c FlatAlpha_000/cnf.mu2e.FlatePlus.MDC2020u.001202_00000000.fcl --TFileName gen-plots-alpha.root
mv dts.MU2EGRIDDSOWNER.FlatePlus.MU2EGRIDDSCONF.001202_00000000.art dts.MU2EGRIDDSOWNER.FlatAlpha.MU2EGRIDDSCONF.001202_00000000.art
mu2e -c vd-steps.fcl -s dts.MU2EGRIDDSOWNER.FlatAlpha.MU2EGRIDDSCONF.001202_00000000.art --TFileName vd-steps-alpha.root
