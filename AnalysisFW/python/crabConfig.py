from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# Choose True for DT, False for MC
DTMode=False
# Choose 16/17/18
RunYear='17'
# Choose ak4/ak8, zb (DT only)
Mode='ak4'


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
      # B16-ver1 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16BI.py'
      config.General.requestName = 'Run16B1'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016B-07Aug17_ver1-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # B16-ver2 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16BII.py'
      config.General.requestName = 'Run16B2'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016B-07Aug17_ver2-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16C.py'
      config.General.requestName = 'Run16C'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016C-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16D.py'
      config.General.requestName = 'Run16D'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016D-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # E16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16E.py'
      config.General.requestName = 'Run16E'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016E-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Fearly16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fe.py'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fe/lumis.json'
      config.General.requestName = 'Run16Fe'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016F-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Flate16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fl.py'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fl/lumis.json'
      config.General.requestName = DTLoc+'Run16Fl'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/Run2016F-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # G16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16G.py'
      config.General.requestName = 'Run16G'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016G-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # H16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16H.py'
      config.General.requestName = 'Run16H'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2016H-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

    elif RunYear=='17':
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

      # F17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17F.py'
      config.General.requestName = 'Run17F'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017F-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

#      # H17 Fall17 ReReco FSQJet2
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_FSQ2_Fall17_mAOD'
#      config.Data.inputDataset = '/FSQJet2/Run2017H-17Nov2017-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # H17 Fall17 ReReco LowEGJet
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_LEG_Fall17_mAOD'
#      config.Data.inputDataset = '/LowEGJet/Run2017H-17Nov2017-v2/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # H17 Fall17 ReReco HighEGJet
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_HEG_Fall17_mAOD'
#      config.Data.inputDataset = '/HighEGJet/Run2017H-17Nov2017-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
      # F17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17F.py'
      config.General.requestName = 'Run17F'+Tag+'_May18_mAOD'
      config.Data.inputDataset = DTLoc+'/Run2017F-09May2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

#      # H17 Fall17 ReReco FSQJet2
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_FSQ2_Fall17_mAOD'
#      config.Data.inputDataset = '/FSQJet2/Run2017H-17Nov2017-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # H17 Fall17 ReReco LowEGJet
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_LEG_Fall17_mAOD'
#      config.Data.inputDataset = '/LowEGJet/Run2017H-17Nov2017-v2/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # H17 Fall17 ReReco HighEGJet
#      config.JobType.psetName = 'cfg/ak4ak817H.py'
#      config.General.requestName = 'Run17H_HEG_Fall17_mAOD'
#      config.Data.inputDataset = '/HighEGJet/Run2017H-17Nov2017-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # H17 Fall17 ReReco ZeroBias
#      config.JobType.psetName = 'cfg/zb17H.py'
#      config.General.requestName = 'Run17H_ZB_Fall17_mAOD'
#      config.Data.inputDataset = '/ZeroBias/Run2017H-17Nov2017-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)

    elif RunYear=='18':
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

