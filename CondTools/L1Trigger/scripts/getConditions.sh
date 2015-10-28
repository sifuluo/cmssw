#!/bin/sh

nflag=0
while getopts 'nph' OPTION
  do
  case $OPTION in
      n) nflag=1
          ;;
      h) echo "Usage: [-n] tsckey"
          echo "  -n: no RS"
          exit
          ;;
  esac
done
shift $(($OPTIND - 1))

# get argument
key=$1

if [ $# -lt 1 ]
    then
    echo "Wrong number of arguments.  Usage: $0 tsckey"
    exit 127
fi

if [ -f l1config.db ]
    then
    mv l1config.db l1config.db.save
fi

# copy default objects
#cmsRun $CMSSW_BASE/src/CondTools/L1Trigger/test/L1ConfigWritePayloadCondDB_cfg.py  inputDBConnect=oracle://cms_orcon_prod/CMS_COND_31X_L1T inputDBAuth=.
cmsRun $CMSSW_BASE/src/CondTools/L1Trigger/test/L1ConfigWritePayloadCondDB_cfg.py  inputDBConnect=oracle://cms_orcon_prod/CMS_CONDITIONS inputDBAuth=.
cmsRun $CMSSW_BASE/src/CondTools/L1Trigger/test/L1ConfigWriteIOVDummy_cfg.py useO2OTags=1

echo "`date` : writing TSC payloads"
$CMSSW_BASE/src/CondTools/L1Trigger/scripts/runL1-O2O-key.sh -c ${key}
o2ocode=$?

if [ ${o2ocode} -eq 0 ]
    then
    echo "L1-O2O-INFO: TSC payloads OK"
else
    echo "L1-O2O-ERROR: TSC payloads not OK!"
    echo "L1-O2O-ERROR: TSC payloads not OK!" 1>&2
    exit ${o2ocode}
fi

echo "`date` : setting TSC IOVs"
$CMSSW_BASE/src/CondTools/L1Trigger/scripts/runL1-O2O-iov.sh 10 ${key}
o2ocode=$?

if [ ${o2ocode} -eq 0 ]
     then
    echo "L1-O2O-INFO: TSC IOVs OK"
else
    echo "L1-O2O-ERROR: TSC IOVs not OK!"
    echo "L1-O2O-ERROR: TSC IOVs not OK!" 1>&2
    exit ${o2ocode}
fi

if [ ${nflag} -eq 0 ]
    then
    echo "`date` : writing RS payloads and setting RS IOVs"
    $CMSSW_BASE/src/CondTools/L1Trigger/scripts/runL1-O2O-rs.sh 10
    o2ocode=$?

    if [ ${o2ocode} -eq 0 ]
	then
	echo "L1-O2O-INFO: RS OK"
    else
	echo "L1-O2O-ERROR: RS not OK!"
	echo "L1-O2O-ERROR: RS not OK!" 1>&2
	exit ${o2ocode}
    fi
else
    echo "`date` : using default RS payloads"
fi

exit ${o2ocode}
