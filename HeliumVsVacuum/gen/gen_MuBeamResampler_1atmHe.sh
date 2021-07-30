generate_fcl --dsconf=1atmHe --dsowner=edmonds --run-number=1 --description=MuBeamResampler --events-per-job=200000 --njobs=1 --include Mu2eStudies/HeliumVsVacuum/fcl/MuBeamResampler_1atmHe.fcl --auxinput=1:physics.filters.beamResampler.fileNames:filelists/sim.edmonds.MuBeamCat.1atmHe.lst
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/MuBeamResampler_1atmHe_$dirname
  mv $dirname grid_fcl/MuBeamResampler_1atmHe_$dirname
 fi
done
