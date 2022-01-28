#!/usr/bin/bash
# create the input list
dataset=sim.mu2e.MuminusTargetStopsCat.MDC2020$1.art
rm filelists/MuminusTargetStopsCat.txt
samListLocations --defname="$dataset" > filelists/MuminusTargetStopsCat.txt
# calucate the max skip from the dataset
nfiles=`../../Production/MDC2020/POMS/samCountFiles.sh $dataset`
nevts=`../../Production/MDC2020/POMS/samCountEvents.sh $dataset`
let nskip=nevts/nfiles
# write the template fcl
TEMPLATE_FCL=fcl/template.fcl
rm $TEMPLATE_FCL
echo '#include "Production/JobConfig/pileup/MuStopPileup.fcl"' >> $TEMPLATE_FCL
echo physics.filters.TargetStopResampler.mu2e.MaxEventsToSkip: ${nskip} >> $TEMPLATE_FCL
echo physics.producers.compressDetStepMCs.debugLevel : 1 >> $TEMPLATE_FCL
#
generate_fcl --dsconf=MDC2020$1 --dsowner=edmonds --run-number=1205 --description=MuStopPileup --events-per-job=200000 --njobs=1 \
--embed $TEMPLATE_FCL --auxinput=1:physics.filters.TargetStopResampler.fileNames:filelists/MuminusTargetStopsCat.txt
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/MuStopPileup$1_$dirname
  mv $dirname grid_fcl/MuStopPileup$1_$dirname
 fi
done

