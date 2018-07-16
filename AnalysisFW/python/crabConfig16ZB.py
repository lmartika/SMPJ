from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducer16ZBm.py'
config.JobType.pluginName = 'Analysis'
#config.Data.runRange = '193093-193999'

# B16-ver1 Legacy
#config.General.requestName = 'ZB16B1'
#config.Data.inputDataset = '/ZeroBias/Run2016B-07Aug17_ver1-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# B16-ver2 Legacy
#config.General.requestName = 'ZB16B2'
#config.Data.inputDataset = '/ZeroBias/Run2016B-07Aug17_ver2-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# C16 Legacy
#config.General.requestName = 'ZB16C'
#config.Data.inputDataset = '/ZeroBias/Run2016C-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# D16 Legacy
#config.General.requestName = 'ZB16D'
#config.Data.inputDataset = '/ZeroBias/Run2016D-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# E16 Legacy
#config.General.requestName = 'ZB16E'
#config.Data.inputDataset = '/ZeroBias/Run2016E-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# Fearly16 Legacy
#config.General.requestName = 'ZB16Fe'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/ZB16runs/Fe/lumis.json'
#config.Data.inputDataset = '/ZeroBias/Run2016F-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# Flate16 Legacy
config.General.requestName = 'ZB16Fl_new'
config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/ZB16runs/Fl/lumis.json'
config.Data.inputDataset = '/ZeroBias/Run2016F-07Aug17-v1/MINIAOD'
config.Data.unitsPerJob = 10
# G16 Legacy
#config.General.requestName = 'ZB16G'
#config.Data.inputDataset = '/ZeroBias/Run2016G-07Aug17-v1/MINIAOD'
#config.Data.unitsPerJob = 10
# H16 Legacy
#config.General.requestName = 'ZB16H'
#config.Data.inputDataset = '/ZeroBias/Run2016H-07Aug17-v1/MINIAOD'
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
