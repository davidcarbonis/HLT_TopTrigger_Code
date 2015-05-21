from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'ReRunHLT_Test_150520a'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'ReRunHLTandOutput_cfg.py'

config.Data.inputDataset = '/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/GEN-SIM-RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'

config.Data.unitsPerJob = 1
config.Data.totalUnits = 1

config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.publishDataName = 'CRAB3_ReRunHLT_150520a'

config.Site.storageSite = 'T2_UK_London_Brunel'
#config.Site.whitelist = ['T2_UK_London_Brunel','T2_US_Florida,T2_CH_CERN','T2_CH_CERN']
config.Site.blacklist = ['T2_DE_DESY']
