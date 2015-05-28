import FWCore.ParameterSet.Config as cms

process = cms.Process("USER")

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1')

process.load("FWCore.MessageService.MessageLogger_cfi")

## Events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Input files
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/nfs/data/eepgadm/ROOTfiles/SingleTop/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/08860A39-3770-E411-A0AE-0026189438AD.root',
    )
)

## Initialize analyzer
process.bTaggingReaderAOD = cms.EDAnalyzer('BTaggingReaderAOD',


    jets = cms.InputTag('ak4PFJetsCHS'), # input jet collection name - miniAODSim uses 'slimmedJets'
    TriggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "HLT"),
    PathName = cms.untracked.string("HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"), 
##Old Algos
#HLT_Mu17_Mu8_v, HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1, HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1;

    debug = cms.untracked.bool(False),
)

## Output file
process.TFileService = cms.Service("TFileService", fileName = cms.string('Output_AOD.root'))


## Let it run
process.p = cms.Path(process.myPartons*process.AK4Flavour*process.bTaggingReaderAOD)
