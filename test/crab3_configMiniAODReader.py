
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()


config.General.requestName = 'MC_MiniAODTest_150528a'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'runBTaggingReaderMiniAOD_cfg.py'

config.Data.inputDataset = '/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.inputDBS = 'global' 	#Variable to be used when analysing data
config.Data.useParent = True           	# Important!

config.Data.splitting = 'FileBased' #EventBased for generation, FileBased for analysing data 
config.Data.unitsPerJob = 100
#config.Data.totalUnits = 100 		# Only used during MC generation, comment out for analysis
#config.Data.useParent = True           # Important!


config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.publishDataName = 'CRAB3_MiniAODTest_150528a'

config.Site.storageSite = 'T2_UK_London_Brunel'







