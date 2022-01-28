generate_fcl --description vd-steps-selector --dsconf PSEndCap-Old --dsowner edmonds --embed fcl/vd-steps-selector.fcl --inputs sim.edmonds.Beam.PSEndCap-Old.lst --merge-factor 100 #--first-subrun 500 --old-seeds seeds.edmonds.vd-steps-selector.baseline.xPYn.txt
rm -rf grid_fcl/vd-steps-selector-PSEndCap-Old
mv 000 grid_fcl/vd-steps-selector-PSEndCap-Old
