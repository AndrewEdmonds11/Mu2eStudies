generate_fcl --description vd-steps-selector --dsconf PSEndCap-New --dsowner edmonds --embed fcl/vd-steps-selector.fcl --inputs sim.edmonds.Beam.PSEndCap-New.lst --merge-factor 100 #--first-subrun 500 --new-seeds seeds.edmonds.vd-steps-selector.baseline.xPYn.txt
rm -rf grid_fcl/vd-steps-selector-PSEndCap-New
mv 000 grid_fcl/vd-steps-selector-PSEndCap-New
