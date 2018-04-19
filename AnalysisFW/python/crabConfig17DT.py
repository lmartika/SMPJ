from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducer17DT.py'
config.JobType.pluginName = 'Analysis'
#config.outputFiles = ['DATA_ProcessedTreeProducer_2.root']
#config.Data.runRange = '193093-193999'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029/src/SMPJ/AnalysisFW/python/RunG.json'
#config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/TUPLES_8029/src/SMPJ/AnalysisFW/python/Lumis_Fearly.txt'
# A17 ReReco
#config.General.requestName = 'RunA_17Nov17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2017A-12Sep2017-v1/AOD'
#config.Data.unitsPerJob = 10
# B17 ReReco
#config.General.requestName = 'RunB_17Nov17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2017B-17Nov2017-v1/AOD'
#config.Data.unitsPerJob = 10
# C17 ReReco
#config.General.requestName = 'RunC_17Nov17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2017C-17Nov2017-v1/AOD'
#config.Data.unitsPerJob = 10
# D17 ReReco
#config.General.requestName = 'RunD_17Nov17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2017D-17Nov2017-v1/AOD'
#config.Data.unitsPerJob = 10
# E17 ReReco
config.Data.lumiMask = 'ELeft.json'
config.General.requestName = 'RunE_17Nov17-v1-Golden-left'
config.Data.inputDataset = '/JetHT/Run2017E-17Nov2017-v1/AOD'
config.Data.unitsPerJob = 1
# F17 ReReco
#config.General.requestName = 'RunF_17Nov17-v1-Golden'
#config.Data.inputDataset = '/JetHT/Run2017F-17Nov2017-v1/AOD'
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

