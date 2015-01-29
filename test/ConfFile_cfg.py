import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
        )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/46B7A5FA-E36F-E411-9DCF-003048FF9AA6.root',
                                'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/242F3290-DF6F-E411-BD4D-0025905A6118.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/12F3E399-EF6F-E411-A634-00261894386E.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/027C4F3F-F26F-E411-A4E0-0025905A6138.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/02B8B1D3-CE6F-E411-9F6E-0025905A6084.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/04CF79D9-DD6F-E411-B846-0025905A60CE.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/061FD409-D96F-E411-882D-0025905A48BA.root'
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/08000795-B56F-E411-B734-0026189438E7.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/080A86F7-D86F-E411-A7EF-0025905B858C.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/08671001-E86F-E411-9556-0025905A60B0.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/0A5C9F19-D16F-E411-98A5-0025905A48F0.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/0CAAC988-C46F-E411-A657-0025905A48EC.root',
                                #'root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TT_Tune4C_13TeV-pythia8-tauola/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/0CFBE052-E26F-E411-A973-0025905A6088.root'
                                )
                            )

process.demo = cms.EDAnalyzer('DemoAnalyzer',

                                         minTracks=cms.untracked.uint32(-1)
                              )

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('hist.root')
                                   )


process.p = cms.Path(process.demo)
