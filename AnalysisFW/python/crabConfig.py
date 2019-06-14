from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

## Configurable parameters begin

# Choose 16/17/18
RunYear='16'
# Choose ak4/ak8, zb (DT only)
Mode='ak4'
# Choose True for DT, False for MC
DTMode=False
# Choose py/pylong/hw/nu/mg (MC only)
MCType = 'nu'

## Configurable parameters end

Tag=''
DTLoc='/JetHT'
if Mode=='ak8':
  Tag='_AK8'
elif Mode=='zb':
  if DTMode:
    Tag='_ZB'
    DTLoc='/ZeroBias'
  else:
    Mode='ak4'
# Run only extensions (MC)
ExtMode=False

config.General.transferOutputs = True
config.General.transferLogs = False

if DTMode:
  config.General.workArea = 'runs_dt'
  config.Data.splitting = 'LumiBased'
else:
  if not ExtMode:
    config.General.workArea = 'runs_mc'
  else:
    config.General.workArea = 'runs_extmc'
  config.Data.splitting = 'FileBased'

config.Data.inputDBS = 'global'
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False

config.JobType.pluginName = 'Analysis'
config.Site.storageSite = 'T2_FI_HIP'

if __name__ == '__main__':

  from CRABAPI.RawCommand import crabCommand
  from CRABClient.ClientExceptions import ClientException
  from httplib import HTTPException

  def submit(config):
    try:
      crabCommand('submit', config = config)
    except HTTPException as hte:
      print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
      print "Failed submitting task: %s" % (cle)

  if DTMode:
    if RunYear=='16':
      # The files presented here are produced on CMSSW_9_4_9, original AODSIM is from 80X, not 94X

      # The first sector (ver1 == original PromptReco-v1) is completely rejected by the json. 
      # B16-ver1 Legacy ReReco
      #config.JobType.psetName = 'cfg/'+Mode+'16BI.py'
      #config.General.requestName = 'Run16B1'+Tag+'_Legacy_mAOD'
      #config.Data.inputDataset = DTLoc+'/Run2016B-17Jul2018_ver1-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)

      # B16-ver2 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16BII.py'
      config.General.requestName = 'Run16B2'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016B-17Jul2018_ver2-v2/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16C.py'
      config.General.requestName = 'Run16C'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016C-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16D.py'
      config.General.requestName = 'Run16D'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016D-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # E16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16E.py'
      config.General.requestName = 'Run16E'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016E-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Fearly16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fe.py'
      config.Data.lumiMask = 'Fe16lumis.json'
      config.General.requestName = 'Run16Fe'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016F-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Flate16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fl.py'
      config.Data.lumiMask = 'Fl16lumis.json'
      config.General.requestName = 'Run16Fl'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016F-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # G16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16G.py'
      config.General.requestName = 'Run16G'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016G-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # H16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16H.py'
      config.General.requestName = 'Run16H'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016H-17Jul2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_4_5-9_4_7

      # B17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17B.py'
      config.General.requestName = 'Run17B'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017B-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17C.py'
      config.General.requestName = 'Run17C'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017C-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17D.py'
      config.General.requestName = 'Run17D'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017D-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # E17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17E.py'
      config.General.requestName = 'Run17E'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017E-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # For Run2017F only, there is a fresher rereco
      ## F17 March18 ReReco
      #config.JobType.psetName = 'cfg/'+Mode+'17F.py'
      #config.General.requestName = 'Run17F'+Tag+'_Mar18_mAOD'
      #config.Data.inputDataset = DTLoc+'/Run2017F-31Mar2018-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)

      # F17 May18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17F.py'
      config.General.requestName = 'Run17F'+Tag+'_May18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017F-09May2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # These are special runs performed in the end of 2017. Typically not used.
      ## H17 Fall17 ReReco FSQJet2
      #config.JobType.psetName = 'cfg/ak4ak817H.py'
      #config.General.requestName = 'Run17H_FSQ2_Fall17_mAOD'
      #config.Data.inputDataset = '/FSQJet2/Run2017H-17Nov2017-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)
      #
      ## H17 Fall17 ReReco LowEGJet
      #config.JobType.psetName = 'cfg/ak4ak817H.py'
      #config.General.requestName = 'Run17H_LEG_Fall17_mAOD'
      #config.Data.inputDataset = '/LowEGJet/Run2017H-17Nov2017-v2/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)
      #
      ## H17 Fall17 ReReco HighEGJet
      #config.JobType.psetName = 'cfg/ak4ak817H.py'
      #config.General.requestName = 'Run17H_HEG_Fall17_mAOD'
      #config.Data.inputDataset = '/HighEGJet/Run2017H-17Nov2017-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)
      #
      ## H17 Fall17 ReReco ZeroBias
      #config.JobType.psetName = 'cfg/zb17H.py'
      #config.General.requestName = 'Run17H_ZB_Fall17_mAOD'
      #config.Data.inputDataset = '/ZeroBias/Run2017H-17Nov2017-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)

    elif RunYear=='18':
      # The files presented here are produced on CMSSW_10_2_1-10_2_5

      # A18-Sep18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
      config.General.requestName = 'Run18A'+Tag+'_17Sep18RR_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2018A-17Sep2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # B18-Sep18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'18B.py'
      config.General.requestName = 'Run18B'+Tag+'_17Sep18RR_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2018B-17Sep2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C18-Sep18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
      config.General.requestName = 'Run18C'+Tag+'_17Sep18RR_mAOD1'
      config.Data.inputDataset = DTLoc+'/Run2018C-17Sep2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D18: PromptReco equal to ReReco in previous eras. D18-PR-v2 holds all the info. -v1 is very small.
      config.JobType.psetName = 'cfg/'+Mode+'18D.py'
      config.General.requestName = 'Run18D'+Tag+'_PR_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2018D-PromptReco-v2/MINIAOD'
      config.Data.unitsPerJob = 25
      submit(config)

      # This is a special run.
      #config.JobType.psetName = 'cfg/'+Mode+'18E.py'
      #config.General.requestName = 'Run18E'+Tag+'_PR_mAOD'
      #config.Data.inputDataset = DTLoc+'/Run2018E-PromptReco-v1/MINIAOD'
      #config.Data.unitsPerJob = 10
      #submit(config)

  else:
    if RunYear=='16':
      # The files presented here are produced on CMSSW_9_4_9, original AODSIM is from 80X, not 94X
      if MCType=='nu':
        # Neutrino Gun settings
        config.JobType.psetName = 'cfg/'+Mode+'16nu.py'

        ##### Neutrino Gun
        if not ExtMode:
          # The tag "magnetOff" here does not imply anything meaningful. These ntuples are used to monitor pileup.
          config.General.requestName = 'QCD16'+Tag+'_SingleNu_v3'
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer16MiniAODv3-PUMoriond17_magnetOff_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 1
          submit(config)
      elif MCType=='hw':
        # Herwig++ settings
        config.JobType.psetName = 'cfg/'+Mode+'16hw.py'

        ##### Herwig++ Flat
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_HS1_15to7k_v3'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='py':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'16py.py'

        ##### Pythia 8 Flat
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_15to7k_v3'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_magnetOn_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='pylong':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'16py.py'

        ##### Pythia 8 slices 15-30
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_15to30_v3'
          config.Data.inputDataset = '/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 25
          submit(config)

        ##### Pythia 8 slices 30-50
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_30to50_v3'
          config.Data.inputDataset = '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 50-80
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_50to80_v3'
          config.Data.inputDataset = '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 80-120
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_80to120_v3'
          config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_80to120_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 120-170
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_120to170_v3'
          config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_120to170_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)

        ##### Pythia 8 slices 170-300
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_170to300_v3'
          config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_170to300_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 300-470
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_300to470_v3'
          config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_300to470_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)

        ##### Pythia 8 slices 470-600
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_470to600_v3'
          config.Data.inputDataset = '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)

        ##### Pythia 8 slices 600-800
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_600to800_v3'
          config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_600to800_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 800-1000
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_800to1000_v3'
          config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_800to1000_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)

        ##### Pythia 8 slices 1000-1400
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1000to1400_v3'
          config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1000to1400_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 1400-1800
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1400to1800_v3'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1400to1800_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)

        ##### Pythia 8 slices 1800-2400
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1800to2400_v3'
          config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_1800to2400_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)

        ##### Pythia 8 slices 2400-3200
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_2400to3200_v3'
          config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_2400to3200_v3_ext'
          config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 3200-Inf
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_P8M1_3200toInf_v3'
          config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 5
          submit(config)

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'16mg.py'

        ##### Madgraph + Pythia8 slices 50-100
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT50to100_v3'
          config.Data.inputDataset = '/QCD_HT50to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 100-200
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT100to200_v3'
          config.Data.inputDataset = '/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 200-300
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT200to300_v3'
          config.Data.inputDataset = '/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT200to300_v3_ext'
          config.Data.inputDataset = '/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 300-500
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT300to500_v3'
          config.Data.inputDataset = '/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT300to500_v3_ext'
          config.Data.inputDataset = '/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 500-700
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT500to700_v3'
          config.Data.inputDataset = '/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT500to700_v3_ext'
          config.Data.inputDataset = '/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 700-1000
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT700to1000_v3'
          config.Data.inputDataset = '/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT700to1000_v3_ext'
          config.Data.inputDataset = '/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1000-1500
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT1000to1500_v3'
          config.Data.inputDataset = '/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT1000to1500_v3_ext'
          config.Data.inputDataset = '/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1500-2000
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT1500to2000_v3'
          config.Data.inputDataset = '/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT1500to2000_v3_ext'
          config.Data.inputDataset = '/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 2000-Inf
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT2000toInf_v3'
          config.Data.inputDataset = '/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT2000toInf_v3_ext'
          config.Data.inputDataset = '/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      else:
        print "Unknown MC Type, ", MCType

    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_2_8-9_4_6
      if MCType=='nu': 
        # Neutrino Gun settings
        config.jobtype.psetname = 'cfg/'+mode+'17nu.py'

        ##### neutrino gun
        if not extmode:
          config.general.requestname = 'QCD17'+Tag+'_SingleNu_PU2017_mc17r_v14'
          config.data.inputdataset = '/SingleNeutrino/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.data.unitsperjob = 1
          submit(config)

      elif MCType=='hw':
        # Herwig++ settings
        config.JobType.psetName = 'cfg/'+Mode+'17hw.py'

        # Herwig++ Flat
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_HS1_15to7k_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='py':
        # Pythia8 settings
        config.jobtype.psetname = 'cfg/'+mode+'17py.py'

        ##### Pythia 8 Flat
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to7k_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)

      elif MCType=='pylong':
        # Pythia8 settings
        config.jobtype.psetname = 'cfg/'+mode+'17py.py'

        ##### Pythia 8 slices 15-30
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to30_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_15to30_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 30-50
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_30to50_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_30to50_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 50-80
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_50to80_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_50to80_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 80-120
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_80to120_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_80to120_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 120-170
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_120to170_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_120to170_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 170-300
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_170to300_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_170to300_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 300-470
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_300to470_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_300to470_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 470-600
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_470to600_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_470to600_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 600-800
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_600to800_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_600to800_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 800-1000
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_800to1000_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_800to1000_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 1000-1400
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_1000to1400_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_1000to1400_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Pythia 8 slices 1400-1800
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_1400to1800_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_1400to1800_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 1800-2400
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_1800to2400_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 2400-3200
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_2400to3200_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 3200-Inf
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_3200toInf_mc17r_v14'
          config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'17mg.py'

        ##### Madgraph + Pythia8 slices 50-100
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT50to100_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 100-200
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT100to200_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT100to200_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT100to200_mc17r_v14_ext'
          config.Data.inputDataset = '/QCD_HT100to200_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 200-300
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT200to300_mc17r_v14_npmx'
          config.Data.inputDataset = '/QCD_HT200to300_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT200to300_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT200to300_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 300-500
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT300to500_mc17r_v14_npmx'
          config.Data.inputDataset = '/QCD_HT300to500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT300to500_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT300to500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 500-700
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT500to700_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT500to700_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT500to700_mc17r_v14_opmx'
          config.Data.inputDataset = '/QCD_HT500to700_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_old_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 700-1000
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT700to1000_mc17r_v14_npmx'
          config.Data.inputDataset = '/QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT700to1000_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1000-1500
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT1000to1500_mc17r_v14_npmx'
          config.Data.inputDataset = '/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT1000to1500_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1500-2000
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT1500to2000_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT1500to2000_mc17r_v14_opmx'
          config.Data.inputDataset = '/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_old_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 2000-Inf
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT2000toInf_mc17r_v14'
          config.Data.inputDataset = '/QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_HT2000toInf_mc17r_v14_opmx'
          config.Data.inputDataset = '/QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_old_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      else:
        print "Unknown MC Type, ", MCType

    elif RunYear=='18':
      if MCType=='nu':
        # Neutrino Gun settings
        config.JobType.psetName = 'cfg/'+Mode+'18nu.py'

        ##### Neutrino Gun
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_SingleNu_mc18r_v15'
          config.Data.inputDataset = '/SingleNeutrino/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'
          config.Data.unitsPerJob = 1
          submit(config)

      elif MCType=='hw':
        # Herwig7 settings
        config.JobType.psetName = 'cfg/'+Mode+'18hw.py'

        # Herwig7 Flat
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_CH2_15to7k_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH2_Flat_13TeV_herwig7/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='py':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'18py.py'
        
        # Pythia 8 Flat
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to7k_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='pylong':
        # Pythia8 settings
        config.jobtype.psetname = 'cfg/'+mode+'18py.py'

        ##### Pythia 8 slices 15-30
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to30_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_15to30_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 30-50
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_30to50_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_30to50_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 50-80
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_50to80_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_50to80_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 80-120
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_80to120_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 120-170
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_120to170_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_120to170_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 170-300
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_170to300_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 300-470
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_300to470_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 470-600
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_470to600_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_470to600_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_470to600_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_470to600_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 600-800
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_600to800_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 800-1000
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_800to1000_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 1000-1400
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_1000to1400_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Pythia 8 slices 1400-1800
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_1400to1800_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_1400to1800_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 1800-2400
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_1800to2400_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_1800to2400_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 2400-3200
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_2400to3200_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_2400to3200_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

        ##### Pythia 8 slices 3200-Inf
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_3200toInf_mc18r_v15'
          config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        else:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_3200toInf_mc18r_v15_ext'
          config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'18mg.py'

        ##### Madgraph + Pythia8 slices 50-100
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT50to100_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 100-200
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT100to200_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 200-300
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT200to300_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 300-500
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT300to500_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 500-700
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT500to700_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 700-1000
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT700to1000_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1000-1500
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT1000to1500_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 1500-2000
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT1500to2000_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
        ##### Madgraph + Pythia8 slices 2000-Inf
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_MgMLMP8_HT2000toInf_mc18r_v15'
          config.Data.inputDataset = '/QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      else:
        print "Unknown MC Type, ", MCType