#      # A18-PR-v1
#      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
#      config.General.requestName = 'Run18A1'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018A-PromptReco-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # A18-PR-v2
#      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
#      config.General.requestName = 'Run18A2'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018A-PromptReco-v2/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # A18-PR-v3
#      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
#      config.General.requestName = 'Run18A3'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018A-PromptReco-v3/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # B18-PR-v1
#      config.JobType.psetName = 'cfg/'+Mode+'18B.py'
#      config.General.requestName = 'Run18B1'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018B-PromptReco-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # B18-PR-v2
#      config.JobType.psetName = 'cfg/'+Mode+'18B.py'
#      config.General.requestName = 'Run18B2'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018B-PromptReco-v2/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#
#      # C18-PR-v1 (trash)
#      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
#      config.General.requestName = 'Run18C1'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018C-PromptReco-v1/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # C18-PR-v2 (trash)
#      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
#      config.General.requestName = 'Run18C2'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018C-PromptReco-v2/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # C18-PR-v3
#      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
#      config.General.requestName = 'Run18C3'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018C-PromptReco-v3/MINIAOD'
#      config.Data.unitsPerJob = 10
#      submit(config)
#      # D18-PR-v1 excluded in the json
#      # D18-PR-v2
#      config.JobType.psetName = 'cfg/'+Mode+'18D.py'
#      config.General.requestName = 'Run18D'+Tag+'_PR_mAOD'
#      config.Data.inputDataset = DTLoc+'/Run2018D-PromptReco-v2/MINIAOD'
#      config.Data.unitsPerJob = 25
#      submit(config)

  else:
    if RunYear=='16':
      ## Herwig++ settings
      #config.JobType.psetName = 'cfg/'+Mode+'16hw.py'

      ## Herwig++ Flat
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_HS1_15to7k_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      # Pythia8 settings
      config.JobType.psetName = 'cfg/'+Mode+'16py.py'

      ##### Neutrino Gun
      if not ExtMode:
        config.General.requestName = 'QCD16'+Tag+'_SingleNu_TIV_v6'
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer16MiniAODv2-PUMoriond17_magnetOff_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
        config.Data.unitsPerJob = 1
        submit(config)

      ###### Pythia 8 Flat
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_15to7k_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_magnetOn_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 15-30
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_15to30_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 25
      #  submit(config)

      ###### Pythia 8 slices 30-50
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_30to50_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 50-80
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_50to80_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 80-120
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_80to120_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 2 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_80to120_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 120-170
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_120to170_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_120to170_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)

      ###### Pythia 8 slices 170-300
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_170to300_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_170to300_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 300-470
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_300to470_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 20
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_300to470_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 20
      #  submit(config)

      ###### Pythia 8 slices 470-600
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_470to600_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)

      ###### Pythia 8 slices 600-800
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_600to800_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_600to800_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 800-1000
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_800to1000_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_800to1000_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 20
      #  submit(config)

      ###### Pythia 8 slices 1000-1400
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1000to1400_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1000to1400_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 1400-1800
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1400to1800_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1400to1800_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)

      ###### Pythia 8 slices 1800-2400
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1800to2400_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_1800to2400_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)

      ###### Pythia 8 slices 2400-3200
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_2400to3200_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  # extension 1 v 1
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_2400to3200_TIV_v6_ext'
      #  config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 3200-Inf
      #if not ExtMode:
      #  config.General.requestName = 'QCD16'+Tag+'_P8M1_3200toInf_TIV_v6'
      #  config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM'
      #  config.Data.unitsPerJob = 5
      #  submit(config)

    elif RunYear=='17':
      ## Herwig++ settings
      #config.JobType.psetName = 'cfg/'+Mode+'17hw.py'

      ## Herwig++ Flat
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_HS1_15to7k_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      # Pythia8 settings
      config.JobType.psetName = 'cfg/'+Mode+'17py.py'

      ##### Neutrino Gun
      if not ExtMode:
        config.General.requestName = 'QCD17'+Tag+'_SingleNu_PU2017_mc17r_v11'
        config.Data.inputDataset = '/SingleNeutrino/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
        config.Data.unitsPerJob = 1
        submit(config)


      ###### Pythia 8 Flat (old tune)
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8M1_15to7k_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 Flat (new tune)
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to7k_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 20
      #  submit(config)
      #
      ###### Pythia 8 slices 15-30
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to30_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_15to30_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 30-50
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_30to50_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_30to50_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 50-80
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_50to80_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_50to80_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 80-120
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_80to120_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_80to120_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 120-170
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_120to170_mc17real_v14'
      #  config.Data.inputDataset = '/QCD_Pt_120to170_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 170-300
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_170to300_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_170to300_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 300-470
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_300to470_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_300to470_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 470-600
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_470to600_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_470to600_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 600-800
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_600to800_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_600to800_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 800-1000
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_800to1000_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_800to1000_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)

      ###### Pythia 8 slices 1000-1400
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_1000to1400_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      ##else:
      #  # extension 1 v 2
      #  #config.General.requestName = 'QCD17'+Tag+'_P8CP5_1000to1400_mc17r_v14_ext'
      #  #config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  #config.Data.unitsPerJob = 10
      #  #submit(config)
      #
      ###### Pythia 8 slices 1400-1800
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_1400to1800_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)
      #else:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_1400to1800_mc17r_v14_ext'
      #  config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 1800-2400
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_1800to2400_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 2400-3200
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_2400to3200_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      ###### Pythia 8 slices 3200-Inf
      #if not ExtMode:
      #  config.General.requestName = 'QCD17'+Tag+'_P8CP5_3200toInf_mc17r_v14'
      #  config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

    elif RunYear=='18':

      # Pythia8 settings
      config.JobType.psetName = 'cfg/'+Mode+'18py.py'

      ##### Neutrino Gun
      if not ExtMode:
        config.General.requestName = 'QCD18'+Tag+'_SingleNu_mc18r_v15'
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer16MiniAODv2-PUMoriond17_magnetOff_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM '
        config.Data.unitsPerJob = 1
        submit(config)
      
      ###### Pythia 8 Flat
      #if not ExtMode:
      #  config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to7k_mc18r_v15nnn'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'
      #  #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'
      #  #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM'
      #  #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIIFall18MiniAOD-102X_upgrade2018_realistic_v12-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 1
      #  submit(config)
      #else:
      #  config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to7k_mc18r_v12_ext1'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIIFall18MiniAOD-102X_upgrade2018_realistic_v12_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

      #  config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to7k_mc18r_v12_ext1b'
      #  config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIFall18MiniAOD-102X_upgrade2018_realistic_v12_ext1-v1/MINIAODSIM'
      #  config.Data.unitsPerJob = 10
      #  submit(config)

