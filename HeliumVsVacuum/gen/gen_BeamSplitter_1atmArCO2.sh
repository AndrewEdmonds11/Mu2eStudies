generate_fcl --dsconf=1atmArCO2 --dsowner=edmonds --description=BeamSplitter --include Production/JobConfig/beam/BeamSplitter.fcl \
--inputs filelists/sim.edmonds.Beam.1atmArCO2.lst --merge-factor 1000
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/BeamSpliter_1atmArCO2_$dirname
  mv $dirname grid_fcl/BeamSpliter_1atmArCO2_$dirname
 fi
done
