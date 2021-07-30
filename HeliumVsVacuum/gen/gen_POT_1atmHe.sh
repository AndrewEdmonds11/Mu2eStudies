generate_fcl --dsconf=1atmHe --dsowner=edmonds --run-number=1 --events-per-job=10000 --njobs=50 --include Mu2eStudies/HeliumVsVacuum/fcl/POT_1atmHe.fcl --description=POT
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/POT_1atmHe_$dirname
  mv $dirname grid_fcl/POT_1atmHe_$dirname
  mv seeds*.txt grid_fcl/
 fi
done
