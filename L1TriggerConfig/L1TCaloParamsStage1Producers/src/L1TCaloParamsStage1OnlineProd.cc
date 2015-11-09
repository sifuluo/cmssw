#include "CondTools/L1TriggerExt/interface/L1ConfigOnlineProdBaseExt.h"

//#include "L1Trigger/L1TCalorimeter/interface/L1TCaloParamsStage1Builder.h"

#include "CondFormats/L1TObjects/interface/CaloConfig.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "CondFormats/DataRecord/interface/L1TCaloConfigRcd.h"
#include "CondFormats/DataRecord/interface/L1TCaloParamsRcd.h"
#include "CaloParamsHelper.h"

class L1TCaloParamsStage1OnlineProd : public L1ConfigOnlineProdBaseExt<L1TCaloParamsRcd,l1t::CaloParams> {
private:
    // default parameters, that are not present in OMDS but have to be set for the emulator:
    std::vector<int>    etSumEtaMin, etSumEtaMax;
    std::vector<double> etSumEtThreshold;
    double towerLsbSum, regionLsb, jetLsb, egLsb;
  void hexStringToVector(const std::string& s, std::vector<uint32_t>& v);

public:
    virtual boost::shared_ptr<l1t::CaloParams> newObject(const std::string& objectKey) override ;

    L1TCaloParamsStage1OnlineProd(const edm::ParameterSet&);
    ~L1TCaloParamsStage1OnlineProd(void){}
};

L1TCaloParamsStage1OnlineProd::L1TCaloParamsStage1OnlineProd(const edm::ParameterSet& iConfig) :
    L1ConfigOnlineProdBaseExt<L1TCaloParamsRcd,l1t::CaloParams>(iConfig) {
    // fetch the default parameters from the configuration of this producer
    etSumEtaMin      = iConfig.getParameter< std::vector<int> >("etSumEtaMin");
    etSumEtaMax      = iConfig.getParameter< std::vector<int> >("etSumEtaMax");
    etSumEtThreshold = iConfig.getParameter< std::vector<double> >("etSumEtThreshold");
    towerLsbSum      = iConfig.getParameter<double>("towerLsbSum");
    regionLsb        = iConfig.getParameter<double>("regionLsb");
    jetLsb           = iConfig.getParameter<double>("jetLsb");
    egLsb            = iConfig.getParameter<double>("egLsb");
}

// Some intermediary funtions
void computeTauRangeEtaFromColumnEn(int tauColumnEn, int& tauMinEta, int& tauMaxEta) {
    std::vector<int> t;

    int prevBitValue = 0;
    int tauEta = 4; // Start ETA for Tau is 4 (CE)

    tauColumnEn &= 0x3FFF; // Filter number
    for (int i = 0; i < 15; i++) { // 14b + 1 for transition detection
        int bitValue = (tauColumnEn & (1 << i))?1:0;
        if (bitValue != prevBitValue) {
            t.push_back(tauEta + i);
        }
        prevBitValue = bitValue;
    }

    if (t.size() > 2 || t.size() == 0) { // Size will never be 0, just in case
        throw std::runtime_error("Strange tau column enable size");
    } else if (t.size() == 2) {
        tauMinEta = t.front();
        tauMaxEta = t.back()-1;
    } else if (t.size() == 1) {
        tauMinEta = tauEta;
        tauMaxEta = t.back()-1;
    }
}

void L1TCaloParamsStage1OnlineProd::hexStringToVector(const std::string& s, std::vector<uint32_t>& v)
{
    unsigned int len = s.length();
    // TODO: XCEPT_ASSERT((len % 8) == 0, tsexception::ConversionError, "Input string length isn't power of 8");

    v.clear();
    for (unsigned int i = 0; i < len; i += 8) {
        uint32_t x;
        std::stringstream ss;
        ss << std::hex << s.substr(i, 8);
        ss >> x;
        v.push_back(x);
    }
}

