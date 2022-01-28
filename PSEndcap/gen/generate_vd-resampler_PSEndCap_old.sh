generate_fcl --description vd-resampler --dsconf PSEndCap-Old --dsowner edmonds --embed fcl/vd-resampler_PSEndCap_old.fcl --run-number 2 --events-per-job 200000 --njobs 50 --auxinput=1:physics.filters.beamResampler.fileNames:sim.edmonds.vd-steps-selector.PSEndCap-Old.lst #--first-subrun 500 --old-seeds seeds.edmonds.vd-resampler.baseline.xPYn.txt
rm -rf grid_fcl/vd-resampler-PSEndCap-Old
mv 000 grid_fcl/vd-resampler-PSEndCap-Old
