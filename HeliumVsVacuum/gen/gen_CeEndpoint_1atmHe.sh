generate_fcl --dsconf=1atmHe --dsowner=edmonds --run-number=1210 --description=CeEndpoint --events-per-job=10000 --njobs=1 --include Mu2eStudies/HeliumVsVacuum/fcl/CeEndpoint_1atmHe.fcl --auxinput=1:physics.filters.TargetStopResampler.fileNames:filelists/sim.edmonds.MuminusStops.1atmHe.lst
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/CeEndpoint_1atmHe_$dirname
  mv $dirname grid_fcl/CeEndpoint_1atmHe_$dirname
 fi
done
