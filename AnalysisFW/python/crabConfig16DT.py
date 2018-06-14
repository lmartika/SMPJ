from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducer16DTm.py'
config.JobType.pluginName = 'Analysis'
#config.outputFiles = ['DATA_ProcessedTreeProducer_2.root']
#config.Data.runRange = '193093-193999'

# B16-ver1 Legacy
config.General.requestName = 'Run16B1_Legacy_mAOD'
config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver1-v1/MINIAOD'
config.Data.unitsPerJob = 10
# B16-ver2 Legacy
#config.General.requestName = 'Run16B2_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver2-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# C16 Legacy
#config.General.requestName = 'Run16C_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016C-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# D16 Legacy
#config.General.requestName = 'Run16D_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016D-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# E16 Legacy
#config.General.requestName = 'Run16E_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016E-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# Fearly16 Legacy
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fe/lumis.json'
#config.General.requestName = 'Run16Fe_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# Flate16 Legacy
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fl/lumis.json'
#config.General.requestName = 'Run16Fl_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# G16 Legacy
#config.General.requestName = 'Run16G_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016G-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# H16 Legacy
#config.General.requestName = 'Run16H_Legacy_mAOD'
#config.Data.inputDataset = '/JetHT/Run2016H-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10

#config.section_("Data")
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
#NJOBS = 1000
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False
#config.Data.outputDatasetTag = 'RunH_ReReReco_SMPJtuple'

#config.section_("Site")
config.Site.storageSite = 'T2_FI_HIP'

