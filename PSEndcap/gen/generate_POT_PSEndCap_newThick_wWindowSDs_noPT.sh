generate_fcl --description POT --dsconf PSEndCap-NewThick-wWindowSDs_noPT --dsowner edmonds --embed fcl/POT_PSEndCap_newThick_wWindowSDs_noPT.fcl --run-number 1 --events-per-job 10000 --njobs 500 #--first-subrun 500 --new-seeds seeds.edmonds.POT.baseline.xPYn.txt
rm -rf grid_fcl/POT-PSEndCap-NewThick-wWindowSDs_noPT
mv 000 grid_fcl/POT-PSEndCap-NewThick-wWindowSDs_noPT
