generate_fcl --description POT --dsconf PSEndCap-Old --dsowner edmonds --embed fcl/POT_PSEndCap_old.fcl --run-number 1 --events-per-job 10000 --njobs 500 #--first-subrun 500 --old-seeds seeds.edmonds.POT.baseline.xPYn.txt
rm -rf grid_fcl/POT-PSEndCap-Old
mv 000 grid_fcl/POT-PSEndCap-Old
