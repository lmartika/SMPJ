from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

## Configurable parameters begin

JobsDt=55 #100
TagTag=''
# Choose 16/17/18
RunYear='18'
# Era
Era='B'
# Choose ak4/ak8, zb (DT only)
Mode='ak4'
# Choose True for DT, False for MC
DTMode=False
# Choose py/pylong/hw/nu/mg (MC only)
MCType = 'nu'

# In case of pylong (0-14) or mg (0-8)
RangeIdx = 1

## Configurable parameters end

PtRanges = ['15to30','30to50','50to80','80to120','120to170','170to300','300to470','470to600','600to800','800to1000','1000to1400','1400to1800','1800to2400','2400to3200','3200toInf']
HTRanges = ['HT50to100','HT100to200','HT200to300','HT300to500','HT500to700','HT700to1000','HT1000to1500','HT1500to2000','HT2000toInf']

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
    config.Data.unitsPerJob = JobsDt
    config.JobType.psetName = 'cfg/'+Mode+RunYear+Era+'.py'
    config.General.requestName = 'Run'+RunYear+Era+Tag+TagTag
    Ver = '1'
    Campaign = ''
    # UL ReReco
    if RunYear=='16':
      # CMSSW ????: Era == ????
      #config.Data.lumiMask = 'Fe16lumis.json'
      #config.Data.lumiMask = 'Fl16lumis.json'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt'
    elif RunYear=='17':
      # CMSSW 10_6_2: Era == B/C/D/E/F
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1mod.txt'
      Campaign = '09Aug2019_UL2017'
    elif RunYear=='18':
      # CMSSW 10_6_4_patch1: Era == A/B/C/D
      # ZB: A/B avail, HT: A/B/D avail
      Ver = '2'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'
      Campaign = '12Nov2019_UL2018'
      if Mode=='ak4':
        if Era=='C':
          Campaign += '_rsb'
          Ver = '1'
        if Era=='D': Ver = '4'
      elif Mode=='zb':
        if Era=='C' or Era=='D':
          Campaign += '_rsb'
          Ver = '1'
    config.Data.inputDataset = DTLoc+'/Run20'+RunYear+Era+'-'+Campaign+'-v'+Ver+'/MINIAOD'
    submit(config)
  else:
    if not ExtMode:
      config.General.workArea = 'runs_mc'
    else:
      config.General.workArea = 'runs_extmc'
    config.Data.splitting = 'FileBased'

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

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'16mg.py'

        ##### Madgraph + Pythia8 slices 50-100
        if not ExtMode:
          config.General.requestName = 'QCD16'+Tag+'_MgMLMP8_HT50to100_v3'
          config.Data.inputDataset = '/QCD_HT50to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
      else:
        print "Unknown MC Type, ", MCType

    elif RunYear=='17':
      # The files presented here are produced on CMSSW_9_2_8-9_4_6
      if MCType=='nu': 
        # Neutrino Gun settings
        config.JobType.psetName = 'cfg/'+Mode+'17nu.py'

        ##### neutrino gun
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_SingleNu_PU2017_mc17r_v6_2'
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='hw':
        # Herwig++ settings
        config.JobType.psetName = 'cfg/'+Mode+'17hw.py'

        # Herwig++ Flat
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_HS1_15to7k_mc17r_v6_2'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='py':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'17py.py'

        ##### Pythia 8 Flat
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to7k_S19UL17_mc17r_v6_2'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)
        else:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_15to7k_S19UL17_mc17r_v6_2_e2'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6_ext2-v2/MINIAODSIM'
          config.Data.unitsPerJob = 20
          submit(config)

      elif MCType=='pylong':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'17py.py'

        ##### Pythia 8 Pthat slices
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_'+PtRanges[RangeIdx]+'_S19UL17_mc17r_v6_2'
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'17mg.py'

        ##### Madgraph + Pythia8 HT slices
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL17_mc17r_v6_2'
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
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
          config.General.requestName = 'QCD18'+Tag+'_SingleNu_mc18r_v11_L1v1_v2'
          #config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.inputDataset = '/SingleNeutrino/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
          config.Data.unitsPerJob = 1
          submit(config)

      elif MCType=='hw':
        # Herwig7 settings
        config.JobType.psetName = 'cfg/'+Mode+'18hw.py'

        # Herwig7 Flat
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_CH2_15to7k_mc18r_v11_L1v1_v2'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCH2_Flat_13TeV_herwig7/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='py':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'18py.py'
        
        # Pythia 8 Flat
        if not ExtMode:
          config.General.requestName = 'QCD18'+Tag+'_P8CP5_15to7k_mc18r_v11_L1v1_v2s'
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          #config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer19UL18MiniAOD-FlatPU0to70_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='pylong':
        # Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'18py.py'

        ##### Pythia 8 Pthat slices
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_P8CP5_'+PtRanges[RangeIdx]+'_S19UL17_mc18r_v11_L1v1_v2'
          config.Data.inputDataset = '/QCD_Pt_'+PtRanges[RangeIdx]+'_TuneCP5_13TeV_pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)

      elif MCType=='mg':
        # Madgraph + Pythia8 settings
        config.JobType.psetName = 'cfg/'+Mode+'17mg.py'

        ##### Madgraph + Pythia8 HT slices
        if not ExtMode:
          config.General.requestName = 'QCD17'+Tag+'_MgMLMP8_'+HTRanges[RangeIdx]+'_S19UL17_mc18r_v11_L1v1_v2'
          config.Data.inputDataset = '/QCD_'+HTRanges[RangeIdx]+'_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
          config.Data.unitsPerJob = 10
          submit(config)
        
      else:
        print "Unknown MC Type, ", MCType

