import FWCore.ParameterSet.Config as cms

#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1')

## Initialize analyzer
bTaggingEfficiency = cms.EDAnalyzer('BTaggingEfficiencyHLT',

    jets = cms.InputTag('slimmedJets::HLT'), # input jet collection name
    TriggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "TEST"),
    PathName = cms.untracked.string("HLT_IsoMu20_eta2p1_CentralPFJet30_BTagCSV07_v1"), 

## Old CSV trigger paths
# HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1, HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1, HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1;

## Current trigger paths
# HLT_IsoMu20_eta2p1_CentralPFJet30_BTagCSV07_v1
# HLT_Ele27_eta2p1_WP75_Gsf_CentralPFJet30_BTagCSV07_v1

    debug = cms.untracked.bool(False),
    bDiscriminators = cms.vstring(      # list of b-tag discriminators to access
        'combinedInclusiveSecondaryVertexV2BJetTags',
    )
)

