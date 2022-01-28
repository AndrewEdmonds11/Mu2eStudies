generate_fcl --description POT --dsconf PSEndCap-NewThickAl-wWindowSDs --dsowner edmonds --embed fcl/POT_PSEndCap_newThickAl_wWindowSDs.fcl --run-number 1 --events-per-job 10000 --njobs 500 #--first-subrun 500 --new-seeds seeds.edmonds.POT.baseline.xPYn.txt
rm -rf grid_fcl/POT-PSEndCap-NewThickAl-wWindowSDs
mv 000 grid_fcl/POT-PSEndCap-NewThickAl-wWindowSDs
