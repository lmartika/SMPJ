from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
#from WMCore.Configuration import Configuration
#config = Configuration()

#config.section_("General")
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

#config.section_("JobType")
config.JobType.psetName = 'ProcessedTreeProducerPY.py'
config.JobType.pluginName = 'Analysis'
#config.outputFiles = ['DATA_ProcessedTreeProducer_2.root']
#config.Data.runRange = '193093-193999'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt'
##### Pythia 8 Flat
#config.General.requestName = 'QCD16_CUETP8M1_Flat15to7000_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_magnetOn_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
##### Pythia 8 slices 15-30
#config.General.requestName = 'QCD16_CUETP8M1_Slice15to30_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 25
##### Pythia 8 slices 30-50
#config.General.requestName = 'QCD16_CUETP8M1_Slice30to50_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
##### Pythia 8 slices 50-80
#config.General.requestName = 'QCD16_CUETP8M1_Slice50to80_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
##### Pythia 8 slices 80-120
#config.General.requestName = 'QCD16_CUETP8M1_Slice80to120_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
## extension 2 v 2
#config.General.requestName = 'QCD16_CUETP8M1_Slice80to120_TrancheIV_v6-2e2'
#config.Data.inputDataset = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v2/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 120-170
#config.General.requestName = 'QCD16_CUETP8M1_Slice120to170_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
## extension 1 v 1
#config.General.requestName = 'QCD16_CUETP8M1_Slice120to170_TrancheIV_v6-1e1'
#config.Data.inputDataset = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 170-300
#config.General.requestName = 'QCD16_CUETP8M1_Slice170to300_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
# extension 1 v 1
#config.General.requestName = 'QCD16_CUETP8M1_Slice170to300_TrancheIV_v6-1e1'
#config.Data.inputDataset = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
#config.Data.unitsPerJob = 10
##### Pythia 8 slices 300-470
#config.General.requestName = 'QCD16_CUETP8M1_Slice300to470_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 20
# extension 1 v 1
#config.General.requestName = 'QCD16_CUETP8M1_Slice300to470_TrancheIV_v6-1e1'
#config.Data.inputDataset = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
#config.Data.unitsPerJob = 20
##### Pythia 8 slices 470-600
#config.General.requestName = 'QCD16_CUETP8M1_Slice470to600_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 600-800
#config.General.requestName = 'QCD16_CUETP8M1_Slice600to800_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
## bad : config.Data.inputDataset = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
##### Pythia 8 slices 800-1000
#config.General.requestName = 'QCD16_CUETP8M1_Slice800to1000_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
## bad : config.Data.inputDataset = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
##### Pythia 8 slices 1000-1400
#config.General.requestName = 'QCD16_CUETP8M1_Slice1000to1400_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 5
## bad : config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
##### Pythia 8 slices 1400-1800
#config.General.requestName = 'QCD16_CUETP8M1_Slice1400to1800_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
# extension 1 v 1
#config.General.requestName = 'QCD16_CUETP8M1_Slice1400to1800_TrancheIV_v6-1e1'
#config.Data.inputDataset = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 1800-2400
#config.General.requestName = 'QCD16_CUETP8M1_Slice1800to2400_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
# extension 1 v 2
#config.General.requestName = 'QCD16_CUETP8M1_Slice1800to2400_TrancheIV_v6-2e1'
#config.Data.inputDataset = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 2400-3200
#config.General.requestName = 'QCD16_CUETP8M1_Slice2400to3200_TrancheIV_v6-1'
#config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.unitsPerJob = 10
## extension 1 v 1
#config.General.requestName = 'QCD16_CUETP8M1_Slice2400to3200_TrancheIV_v6-1e1'
#config.Data.inputDataset = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/AODSIM'
#config.Data.unitsPerJob = 5
##### Pythia 8 slices 3200-Inf
#config.General.requestName = 'QCD16_CUETP8M1_Slice3200toInf_TrancheIV_v6-3'
#config.Data.inputDataset = '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/AODSIM'
#config.Data.unitsPerJob = 5

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

