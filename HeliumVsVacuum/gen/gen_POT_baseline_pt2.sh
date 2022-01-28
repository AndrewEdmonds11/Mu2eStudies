generate_fcl --dsconf=baseline --dsowner=edmonds --run-number=1 --events-per-job=20000 --njobs=9 --first-subrun=1 --old-seeds=grid_fcl/seeds.edmonds.POT.baseline.uqE1.txt --include Mu2eStudies/HeliumVsVacuum/fcl/POT_baseline.fcl --description=POT
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/POT_baseline_pt2_$dirname
  mv $dirname grid_fcl/POT_baseline_pt2_$dirname
  mv seeds*.txt grid_fcl/
 fi
done
