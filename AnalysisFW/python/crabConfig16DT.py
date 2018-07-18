from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# Choose ak4/ak8/zb
Mode='zb'
Tag=''
if Mode=='ak8':
  Tag='_AK8'
elif Mode=='zb':
  Tag='_ZB'

config.General.transferOutputs = True
config.General.transferLogs = False
config.General.workArea = 'runs'

config.JobType.pluginName = 'Analysis'
config.Data.splitting = 'LumiBased'
config.Data.inputDBS = 'global'
config.Data.outLFNDirBase = '/store/user/hsiikone'
config.Data.publication = False

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

    ## B16-ver1 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16BI.py'
    #config.General.requestName = 'Run16B1'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver1-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## B16-ver2 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16BII.py'
    #config.General.requestName = 'Run16B2'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver2-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## C16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16C.py'
    #config.General.requestName = 'Run16C'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016C-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## D16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16D.py'
    #config.General.requestName = 'Run16D'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016D-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## E16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16E.py'
    #config.General.requestName = 'Run16E'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016E-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## Fearly16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16Fe.py'
    #config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fe/lumis.json'
    #config.General.requestName = 'Run16Fe'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    # Flate16 Legacy
    config.JobType.psetName = 'cfg/'+Mode+'16Fl.py'
    config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fl/lumis.json'
    config.General.requestName = 'Run16Fl'+Tag+'_Legacy_mAOD'
    config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
    config.Data.unitsPerJob = 10
    submit(config)

    ## G16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16G.py'
    #config.General.requestName = 'Run16G'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016G-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)

    ## H16 Legacy
    #config.JobType.psetName = 'cfg/'+Mode+'16H.py'
    #config.General.requestName = 'Run16H'+Tag+'_Legacy_mAOD'
    #config.Data.inputDataset = '/JetHT/Run2016H-07Aug17-v1/MINIAOD'
    #config.Data.unitsPerJob = 10
    #submit(config)
