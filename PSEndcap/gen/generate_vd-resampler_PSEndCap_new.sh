generate_fcl --description vd-resampler --dsconf PSEndCap-New --dsowner edmonds --embed fcl/vd-resampler_PSEndCap_new.fcl --run-number 2 --events-per-job 200000 --njobs 50 --auxinput=1:physics.filters.beamResampler.fileNames:sim.edmonds.vd-steps-selector.PSEndCap-New.lst #--first-subrun 500 --new-seeds seeds.edmonds.vd-resampler.baseline.xPYn.txt
rm -rf grid_fcl/vd-resampler-PSEndCap-New
mv 000 grid_fcl/vd-resampler-PSEndCap-New
