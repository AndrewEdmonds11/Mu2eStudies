#!/usr/bin/bash
# create the input list
dataset=sim.mu2e.MuminusTargetStopsCat.MDC2020$1.art
samListLocations --defname="$dataset" > filelists/sim.mu2e.MuminusTargetStopsCat.MDC2020$1.lst
# calucate the max skip from the dataset
nfiles=`../../Production/MDC2020/POMS/samCountFiles.sh $dataset`
nevts=`../../Production/MDC2020/POMS/samCountEvents.sh $dataset`
let nskip=nevts/nfiles
# write the template fcl
TEMPLATE_FCL=fcl/CeEndpoint.fcl
rm $TEMPLATE_FCL
echo '#include "Production/JobConfig/primary/CeEndpoint.fcl"' >> $TEMPLATE_FCL
echo physics.filters.TargetStopResampler.mu2e.MaxEventsToSkip: ${nskip} >> $TEMPLATE_FCL
#
generate_fcl --dsconf=test --dsowner=edmonds --run-number=1210 --description=CeEndpoint --events-per-job=100 --njobs=1 \
--embed $TEMPLATE_FCL --auxinput=1:physics.filters.TargetStopResampler.fileNames:filelists/sim.mu2e.MuminusTargetStopsCat.MDC2020$1.lst
for dirname in 000 001 002 003 004 005 006 007 008 009; do
 if test -d $dirname; then
  echo "found dir $dirname"
  rm -rf grid_fcl/CeEndpoint_$dirname
  mv $dirname grid_fcl/CeEndpoint_$dirname
 fi
done