boost::shared_ptr<l1t::CaloParams> L1TCaloParamsStage1OnlineProd::newObject(const std::string& objectKey) {
    // Note: Key should be MP7_PP_CONF_KEY
    using namespace edm::es;

    std::string stage1CaloSchema = "CMS_S1CALOL2" ;
    edm::LogInfo( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Producing l1t::CaloParams with key =" << objectKey;

    //boost::shared_ptr< l1t::CaloParams > caloParams = L1TCaloParamsStage1Builder::build(defaultSettings);
    boost::shared_ptr< l1t::CaloParams > caloParams( new l1t::CaloParams() );
    l1t::CaloParamsHelper *m_params_helper =  new l1t::CaloParamsHelper();

    if (objectKey.empty()) {
        edm::LogInfo( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Key is empty, returning empty CaloParams";
        return caloParams;
    }

    // Get the keys to all subtables inside PP configuration
    std::vector< std::string > queryColumns;
    queryColumns.push_back( "PP_ALGO_REGS_KEY" ) ;
    queryColumns.push_back( "PILEUP_SUB_LUT_KEY" ) ;
    queryColumns.push_back( "EG_ISO_LUT_KEY" ) ;
    queryColumns.push_back( "TAU_ISO_LUT_KEY" ) ;
    queryColumns.push_back( "TAU_CALIB_LUT_KEY" ) ;
    queryColumns.push_back( "TAU_FIBER_LUT_KEY" ) ;
    queryColumns.push_back( "JET_CALIB_LUT_KEY" ) ;

    l1t::OMDSReader::QueryResults mp7_pp_conf =
        m_omdsReader.basicQuery( queryColumns,
                                 stage1CaloSchema,
                                 "MP7_PP_CONF",
                                 "MP7_PP_CONF.MP7_PP_CONF_KEY",
                                 m_omdsReader.singleAttribute(objectKey)
                               ) ;

    if( mp7_pp_conf.queryFailed() || mp7_pp_conf.numberRows() != 1 ){
        edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get MP7_PP_CONF" ;
        return boost::shared_ptr< l1t::CaloParams >() ;
    }

    std::string pp_algo_regs_key, pileup_sub_lut_key, eg_iso_lut_key, tau_iso_lut_key, tau_calib_lut_key, tau_fiber_lut_key, jet_calib_lut_key;
    mp7_pp_conf.fillVariable( "PP_ALGO_REGS_KEY", pp_algo_regs_key) ;
    mp7_pp_conf.fillVariable( "PILEUP_SUB_LUT_KEY", pileup_sub_lut_key ) ;
    mp7_pp_conf.fillVariable( "EG_ISO_LUT_KEY", eg_iso_lut_key ) ;
    mp7_pp_conf.fillVariable( "TAU_ISO_LUT_KEY", tau_iso_lut_key ) ;
    mp7_pp_conf.fillVariable( "TAU_CALIB_LUT_KEY", tau_calib_lut_key ) ;
    mp7_pp_conf.fillVariable( "TAU_FIBER_LUT_KEY", tau_fiber_lut_key ) ;
    mp7_pp_conf.fillVariable( "JET_CALIB_LUT_KEY", jet_calib_lut_key ) ;

    // Variables will be used later
    int energy_thr, jet_seed_thr, ht_sum_thr, tau_seed_thr, tau_neighbour_thr, tau_max_pt_veto, iso_tau_column_en;
    if (!pp_algo_regs_key.empty()) {
        queryColumns.clear();
        queryColumns.push_back( "ENERGY_THR" ) ;            // PUM energy threshold
        queryColumns.push_back( "JET_SEED_THR" ) ;          // Jet seed threshold
        queryColumns.push_back( "HT_SUM_THR" ) ;            // HT sum threshold
        queryColumns.push_back( "TAU_SEED_THR" ) ;          // Tau seed threshold
        queryColumns.push_back( "TAU_NEIGHBOUR_THR" ) ;     // Tau neighbour threshold
        queryColumns.push_back( "TAU_MAX_PT_TAU_VETO" ) ;   // Tau max pt veto
        queryColumns.push_back( "ISO_TAU_COLUMN_EN" ) ;     // Tau coloumn enable

        // select queryColumns from CMS_S1CALOL2.PP_ALGO_REGS where PP_ALGO_REGS_KEY = pp_algo_regs_key ;
        l1t::OMDSReader::QueryResults pp_algo_regs =
            m_omdsReader.basicQuery( queryColumns,
                                     stage1CaloSchema,
                                     "PP_ALGO_REGS",
                                     "PP_ALGO_REGS.PP_ALGO_REGS_KEY",
                                     m_omdsReader.singleAttribute(pp_algo_regs_key)
                                   ) ;

        if( pp_algo_regs.queryFailed() || pp_algo_regs.numberRows() != 1 ){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get PP_ALGO_REGS" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        pp_algo_regs.fillVariable( "ENERGY_THR",          energy_thr);
        pp_algo_regs.fillVariable( "JET_SEED_THR",        jet_seed_thr);
        pp_algo_regs.fillVariable( "HT_SUM_THR",          ht_sum_thr);
        pp_algo_regs.fillVariable( "TAU_SEED_THR",        tau_seed_thr);
        pp_algo_regs.fillVariable( "TAU_NEIGHBOUR_THR",   tau_neighbour_thr);
        pp_algo_regs.fillVariable( "TAU_MAX_PT_TAU_VETO", tau_max_pt_veto);
        pp_algo_regs.fillVariable( "ISO_TAU_COLUMN_EN",   iso_tau_column_en);

        // Parse all energy registers that are in 0.5GeV steps
        // TODO: Put energy_thr (pileup threshold) to proper place
        m_params_helper->setJetSeedThreshold(jet_seed_thr/2.);
        m_params_helper->setEtSumEtThreshold(0, ht_sum_thr/2.);
        m_params_helper->setTauSeedThreshold(tau_seed_thr/2.);
        m_params_helper->setTauNeighbourThreshold(tau_neighbour_thr/2.);
        m_params_helper->setTauMaxPtTauVeto(tau_max_pt_veto/2.);

        int minTauEta, maxTauEta;
        try {
            computeTauRangeEtaFromColumnEn(iso_tau_column_en, minTauEta, maxTauEta);
            m_params_helper->setIsoTauEtaMin(minTauEta);
            m_params_helper->setIsoTauEtaMax(maxTauEta);
        } catch (std::exception ex) {
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << ex.what();
            return boost::shared_ptr< l1t::CaloParams >() ;
        }
    } else {
        // Registers skipped
    }

    if (!pileup_sub_lut_key.empty()) {
        //*** Pileup subtraction LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults pileupLUT = m_omdsReader.basicQuery( queryColumns,
                                                                           stage1CaloSchema,
                                                                           "PILEUP_SUB_LUT",
                                                                           "PILEUP_SUB_LUT.PILEUP_SUB_LUT_KEY",
                                                                           m_omdsReader.singleAttribute(pileup_sub_lut_key)
                                                                           ) ;

        if( pileupLUT.queryFailed() || pileupLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get PILEUP_SUB_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        pileupLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

        // Convert the LUT to offline format
        if (energy_thr == 0) {
            std::vector<double> pumVector;
            for (size_t i = 0; i < rawData.size(); i++) {
                pumVector.push_back(rawData[i]/2.);
            }

            m_params_helper->setRegionPUSType("PUM0");
            m_params_helper->setRegionPUSParams(pumVector);
        } else {
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Energy threshold bigger than 0 but only PUM0 is supported" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }
    } else {
        // No LUT to load
    }

    if (!eg_iso_lut_key.empty()) {
        //*** EG isolation LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults egLUT = m_omdsReader.basicQuery( queryColumns,
                                                                       stage1CaloSchema,
                                                                       "EG_ISO_LUT",
                                                                       "EG_ISO_LUT.EG_ISO_LUT_KEY",
                                                                       m_omdsReader.singleAttribute(eg_iso_lut_key)
                                                                       ) ;

        if( egLUT.queryFailed() || egLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get EG_ISO_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        egLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

        // Convert the LUT to offline format
        std::ostringstream oss;
        oss << "#<header> V1 17 1 </header>" << std::endl;
        for (size_t i = 0; i < 1024; i++) {
            for (size_t j = 0; j < 32; j++) {
                oss << i * 32 + j << " " << ((rawData[i] & (1<<j))?1:0) << std::endl;
            }
        }
        for (size_t i = 1024; i < 1024 * 2; i++) {
            for (size_t j = 0; j < 32; j++) {
                oss << i * 32 + j << " " << ((rawData[i + 1024] & (1<<j))?1:0) << std::endl;
            }
        }

        for (size_t i = 1024 * 2; i < 1024 * 3; i++) {
            for (size_t j = 0; j < 32; j++) {
                oss << i * 32 + j << " " << ((rawData[i - 1024] & (1<<j))?1:0) << std::endl;
            }
        }

        for (size_t i = 1024 * 3; i < 1024 * 4; i++) {
            for (size_t j = 0; j < 32; j++) {
                oss << i * 32 + j << " " << ((rawData[i] & (1<<j))?1:0) << std::endl;
            }
        }

        // Create the istream for l1t::LUT
        std::istringstream is(oss.str());
        l1t::LUT lut(is);
        m_params_helper->setEgIsolationLUT(lut);
    } else {
        // No LUT to load
    }

    if (!tau_iso_lut_key.empty()) {
        //*** Tau isolation LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults tauIsoLUT = m_omdsReader.basicQuery( queryColumns,
                                                                           stage1CaloSchema,
                                                                           "TAU_ISO_LUT",
                                                                           "TAU_ISO_LUT.TAU_ISO_LUT_KEY",
                                                                           m_omdsReader.singleAttribute(tau_iso_lut_key)
                                                                           ) ;

        if( tauIsoLUT.queryFailed() || tauIsoLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get TAU_ISO_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        tauIsoLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

        // Convert the LUT to offline format
        std::ostringstream oss;
        oss << "#<header> V1 16 1 </header>" << std::endl;
        for (size_t i = 0; i < rawData.size(); i++) {
            for (size_t j = 0; j < 32; j++) {
                oss << i * 32 + j << " " << ((rawData[i] & (1<<j))?1:0) << std::endl;
            }
        }

        // Create the istream for l1t::LUT
        std::istringstream is(oss.str());
        l1t::LUT lut(is);
        m_params_helper->setTauIsolationLUT(lut);
    } else {
        // No LUT to load
    }

    if (!tau_calib_lut_key.empty()) {
        //*** Tau calibration LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults tauCalibLUT = m_omdsReader.basicQuery( queryColumns,
                                                                             stage1CaloSchema,
                                                                             "TAU_CALIB_LUT",
                                                                             "TAU_CALIB_LUT.TAU_CALIB_LUT_KEY",
                                                                             m_omdsReader.singleAttribute(tau_calib_lut_key)
                                                                             ) ;

        if( tauCalibLUT.queryFailed() || tauCalibLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get TAU_CALIB_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        tauCalibLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

 		if (rawData.size() != 1792) {
			edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Strange Tau Calibration LUT size of " << rawData.size() << ", expected 1792" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
		}

        // Convert the LUT to offline format
        std::ostringstream oss;
        oss << "#<header> V1 15 6 </header>" << std::endl;
        // Note: Won't start at 0, not sure if needed, but will fake initial data as 0
        for (size_t i = 0; i < 4096; i++) { oss << i << " 0" << std::endl; }

        for (size_t i = 0; i < rawData.size(); i++) {
            for (size_t j = 0; j < 4; j++) {
                oss << i * 4 + j + 4096 << " " << ((rawData[i] >> (j * 8)) & 0xFF) << std::endl;
            }
        }
 		for (size_t i = 0; i < 7; i++) {
			for (size_t k = 0; k < 256; k++) {
				for (size_t j = 0; j < 4; j++) {
					oss << (i * 256 + k) * 4 + j + 4096 + rawData.size() * 4 << " " << ((rawData[(6-i) * 256 + k] >> (j * 8)) & 0xFF) << std::endl;
				}
			}
		}

        // Create the istream for l1t::LUT
        std::istringstream is(oss.str());
        l1t::LUT lut(is);
        m_params_helper->setTauCalibrationLUT(lut);
    } else {
        // No LUT to load
    }

    if (!tau_fiber_lut_key.empty()) {
        //*** Tau fiber LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults tauFiberLUT = m_omdsReader.basicQuery( queryColumns,
                                                                             stage1CaloSchema,
                                                                             "TAU_FIBER_LUT",
                                                                             "TAU_FIBER_LUT.TAU_FIBER_LUT_KEY",
                                                                             m_omdsReader.singleAttribute(tau_fiber_lut_key)
                                                                             ) ;

        if( tauFiberLUT.queryFailed() || tauFiberLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get TAU_FIBER_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        tauFiberLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

        // Convert the LUT to offline format
        std::ostringstream oss;
        oss << "#<header> V1 6 3 </header>" << std::endl;
        for (size_t i = 0; i < rawData.size(); i++) {
            for (size_t j = 0; j < 8; j++) {
                oss << i * 8 + j << " " << ((rawData[i] >> (j * 4)) & 0xF) << std::endl;
            }
        }

        // Create the istream for l1t::LUT
        std::istringstream is(oss.str());
        l1t::LUT lut(is);
        m_params_helper->setTauEtToHFRingEtLUT(lut);
    } else {
        // No LUT to load
    }

    if (!jet_calib_lut_key.empty()) {
        //*** Jet calibration LUT ***//
        queryColumns.clear();
        queryColumns.push_back( "DATA" ) ;            // Hex LUT data
        l1t::OMDSReader::QueryResults jetCalibLUT = m_omdsReader.basicQuery( queryColumns,
                                                                             stage1CaloSchema,
                                                                             "JET_CALIB_LUT",
                                                                             "JET_CALIB_LUT.JET_CALIB_LUT_KEY",
                                                                             m_omdsReader.singleAttribute(jet_calib_lut_key)
                                                                             ) ;

        if( jetCalibLUT.queryFailed() || jetCalibLUT.numberRows() != 1){
            edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Cannot get JET_CALIB_LUT" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
        }

        std::string hexData;
        jetCalibLUT.fillVariable( "DATA",   hexData);
        std::vector<uint32_t> rawData;
        hexStringToVector(hexData, rawData);

 		if (rawData.size() != 2816) {
			edm::LogError( "L1-O2O: L1TCaloParamsStage1OnlineProd" ) << "Strange Jet Calibration LUT size of " << rawData.size() << ", expected 2816" ;
            return boost::shared_ptr< l1t::CaloParams >() ;
		}

        // Convert the LUT to offline format
        std::ostringstream oss;
        oss << "#<header> V1 15 6 </header>" << std::endl;
        // Note: Calibrations for ETA > 10 are not in hardware, but we replicate them
        for (size_t i = 0; i < rawData.size(); i++) {
            for (size_t j = 0; j < 4; j++) {
                oss << i * 4 + j << " " << ((rawData[i] >> (j * 8)) & 0xFF) << std::endl;
            }
        }
 		for (size_t i = 0; i < 11; i++) {
			for (size_t k = 0; k < 256; k++) {
				for (size_t j = 0; j < 4; j++) {
					oss << (i * 256 + k) * 4 + j + 11264 << " " << ((rawData[(10-i) * 256 + k] >> (j * 8)) & 0xFF) << std::endl;
				}
			}
		}

        // Create the istream for l1t::LUT
        std::istringstream is(oss.str());
        l1t::LUT lut(is);
        m_params_helper->setJetCalibrationLUT(lut);
    } else {
        // No LUT to load
    }


    // fill in the defaults, the code is borrowed from L1Trigger/L1TCalorimeter/plugins/L1TCaloParamsESProducer.cc
    if ((etSumEtaMin.size() == etSumEtaMax.size()) &&  (etSumEtaMin.size() == etSumEtThreshold.size())) {
      for (unsigned i=0; i<etSumEtaMin.size(); ++i) {
        m_params_helper->setEtSumEtaMin(i, etSumEtaMin.at(i));
        m_params_helper->setEtSumEtaMax(i, etSumEtaMax.at(i));
        m_params_helper->setEtSumEtThreshold(i, etSumEtThreshold.at(i));
      }
    }
    else {
      edm::LogError("l1t|calo") << "Inconsistent number of EtSum parameters" << std::endl;
    }
    m_params_helper->setTowerLsbSum(towerLsbSum);
    m_params_helper->setRegionLsb  (regionLsb);
    m_params_helper->setJetLsb     (jetLsb);
    m_params_helper->setEgLsb      (egLsb);

    caloParams = static_cast< boost::shared_ptr<l1t::CaloParams> > (m_params_helper);
    return caloParams;
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(L1TCaloParamsStage1OnlineProd);
