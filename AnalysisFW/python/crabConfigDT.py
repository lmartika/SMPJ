from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

# Choose 16/17/18
RunYear='17'
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

    if RunYear=='16':
      # B16-ver1 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16BI.py'
      config.General.requestName = 'Run16B1'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver1-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # B16-ver2 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16BII.py'
      config.General.requestName = 'Run16B2'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016B-07Aug17_ver2-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16C.py'
      config.General.requestName = 'Run16C'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016C-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16D.py'
      config.General.requestName = 'Run16D'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016D-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # E16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16E.py'
      config.General.requestName = 'Run16E'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016E-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Fearly16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fe.py'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fe/lumis.json'
      config.General.requestName = 'Run16Fe'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # Flate16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16Fl.py'
      config.Data.lumiMask = '/afs/cern.ch/user/h/hsiikone/work/MAOD8029/src/SMPJ/AnalysisFW/triggerinfo/16new/Fl/lumis.json'
      config.General.requestName = 'Run16Fl'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016F-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # G16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16G.py'
      config.General.requestName = 'Run16G'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016G-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # H16 Legacy ReReco
      config.JobType.psetName = 'cfg/'+Mode+'16H.py'
      config.General.requestName = 'Run16H'+Tag+'_Legacy_mAOD'
      config.Data.inputDataset = '/JetHT/Run2016H-07Aug17-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

    elif RunYear=='17':
      # B17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17B.py'
      config.General.requestName = 'Run17B'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = '/JetHT/Run2017B-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17C.py'
      config.General.requestName = 'Run17C'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = '/JetHT/Run2017C-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17D.py'
      config.General.requestName = 'Run17D'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = '/JetHT/Run2017D-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # E17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17E.py'
      config.General.requestName = 'Run17E'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = '/JetHT/Run2017E-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # D17 March18 ReReco
      config.JobType.psetName = 'cfg/'+Mode+'17F.py'
      config.General.requestName = 'Run17F'+Tag+'_Mar18_mAOD'
      config.Data.inputDataset = '/JetHT/Run2017F-31Mar2018-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

    elif RunYear=='18':
      # A18-PR-v1
      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
      config.General.requestName = 'Run18A1'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018A-PromptReco-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)
      # A18-PR-v2
      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
      config.General.requestName = 'Run18A2'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018A-PromptReco-v2/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)
      # A18-PR-v3
      config.JobType.psetName = 'cfg/'+Mode+'18A.py'
      config.General.requestName = 'Run18A3'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018A-PromptReco-v3/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # B18-PR-v1
      config.JobType.psetName = 'cfg/'+Mode+'18B.py'
      config.General.requestName = 'Run18B1'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018B-PromptReco-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)
      # B18-PR-v2
      config.JobType.psetName = 'cfg/'+Mode+'18B.py'
      config.General.requestName = 'Run18B2'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018B-PromptReco-v2/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

      # C18-PR-v1
      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
      config.General.requestName = 'Run18C1'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018C-PromptReco-v1/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)
      # C18-PR-v2
      config.JobType.psetName = 'cfg/'+Mode+'18C.py'
      config.General.requestName = 'Run18C2'+Tag+'_PR_mAOD'
      config.Data.inputDataset = '/JetHT/Run2018C-PromptReco-v2/MINIAOD'
      config.Data.unitsPerJob = 10
      submit(config)

