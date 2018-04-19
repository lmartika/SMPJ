from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducer16DT.py'
config.JobType.pluginName = 'Analysis'
#config.outputFiles = ['DATA_ProcessedTreeProducer_2.root']
#config.Data.runRange = '193093-193999'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029/src/SMPJ/AnalysisFW/python/RunG.json'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029/src/SMPJ/AnalysisFW/python/Lumis_Fearly.txt'
# B16-ver1 Legacy
#config.General.requestName = 'RunB1_07Aug17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver1-v1/AOD'
# B16-ver2 Legacy
#config.General.requestName = 'RunB2_07Aug17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver2-v1/AOD'
# C16 Legacy
#config.General.requestName = 'RunC_07Aug17-v1-Golden-left'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029P/src/SMPJ/AnalysisFW/python/RunCLeft.json'
#config.Data.inputDataset = '/JetHT/Run2016C-07Aug17-v1/AOD'
# D16 Legacy
#config.General.requestName = 'RunD_07Aug17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2016D-07Aug17-v1/AOD'
# E16 Legacy
#config.General.requestName = 'RunE_07Aug17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2016E-07Aug17-v1/AOD'
# Fearly16 Legacy
#config.General.requestName = 'RunFearly_07Aug17-v1-Golden'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029P/src/SMPJ/AnalysisFW/python/Lumis_Fearly.txt'
#config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/AOD'
# Flate16 Legacy
#config.General.requestName = 'RunFlate_07Aug17-v1-Golden'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029P/src/SMPJ/AnalysisFW/python/Lumis_Flate.txt'
#config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/AOD'
# G16 Legacy
config.Data.lumiMask = 'Gleft.json'
config.General.requestName = 'RunG_07Aug17-v1-Golden-left'
config.Data.inputDataset = '/JetHT/Run2016G-07Aug17-v1/AOD'
# H16 Legacy
#config.General.requestName = 'RunH_07Aug17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2016H-07Aug17-v1/AOD'

#config.Data.inputDataset = '/JetHT/Run2016G-23Sep2016-v1/AOD'
#config.Data.inputDataset = '/JetHT/Run2017B-17Nov2017-v1/AOD'

#config.section_("Data")
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 1
#NJOBS = 1000
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False
#config.Data.outputDatasetTag = 'RunH_ReReReco_SMPJtuple'

#config.section_("Site")
config.Site.storageSite = 'T2_FI_HIP'

