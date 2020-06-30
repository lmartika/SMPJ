from CRABClient.UserUtilities import config
config = config()

## Configurable parameters begin
Tag=''
# Choose 16/17/18
RunYear='17'
# Era
Era='F'
# Choose True for DT, False for MC
DTMode=False
# Choose nu/py
MCType='nu'

config.General.transferOutputs = True
config.General.transferLogs = False

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
    config.General.workArea = 'runs_dt'
    config.Data.splitting = 'LumiBased'
    config.Data.unitsPerJob = 100 
    config.JobType.psetName = 'cfg/nn'+RunYear+Era+'.py'
    config.General.requestName = 'PartsRun'+RunYear+Era+Tag
    Ver = '1'
    Campaign = ''
    # UL ReReco
    if RunYear=='17':
      # CMSSW 10_6_2: Era == B/C/D/E/F
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSONmod.txt'
      Campaign = '09Aug2019_UL2017'
    config.Data.inputDataset = '/ZeroBias/Run20'+RunYear+Era+'-'+Campaign+'-v'+Ver+'/MINIAOD'
  else:
    config.General.workArea = 'runs_mc'
    config.Data.splitting = 'FileBased'

    # Neutrino Gun settings
    config.JobType.psetName = 'cfg/nn'+RunYear+'.py'
    config.General.requestName = 'PartsRun'+RunYear+MCType+Tag
    config.Data.unitsPerJob = 10
    if RunYear=='16':
      # The files presented here are produced on CMSSW_9_4_9, original AODSIM is from 80X, not 94X
      if MCType=='nu':
        # The tag "magnetOff" here does not imply anything meaningful. These ntuples are used to monitor pileup.
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer16MiniAODv3-PUMoriond17_magnetOff_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
      elif MCType=='py':
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIISummer16MiniAODv3-PUMoriond17_magnetOn_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_2_8-9_4_6
      if MCType=='nu': 
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
      elif MCType=='py':
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
    elif RunYear=='18':
      if MCType=='nu':
        #config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
      elif MCType=='py':
        #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
        config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
  print(config.Data.inputDataset)
  submit(config)
