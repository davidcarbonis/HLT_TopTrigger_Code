
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'ReRunHLT_Test_150528'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'ReRunHLTandOutput_cfg.py'
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = '/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/GEN-SIM-RAW'
config.Data.inputDBS = 'global' #Variable to be used when analysing data
config.Data.splitting = 'FileBased' #EventBased for generation, FileBased for analysing data 
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1000

config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.publishDataName = 'almorton_CRAB3_Test_ReRunHLT_150528'

config.Site.storageSite = 'T2_UK_London_Brunel'
