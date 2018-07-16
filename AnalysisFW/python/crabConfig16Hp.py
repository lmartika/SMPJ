from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducer16Hpm.py'
config.JobType.pluginName = 'Analysis'
#config.outputFiles = ['DATA_ProcessedTreeProducer_2.root']
#config.Data.runRange = '193093-193999'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt'
# Herwig++ Flat
config.General.requestName = 'QCD16_HS1_Flat_TIV_v6'
config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

#config.section_("Data")
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 10
#NJOBS = 1000
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False
#config.Data.outputDatasetTag = 'RunH_ReReReco_SMPJtuple'

#config.section_("Site")
config.Site.storageSite = 'T2_FI_HIP'

