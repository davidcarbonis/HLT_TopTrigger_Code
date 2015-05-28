import FWCore.ParameterSet.Config as cms

process = cms.Process("USER")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

## Events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Input files
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#	'file:/nfs/data/eepgadm/ROOTfiles/SingleTop/MINIAODSIM/007B37D4-8B70-E411-BC2D-0025905A6066.root',
        'file:/nfs/data/eepgadm/ROOTfiles/SingleTop/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/08860A39-3770-E411-A0AE-0026189438AD.root',
    )
)

## Output file
process.TFileService = cms.Service("TFileService", fileName = cms.string('Output_TestMiniAOD.root'))

# Path and EndPath definitions
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
process.load('Configuration.StandardSequences.PATMC_cff')

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 

# Output definition
## Initialize analyzer
process.bTaggingReaderMiniAOD = cms.EDAnalyzer('BTaggingReaderMiniAOD',


    jets = cms.InputTag('slimmedJets'), # input jet collection name - miniAODSim uses 'slimmedJets'
    TriggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "HLT"),
    PathName = cms.untracked.string("HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"), 
##Old Algos
#HLT_Mu17_Mu8_v, HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1, HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1;

    debug = cms.untracked.bool(False),
    bDiscriminators = cms.vstring(     
        'combinedInclusiveSecondaryVertexV2BJetTags',
    )
)


# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1', '')

#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)

## Let it run
process.p = cms.Path(process.bTaggingReaderMiniAOD)
