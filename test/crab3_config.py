from CRABClient.client_utilities import config
config = config()

config.General.requestName = 'MC_SingleTop_PU40_25ns'
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'Crab3_runBTaggingReader_cfg.py'

config.Data.inputDataset = '/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM' #/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM, /TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFN = '/store/user/almorton/SingleTopPaths/150324'
config.Data.publication = False
config.Data.publishDataName = 'CRAB3_SingleTopPaths_150324'

config.Site.storageSite = 'T2_UK_London_Brunel'







