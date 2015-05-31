
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()


config.General.requestName = 'MC_SingleTop_TestHltAOD_150531'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'runBTaggingReaderTestAOD_cfg.py'
config.Data.inputDataset = '/TT_Tune4C_13TeV-pythia8-tauola/almorton-almorton_CRAB3_Test_ReRunHLT_150529-834cdd9c15c7858dcaa00386fc68ada7/USER'
config.Data.inputDBS = 'global' #Variable to be used when analysing data
config.Data.splitting = 'FileBased' #EventBased for generation, FileBased for analysing data 
config.Data.unitsPerJob = 10
config.Data.totalUnits = 10

config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())

config.Data.publication = False
config.Data.publishDataName = 'CRAB3_SingleTopPaths_TestHltAOD_150531'

config.Site.storageSite = 'T2_UK_London_Brunel'







